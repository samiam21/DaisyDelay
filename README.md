# DaisyPedal

To get started with programming the ElectroSmith Daisy in Arduino [Getting Started (Arduino Edition) Wiki page!](https://github.com/electro-smith/DaisyWiki/wiki/1a.-Getting-Started-(Arduino-Edition))

## Environment

This project is designed to build with VSCode using the Arduino extension.  It _should_ also build in the Arduino IDE but is untested.

* VS Code - https://code.visualstudio.com/
* VS Code Arduino Extension - https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino

## Configuration

The files within the .vscode folder may need updated for your environment.  Below is a description of each and what you may need to change.

### arduino.json

This file contains information on the board and connections.

Notes:
* The "output" definition allows VS Code to re-use compiled libraries, rather than needing to compile them every time.
* You will likely need to update the port to reference the COM port on your machine, for when the device is running, not in program mode.
* The "sketch" definition defines which Arduino sketch is compiled.  This project only has a top-level sketch so this should not be modified.

### c_cpp_properties.json

This file contains include paths and other information required for compiling C/C++ code.

Notes:
* You may need to update the "includePath" and "forcedInclude" definitions to point to the correct locations on your machine.

