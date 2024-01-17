# SV Engine
SV Engine is simple server engine. It helps develop multi-play game on Unity or Unreal engine and other projects.

## Features
### Windows API
It used [netcpp](https://github.com/index1207/netcpp) library that based on IOCP.

### Logging
Through `Console` class to log server status.

### Generated Packet & Packet Handler
Write packet structure with json format on `define.json` and run `generate.py` to generate **Packet** and **Packet Handler** source file.
All you have to do is only implement Packet Handler methods!

It will support many languages.

### Connections
Use connector library to connect to server.
your project can develop through session interaction events being called.

#### Session Events
- On Connect
- On Disconnect
- On Receive

### Support languages
- <details>
    <summary><b>C++</b></summary>
	&nbsp; &nbsp;<input type="checkbox" checked> server connector <br>
	&nbsp; &nbsp;<input type="checkbox" checked> generated packet
</details>

- <details>
	<summary><b>C#</b></summary>
	&nbsp; &nbsp;<input type="checkbox" checked> server connector <br>
	&nbsp; &nbsp;<input type="checkbox"> generated packet - <i>being implementing</i>
</details>
