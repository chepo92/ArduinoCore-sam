
# Arduino Core for SAM4 CPU, Baremetal branch 

This repository contains the source code and configuration files of the Arduino Core for Atmel's SAM4E processor.

## Installation on Arduino IDE

Copy or clone repo content into: 

C:\Users\{User}}\AppData\Local\Arduino15\packages\arduino\hardware\{give a name Eg: sam4E-git}\0.0.1

Then the boards will be listed under "ARM SAM4 (32-bits) Boards Baremetal"
## Installation on VSCode

Use Platformio extension with atmelsam framework

```
[env:due]
platform = atmelsam
framework = arduino
board = due
    
```

Development SAM4 Version

```
[env:sam4s4a_SAM4]
platform = atmelsam
framework = arduino
board = motionbow
platform_packages =
    framework-arduino-sam@https://github.com/chepo92/ArduinoCore-sam.git#dev-SAM4SCore
    toolchain-gccarmnoneeabi @ 1.40804.0

```

## Requirements 

arm-none-eabi-gcc v4.8.3-2014q1 

## Support

There is a dedicated section of the Arduino Forum for general discussion and project assistance:

http://forum.arduino.cc/index.php?board=87.0

## Bugs or Issues

If you find a bug you can submit an issue here on github:

https://github.com/arduino/ArduinoCore-sam/issues

Before posting a new issue, please check if the same problem has been already reported by someone else to avoid duplicates.

## Contributions

Contributions are always welcome.
The preferred way to receive code contribution is by submitting a Pull Request on github.

