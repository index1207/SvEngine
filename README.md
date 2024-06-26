# SV Engine ![Build](https://github.com/index1207/SvEngine/actions/workflows/build.yml/badge.svg) ![Lang](https://img.shields.io/badge/language-C++20-blue) <br>

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
To build this project, clone this repository recursive(`git clone --recursive https://github.com/index1207/SvEngine`) and install python and install other modules.

### 1. Execute `Setup.bat`

### 2. Install python & modules
1. `pip install stringcase`