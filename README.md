# CANFuck - FieldBus-driven Fucking Machine Driver 
> :warning: **This is an experimental, actively unstable driver built only for CANfuck hardware currently** :warning: 

CANFuck's goal is to provide a unified integration point for Motors, Controllers, Drivers, and Motion Generation.
It handles all of the integration, and exposes a friendly end-user interface for configuration and usage.

This is recommended to be used with either
- [Open Source Sex Machine]()
- LinMot-based Custom Machines

but can be used for any other hardware.

> Table of contents  
> <span class="mono">¶</span>&emsp;[Getting Started](#getting-started)  
> <span class="mono">¶</span>&emsp;[Motion Generation](#motion-supported)  
> <span class="mono">¶</span>&emsp;[Drives Supported](#drives-supported)  
> <span class="mono">¶</span>&emsp;[Input/Control Methods Supported](#controllers-supported)  
> <span class="mono">¶</span>&emsp;[Dashboard](#dashboard)  

<a name="getting-started"></a>
# Getting Started (TODO)
CANFuck is only meant to run on ESP32 hardware, as it has some tight integrations with in.
Raspberry Pi's are not a supported target for this software.

CANFuck is built on PlatformIO through VSCode. Arduino IDE is not supported.

`git clone --recursive https://github.com/zylos146/CANFuck/`

<a name="motion-supported"></a>
# Motion Generation
TODO

<a name="drives-supported"></a>
# Drives Supported
CANFuck started as a driver for CANOpen-based LinMot drivers, but now supports a variety of integation options
- EtherCat
  - CiA 402 - Not yet supported, but might be in the future
    - :question: LinMot EtherCat CiA 402 capable drives (E1200-DC, etc)
- CANOpen
  - LinMot CiA 402 capable drives
    - :question: E1200-GP-xx
    - :heavy_check_mark: E1100-GP-xx
    - :heavy_check_mark: E1100-CO-xx
    - :heavy_check_mark: B1100-GP-xx
    - :heavy_check_mark: A1100-GP-LC
  - Other CiA 402 drives could likely be ported if needed
- ModBus
  - :clock10: iHSV57 V6xx Servos 100/140/180W
  - :x: iHSV57 V5xx is **NOT SUPPORTED** due to differing communication protocol with V6xx drivers
- Step/Dir Pulses
  - :heavy_check_mark: TB6600 drives
  - :heavy_check_mark: Other generic Step/Dir stepper/servo drives

<a name="controllers-supported"></a>
# Input/Control Methods Supported
- :clock10: [M5 Remote](https://github.com/ortlof/OSSM-M5-Remote)
- :clock10: MIDI support, so any Control Surface using MIDI can interface with CANFuck
  - For example [Control Surface](https://github.com/tttapa/Control-Surface)
  - Many other communites have built resources for custom MIDI Controllers, and any of them can be interfaced to CANFuck
- :clock10: [Control Websocket]()
  - Our Control Websocket will allow adjusting user-facing parameters such as Speed, Sensation, and Length
- :clock10: [Streaming Websocket]()
  - Our Streaming Websocket uses a buffered, syncronized (Timecode, Position) tuple to allow directly controlling motion
  - This is best used for content where the Position is known in advanced, such as Videos, Music, and other media/files
- :clock10: [T-Code over Serial USB](https://buttplug-spec.docs.buttplug.io/docs/stpihkal/protocols/tcode/)
  - This is a planned integration, but will **only be available over a direct USB connection**
  - This is because T-Code has hard real-time requirements as a protocol and cannot be safely supported over Bluetooth/Websocket

## MIDI Dictionary (TODO - Provide Table of MIDI CC sent / accepted)

<a name="dashboard"></a>
# Dashboard
