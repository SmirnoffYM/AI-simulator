# Binary network protocol specification (version 3)

## Motivation for version 2

* `QLinkedList` can't be serialized into JSON using `qjson` (not with
  the standard capabilities anyway, and I, Alexander Batischev, don't
  really want to implement serializing by myself)
* Client said that JSON is too inefficient and he wants some binary
  protocol

## Motivation for version 3

* Protocol should be as "relative" as possible: agent shouldn't know
  in advance neither the map size, nor his current coordinates, nor
  where the "north" is; he should express his requests in terms of
  "how far I want to go" and "how much do I want to turn", not "where
  exactly I want to go" and "how I want to be oriented".

So here we go!

## Overview

UDP is used as underlying protocol to transmit binary-encoded
messages.

Each message consists of two parts: the header and the body. Header
carries information about sender, while body contains command and some
parameters.

Upon processing the message simulator may send some response. For
`move` it may be either `bump` or nothing (if bump didn't happen). For
`who is there?` it would be `there you see`. Look up relevant sections
for details on those messages.

There are the following types of messages:

* `move`
* `turn`
* `change size`
* `change color`
* `who is there?`
* `parameter report`
* `bump`
* `there you see`
* `start`
* `pause`

Out of those, only first 6 can be sent by agent, and the last 4 can be
sent only by the simulator.

Following are formal specifications of how does header and each message
type look like.

## Header format

Header contains:

* protocol version, *1 octet*
* message's sequential number, *4 octets*, unsigned integer
* environment object's ID, *1 octet*, unsigned integer
* agent's port, *2 octets*, unsigned integer
* message type, *1 octet*

This document describes protocol version 3, so the first octet of the
message should contain 3.

Sequential numbers are set by the agent and required by him to make
sense out of responses. Simulator should just copy that field from the
message he is responding to.

Environment object's ID should be 0 when message is sent by/to the
robot and non-zero when message is sent by/to the environment
controller.

Message types are mapped from names to numbers as follows:

* 0: `move`
* 1: `turn`
* 2: `change size`
* 3: `change color`
* 4: `who is there?`
* 5: `bump`
* 6: `there you see`
* 7: `parameter report`
* 8: `start`
* 9: `pause`

## `move` message

Agent sends that message to move somewhere.

Message contains:

* distance, *4 octets*, unsigned integer

The parameter specifies how far to travel in the direction that agent
is facing (see `turn` message). If agent bumps into something on the
way, he would receive `bump` message.

## `turn` message

Agent sends that message to change his orientation.

Message contains:

* seconds, *4 octets*, integer

second is 1/60 of minute, and minute is, in turn, 1/60 of degree. The
parameter specifies how much agent should turn in clockwise direction.
Negative value means turning in counterclockwise direction.

## `change size` message

Agent sends that message to change his size.

Message contains:

* diameter, *4 octets*, unsigned integer

## `change color` message

Agent sends that message to change his color.

Message contains:

* red value, *1 octet*
* green value, *1 octet*
* blue value, *1 octet*

## `who is there?` message

Agent sends that message to get a list of objects situated not further
from him than a specified distance. Landscape can influence visibility
of the objects.

Message contains:

* distance, *4 octets*, unsigned integer

## `parameter report` message

Agent sends that message to report the state of one of his current
parameters.

Mesage contains:

* parameter id, *1 octet*
* integral part of the value, *4 octets*, signed integer
* real part of the value, *4 octets*, unsigned integer

Parameter id is an unsigned integer number in the range of [0; 15].

Parameter's value is split into integral part and first 6 digits of
real part. Note that real part is unsigned - sign is carried with
integral part.

## `bump` message

Simulator returns that message when agent bumps into something while
moving.

Message contains:

* distance, *4 octets*, unsigned integer

The parameter contains the distance agent has traveled before bumping
into the object.

## `there you see` message

Simulator sends that message in response to `who is there?` message.

Message contains:

* number of objects found, *4 octets*, unsigned integer
* list of objects

Each object is represented as follows:

* type, *1 octet*
* azimuth, *4 octets*, unsigned integer
* distance, *4 octets*, unsigned integer

There are the following types of objects:

* 0: boundary of the map
* 1: other agent (possibly an environment object)

Agent objects (type 1) have five more fields:

* diameter, *4 octets*, unsigned integer
* orientation, *4 octets*, unsigned integer
* red, green and blue components of color, *1 octet* each

Azimuth (measured in seconds relatively to the direction agent is
facing) and distance parameters specify position of the object
relative to the agent's position. Diameter, orientation (measured in
seconds relatively to the direction agent is facing) and color
describe the object.

List of objects is just a stream of objects descriptions.

## `start` and `pause` messages

Those are used to control simulation flow. Agent program should start
doing its work (i.e. moving agent around) upon receiving `start`
message. If `pause` is sent, agent should pause and wait for the
`start`. There's no `stop` message because agent processes are being
killed by the simulator when user wants to stop simulation.

Those messages doesn't contain anything other than header.

## Example messages

### `move` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x00   -- message's sequential number
0x00                  -- env. obj. ID is 0, thus message is for robot
0x04 0x01             -- agent's port, 1025
0x00                  -- "move" message
0x00 0x00 0x00 0x15   -- distance, 21
```

### `bump` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x00   -- message's sequential number, copied from the
                      -- original message
0x00                  -- env. obj. ID, zero means we're talking about robot
0x04 0x01             -- agent's port, 1025
0x06                  -- "bump" message
0x00 0x00 0x00 0x0b   -- distance, 11
```

### `change color` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x01   -- message's sequential number, 1
0x03                  -- 3, env. object's ID
0x04 0x01             -- agent's port, 1025
                      -- agent here is environment controller application
0x03                  -- "change color" message
0xa1                  -- red component, 161
0xb2                  -- green component, 178
0xc3                  -- blue component, 195
```

### `there you see` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x02   -- message's sequential number, 2
                      -- (answering to some hyphotetical request)
0x00                  -- env. obj. ID, zero means robot
0x04 0x01             -- agent's port, 1025
0x07                  -- "there you see" message
0x03                  -- number of objects found, 3
                      -- description of first object starts here
0x01                  --     some agent
0x00 0x00 0x00 0x10   --     azimuth, 16 seconds
0x00 0x00 0x00 0x12   --     distance, 18
0x00 0x00 0x00 0x15   --     diameter, 21
0x00 0x00 0x01 0x68   --     orientation, 360 seconds (one degree)
0x7f                  --     red component, 127
0xda                  --     green component, 218
0x3d                  --     blue component, 61
                      -- description of second object starts here
0x00                  --     boundary of the map
0x00 0x00 0x00 0x5d   --     azimuth, 93
0x00 0x00 0x09 0x2a   --     distance, 2346
                      -- description of third object starts here
0x01                  --     some agent
0x00 0x0d 0xa6 0x3b   --     azimuth, 894523 seconds (248° 28' 43")
0x00 0x08 0xa7 0xfb   --     distance, 567291
0x00 0x00 0x08 0x6c   --     diameter, 2165
0x00 0x03 0x4b 0xc0   --     orientation, 216000 (60 degrees)
0x5c                  --     red component, 92
0x41                  --     green component, 65
0x04                  --     blue component, 4
```

## Note about endianness

All integer values larger than an octet should be converted to network
byte order (big endian).

<!-- vim: set tw=70 : -->
