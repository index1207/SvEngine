# SV Engine
SV Engine is simple server engine. It helps develop multi-play game on Unity or Unreal engine and other projects.

## Features
### 
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


## Setup
To use this server engine, firstly clone third-party libraries and build binary at `lib/bin` directory. <br>
Secondly, install python and install other modules.
### 1. Build netcpp
1. `cd lib && mkdir bin`
2. `cmake -S netcpp -B bin`
3. Open `.sln` project and build by `Debug` & `Release` mods.

### 2. Install python & modules
1. `pip install stringcase`