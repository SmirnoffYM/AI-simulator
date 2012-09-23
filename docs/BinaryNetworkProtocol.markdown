# Binary network protocol specification (version 3)

## Motivation for version 2

* `QLinkedList` can't be serialized into JSON using `qjson` (not with
  the standard capabilities anyway, and I, Alexander Batischev, don't
  really want to implement serializing by myself)
* Client said that JSON is too inefficient and he wants some binary
  protocol

## Motivation for version 3

* Agent should not know its absolute position on the map. All
  coordinates should be expressed relatively to the agent itself.
* I made a mistake specifying `move` to have *optional* parameter.
  That led to slow and quite unreliable decisions, namely using
  timeouts to wait for response.

So here we go!

## Overview

UDP is used as underlying protocol to transmit binary-encoded
messages.

Each message consists of two parts: the header and the body. Header
carries information about sender, while body contains command and some
parameters.

Upon processing the message simulator would send some response. For
`move` it would be either `bump` or `moved successfully` (if bump
didn't happen). For `who is there?` it would be `there you see`. Look
up relevant sections for details on those messages.

There are the following types of messages:

* `move`
* `turn`
* `change size`
* `change color`
* `who is there?`
* `parameter report`
* `bump`
* `moved successfully`
* `there you see`
* `start`
* `pause`

Out of those, only first 6 can be sent by agent, and the last 5 can be
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

*  0: `move`
*  1: `turn`
*  2: `change size`
*  3: `change color`
*  4: `who is there?`
*  5: `bump`
*  6: `moved successfully`
*  7: `there you see`
*  8: `parameter report`
*  9: `start`
* 10: `pause`

## `move` message

Agent sends that message to move somewhere.

Message contains:

* X coordinate, *4 octets*, signed integer
* Y coordinate, *4 octets*, signed integer

Parameters describe point on Cartesian plane where agent wants to move
to. Agent is placed in the center of the plane, and axes are always
oriented horizontally and vertically (thus agent's orientation doesn't
affect them).

If agent bumps into something on the way, he would receive `bump`
message, otherwise there would be `moved successfully` message.

## `turn` message

Agent sends that message to change his orientation.

Message contains:

* degrees, *4 octets*, unsigned integer

The parameter specifies how much agent should turn in clockwise
direction relatively to its current orientation.

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

In response to that request, simulator would send `there you see`
message.

## `parameter report` message

Agent sends that message to report current state of one of his
parameters (which are totally user-defined).

Message contains:

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

* X coordinate, *4 octets*, signed integer
* Y coordinate, *4 octets*, signed integer

Parameters specify agent's position on Cartesian plane with agent's
previous position as a center.

## `moved successfully` message

Simulator returns that message when he agent doesn't bump into something
while moving. There's no payload in this message.

## `there you see` message

Simulator sends that message in response to `who is there?` message.

Message contains:

* number of objects found, *4 octets*, unsigned integer
* list of objects

Each object is represented as follows:

* type, *1 octet*
* X coordinate, *4 octets*, signed integer
* Y coordinate, *4 octets*, signed integer

There are the following types of objects:

* 0: boundary of the map
* 1: other agent (possibly an environment object)

Agent objects (type 1) have five more fields:

* diameter, *4 octets*, unsigned integer
* orientation, *2 octets*, unsigned integer
* red, green and blue components of color, *1 octet* each

X and Y parameters specify position of the object relative to the
agent's position. Diameter, orientation (measured in degrees
relatively to the "north") and color describe the object.

List of objects is just a stream of objects descriptions.

## `start` and `pause` messages

Those are used to control simulation flow. Agent program should start
doing its work (i.e. moving agent around) upon receiving `start`
message. If `pause` is sent, agent should pause and wait for the
`start`. There's no `stop` message because agent processes are being
killed by the simulator when user wants to stop simulation.

No `start` is being sent when agent is run by the simulator, i.e.
agents should start doing its work right away.

Those messages doesn't contain anything other than header.

## Example messages

### `move` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x00   -- message's sequential number
0x00                  -- env. obj. ID is 0, thus message is for robot
0x04 0x01             -- agent's port, 1025
0x00                  -- "move" message
0x00 0x00 0x00 0x15   -- X coordinate, 21
0xff 0xff 0xff 0xeb   -- Y coordinate, -21
```

### `bump` message

```
0x03                  -- 3, protocol version
0x00 0x00 0x00 0x00   -- message's sequential number, copied from the
                      -- original message
0x00                  -- env. obj. ID, zero means we're talking about robot
0x04 0x01             -- agent's port, 1025
0x06                  -- "bump" message
0x00 0x00 0x00 0x0b   -- X coordinate, 11
0xff 0xff 0xff 0xf5   -- Y coordinate, -11
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
0x00 0x00 0x00 0x10   --     X coordinate, 16
0xff 0xff 0xff 0xf0   --     Y coordinate, -16
0x00 0x00 0x00 0x15   --     diameter, 21
0x01 0x68             --     orientation, 359 degrees (one degree)
0x7f                  --     red component, 127
0xda                  --     green component, 218
0x3d                  --     blue component, 61
                      -- description of second object starts here
0x00                  --     boundary of the map
0x00 0x00 0x00 0x5d   --     x coordinate, 93
0x00 0x00 0x09 0x2a   --     Y coordinate, 2346
                      -- description of third object starts here
0x01                  --     some agent
0x00 0x0d 0xa6 0x3b   --     X coordinate, 894523
0x00 0x08 0xa7 0xfb   --     Y coordinate, 567291
0x00 0x00 0x08 0x6c   --     diameter, 2165
0x00 0x2a             --     orientation, 21 degrees
0x5c                  --     red component, 92
0x41                  --     green component, 65
0x04                  --     blue component, 4
```

## Note about endianness

All integer values larger than an octet should be converted to network
byte order (big endian).

<!-- vim: set tw=70 : -->
