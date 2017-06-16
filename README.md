# SoccerBallDetection
---
\[THIS IS UNSTABLE VERSION\]
This project is aimed to be used as an integrated module to detect any the
soccer ball. This project is specifically designed to solve the problem of ball
detection in RoboCup humanoid and SPL leagues. Hence it provide to interfaces
useful in these environments.

## Requirements
* qt-sdk (for offline debugger and camera app)
* opencv

## Installation Guide
There are several ways to use this module:

### Camera Debugger App
The camera debugger application is an Qt GUI-based application that mounts the 
camera and runs the ball detection module on the camera stream in order to 
demonstrate the results easily. To run the offline debugger please do the 
following instruction under the <i>interfaces/camera/</i> directory:
```mkdir -p build
cd build
qmake ..
make
./ballDetection
```

### Offline Debugger App
The offline debugger is an Qt GUI-based application that runs the ball detection
module in order to demonstrate the results. To run the offline debugger please 
do the following instruction under the <i>interfaces/offline</i> directory:
```mkdir -p build
cd build
qmake ..
make
./ballDetection
```
The offline debugger use qt sdk to show the image in <i>interfaces/offline/images/</i> 
folder and it runs the detector module on the images.

### Windows Application
Windows application is a window-based debugger for ball detector modules.
To run this application, open ballDetectionWindows.sln (located in <i>interfaces/ballDetectionWindows/</i>)
via visual studio and run it. Please note the opencv has to be installed on your computer
and should be added to you visual studio (either local or global) configes.

### B-Human Integrated Module (BH2014):
To use the B-human interface, the code should be port into B-Human folder. To do 
so, please move the entire soccerBallDetection directory into the B-Human's code
 inside the perception folder, instead of the `ballperceptor` modules.

### Use Module Directly
You can also use the Ball Detector module source code. To do so, you only need 
to add the library include path to your project and include the `balldetector.h`.
Then, make an instance of it and call the `update` function and pass the image to it.
Finally, the results will be accessible by `getResult` function.

## Contacts:
Please let me know about any comment you might have about this project. <br/>
Aref Moqadam Mehr (a.moqadam@mrl-spl.ir)


