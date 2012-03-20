Network Protocol
================

Messages in simulator are encoded using JSON format and are transmitted using
UDP. Every message have a type, which is determined by the `type` field. There
are 7 message types:

* `move`
* `bump`
* `turn`
* `change size`
* `change color`
* `who is there?`
* `there you see`

Each message also contains `id` field:

* for incoming message, that's an identifier of an agent;
* for outgoing message (`bump` and `there you see`), that's a port to which
  message should be sent.

There are type-specific fields for each message type. Those would be described
in more details shortly, but first it should be noted that of those 7 types, 4
are paired. Pairs are:

* `move` and `bump`
* `who is there?` and `there you see`

The reason we have pairs is that some messages may or should have some answer
from simulator. For example, moving to other position may result in bumping
into something, thus the `bump` message. And as network protocol is no place
for rhetorical questions, `who is there?` should be answered by `there you
see`.

Now let's take a closer look at each message type.


`move`
------

Agent sends that message to move to somewhere else.

*Type-specific fields*: `coordX`, `coordY`

Both fields are non-negative integers representing new (desirable) coordinates
of agent.

Agent may bump into something along the way. In that case simulator would send
`bump` message (see below).


`bump`
------

Simulator returns that message when agent bumps into something while moving to
the point specified by `move` command.

*Type-specific fields*: `coordX`, `coordY`

Both fields are non-negative integers. Those fields specify new position of
agent (i.e. point where the bump occured and thus agent have stopped).


`turn`
------

Agent sends that message to change its orientation.

*Type-specific fields*: `degrees`

`degrees` is a floating point number (represented internally by `double`, which
holds value in the range of ±1.7e±308 and about 15 digits after decimal point).
Agent turns by `degrees` clockwise. The number may be negative (to turn
counterclockwise).


`change size`
-------------

Agent sends that message to change its size.

*Type-specific fields*: `newDiameter`

`newDiameter` is non-negative integer specifying new diameter of agent.


`change color`
--------------

Agent sends that message to change its color.

*Type-specific fields*: `newColor`

TODO: figure out what colors we would use and how to represent them.


`who is there?`
---------------

Agent sends that message to get a list of object situated inside a circle with
the given centre and radius.

*Type-specific fields*: `coordX` `coordY`, `radius`

`coordX` and `coordY` are non-negative integers specifying the centre of a
circle. Radius is non-negative real number.

In response to that message simulator would send `there you see` message.


`there you see`
---------------

Simulator sends that message in response to `who is there?` message.

*Type-specific fields*: `objects`

`objects` is a list of lists each describing separate object. Each inner list
contains the following fields:

* `coordX` and `coordY`, describing coordinates of the object
* `width` and `height`, describing object's size
* `color`, describing object's color

TODO: just as in `change color` message, we need to figure out colors to use
and some representation for them.
