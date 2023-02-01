# UE4-Dumper
Dump offsets and functions for games made using Unreal Engine 

# What it does exactly?
1. It regenerates and dumps all strings from the games using the Global Names pointer (GNames/FNames).
2. It dumps all objects present in the game using the Global Names (GNames/FNames) & Global Object(GUObjectArray).
3. It dumps the offsets and function address using Global Names (GNames/FNames) and Global Object(GUObjectArray) or using Global World (UWOrld/GWorld).
4. It also dumps the Actors present as entity in the game to check the integrity of the Global World pointer.

# What to do with the outputs?
Its upto you how you want to use the dump files. For general knowledge, the output files are used as reference to reverse engineer the game to access non-accessable options/ unfair advantages.

# How to build this source?
This was made using VS 2017 (Year - 2022) and can be easily rebuild. 
Change the offsets required, most games use the offsets provided for dumping and you just need to update the Global pointers but in case it doesn't dump correctly consider making somee changes to the pre initialized offsets in 'Offsets.h' file

# Failed to get Base Address of games?
1. Use a driver to read memory rather using the winapi callbacks. In case you don't have a driver or don't know about making one refer to youtube/github for public sources.
2. You can also disable any anticheat running as they can block R/W permsisson.

As for the Process id, it will actually be auto selected even there are multiple instances of the same game, the func searches for the game and checks for the highest memory usage among the instances.

# What games does it work on?
It will work on any game built using Unreal Engine no matter the version used. 
This source or dumper is not intended to be any game specific, its for educational purposes, so people can learn.
