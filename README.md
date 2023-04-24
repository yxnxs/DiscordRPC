# DiscordRPC
Discord RPC Client for Windows (focused on aesthetics)

This library is modeled as closely as possible after the [Discord Docs](https://discord.com/developers/docs/topics/gateway-events#activity-object) for presence.
Due to the fact that these docs show the activity object as a mix of the "old" RPC structure used here and the new DiscordSDK structure, it contains fields 
that are not selectable with this library. Furthermore, this library was designed for purely aesthetic use, therefore functionality for game secrets was not implemented.

The remaining fields are therefore: 
+ Timestamps
+ Details
+ State
+ Buttons

This library allows the design of an activity using ``RPC::Activity``, provided by the ``activity.hpp`` header. You can either create the activity yourself
using the various setters (these follow the builder patter), or load it directly from a .json file using ``RPC::Activity::FromFile()``.