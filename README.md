# SoccerBallDetection
---
\[THIS IS UNSTABLE VERSION\]
This project is aimed to be used as an integrated module to detect any the
soccer ball. This project is specifically designed to solve the problem of ball
detection in RoboCup humanoid and SPL leagues. Hence it provide to interfaces
useful in these environments.

## Requirements
* qt-sdk (for offline debugger)

## Installation Guide
There are several ways to use this module:

### B-Human Integrated Module:
To use the B-human interface, the code should be port into B-Human folder.

### Use Module Directly
--> use the balldetector.h

### Install As Library
--> Compile and install .so file

### Run As Separate Process
--> connect with Shared Memory, etc.

### Use Offline Qt-Debugger
To run the offline debugger please do the following instruction in <i>interfaces/offline</i>:
```mkdir -p build
qmake ..
make
./ballDetection
```
The offline debugger use qt sdk to show the image in <i>interfaces/offline</i> folder and run the code on them.

## Contacts:
Please let me know about any comment you might have about this project.
Aref Moqadam Mehr (a.moqadammehr@mrl-spl.ir)


