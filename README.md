# SV Engine
SV Engine is simple server engine. It helps develop multi-play game on Unity or Unreal engine and other projects.

## Features
### Windows API
It used [netcpp](https://github.com/index1207/netcpp) library that based on IOCP.

### Logging
Through `Console` class to log server status.

### Generated Packet & Packet Handler
Write packet structure with json formated file at `define` directory and run `generate.py` or `PacketGenerator` project to generate **Packet** and **Packet Handler** source file.
All you have to do is only implement Packet Handler methods!

It will support many languages.

### Connections
Use connector library to connect to server.
your project can develop through session interaction events being called.

### Support languages
- **C++**	
	- ✅ server connector
	- ✅ generated packet
- **C#**
	- ✅ server connector
	- ❌ generated packet - <i>being implementing</i>


## How to setup
To use this server engine, to build `netcpp` library.
To build `netcpp`, follow this process.
1. `cd lib && mkdir bin`
2. `cmake -S netcpp -B bin`
3. Open `.sln` project and build by `Debug` & `Release` mods.