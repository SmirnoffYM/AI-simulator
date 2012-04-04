# Binary network protocol specification

## Motivation

* `QLinkedList` can't be serialized into JSON using `qjson` (not with
  the standard capabilities anyway, and I, Alexander Batischev, don't
  really want to implement serializing by myself)
* Client said that JSON is too inefficient and he wants some binary
  protocol

So here we go!

## Overview

UDP is used as underlying protocol to transmit binary-encoded messages.

Each message consists of two parts: the header and the body. Header
carries information about sender, while body contains command and some
parameters.

Upon processing the message simulator should send some response. It may
be either some dull message (see "`acknowledge` message") or answer to
the question that agent asked (see "`there you see` message" and "`bump`
message").

There are 9 types of messages:

* `move`
* `turn`
* `change size`
* `change color`
* `who is there?`
* `parameter report`
* `acknowledge`
* `bump`
* `there you see`

Out of those, only first 6 can be sent by agent, and the last three
can be sent only by the simulator.

Following are formal specification of how does header and each message
type look like.

## Header format

Header contains:

* message's sequential number, *4 octets*, unsigned integer
* agent's port, *2 octets*, unsigned integer
* message type, *1 octet*

Sequential numbers are set by the agent and required by him to make
sense out of responses. Simulator should just copy that field from the
message it is responding to.

Message types are mapped from names to numbers as follows:

* 0: `acknowledge`
* 1: `move`
* 2: `turn`
* 3: `change size`
* 4: `change color`
* 5: `who is there?`
* 6: `bump`
* 7: `there you see`
* 8: `parameter report`

## `move` message

Agent sends that message to move to somewhere else.

Message contains:

* X coordinate, *4 octets*, unsigned integer
* Y coordinate, *4 octets*, unsigned integer

## `turn` message

Agent sends that message to change its orientation.

Message contains:

* seconds, *4 octets*, signed integer

seconds are 1/60 of minute, and minute is in turn 1/60 of degree.  That
field specify number of seconds agent should turn clockwise.  To turn
counterclockwise, one should specify negative number of seconds.

## `change size` message

Agent sends that message to change its size.

Message contains:

* diameter, *4 octets*, unsigned integer

## `change color` message

Agent sends that message to change its color.

Message contains:

* red value, *1 octet*
* green value, *1 octet*
* blue value, *1 octet*

## `who is there?` message

Agent sends that message to get a list of object situated inside a
circle with the given centre and radius.

Message contains:

* X coordinate, *4 octets*, unsigned integer
* Y coordinate, *4 octets*, unsigned integer
* radius, *4 octets*, unsigned integer

## `parameter report` message

Agent sends that message to report the state of one of his current
parameters.

Mesage contains:

* string length, *1 octet*, unsigned
* string containing value of the parameter, *varying length*

String should have a format "%.15e", i.e. scientific format with 15 digits
after decimal point, e.g. 1.214509386098369e-308, 2.354635736300000e+32 and
\0.000000000000000e+00.

## `acknowledge` message

Just a dumb message simulator sends in response to messages that doesn't
require any meaningful answer. Those messages are:

* `turn`
* `change size`
* `change color`

Message doesn't contain anything.

## `bump` message

Simulator returns that message when agent bumps into something while
moving to the point specified by `move` command. It contains new
coordinates of the agent.

Message contains:

* X coordinate, *4 octets*, unsigned integer
* Y coordinate, *4 octets*, unsigned integer

## `there you see` message

Simulator sends that message in response to `who is there?` message.

Message contains:

* number of objects found, *4 octets*, unsigned integer
* list of objects

Each object is represented as follows:

* X coordinate, *4 octets*, unsigned integer
* Y coordinate, *4 octets*, unsigned integer
* diameter, *4 octets*, unsigned integer
* seconds, *4 octets*, integer
* red, green and blue components of color, *1 octet* each

Seconds field indidates orientation of object.

List of objects is just a stream of objects descriptions.

## Example messages

### `move` message

```
0x00 0x00 0x00 0x00   -- message's sequential number
0x04 0x01             -- agent's port, 1025
0x01                  -- "move" message
0x00 0x00 0x00 0x15   -- x coordinate, 21
0x00 0x00 0x00 0x2a   -- y coordinate, 42
```

### `bump` message

```
0x00 0x00 0x00 0x00   -- message's sequential number, copied from the
                      -- original message
0x04 0x01             -- agent's port, 1025
0x06                  -- "bump" message
0x00 0x00 0x00 0x0b   -- x coordinate, 11
0x00 0x00 0x00 0x0f   -- y coordinate, 15
```

### `change color` message

```
0x00 0x00 0x00 0x01   -- message's sequential number, 1
0x04 0x01             -- agent's port, 1025
0x04                  -- "change color" message
0xa1                  -- red component, 161
0xb2                  -- green component, 178
0xc3                  -- blue component, 195
```

### `acknowledge` message

```
0x00 0x00 0x00 0x01   -- message's sequential number, 1
0x04 0x01             -- agent's port, 1025
0x00                  -- "acknowledge" message
```

### `there you see` message

```
0x00 0x00 0x00 0x02   -- message's sequential number, 2
                      -- (answering to some hyphotetical request)
0x04 0x01             -- agent's port, 1025
0x07                  -- "there you see" message
0x03                  -- number of objects found, 3
                      -- description of first object starts here
0x00 0x00 0x00 0x10   --     x coordinate, 16
0x00 0x00 0x00 0x12   --     y coordinate, 18
0x00 0x00 0x00 0x15   --     diameter, 21
0x7f                  --     red component, 127
0xda                  --     green component, 218
0x3d                  --     blue component, 61
                      -- description of second object starts here
0x00 0x00 0x00 0x5d   --     x coordinate, 93
0x00 0x00 0x09 0x2a   --     y coordinate, 2346
0x00 0x00 0x00 0x03   --     diameter, 3
0x05                  --     red component, 5
0x07                  --     green component, 7
0x0b                  --     blue component, 11
                      -- description of third object starts here
0x00 0x0d 0xa6 0x3b   --     x coordinate, 894523
0x00 0x08 0xa7 0xfb   --     y coordinate, 567291
0x00 0x00 0x08 0x6c   --     diameter, 2165
0x5c                  --     red component, 92
0x41                  --     green component, 65
0x04                  --     blue component, 4
```

## Note about endianness

All integer values larger than an octet should be converted to network
byte order (big endian).
