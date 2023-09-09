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

# Current State
This repository is being actively developed and discussed on the [KinkyMakers Discord](https://t.co/NJdd6mOPO1).

**Elims**, **Vampix**, and **Zylos** are the main devs working towards an improved OSSM/FuckIO/CANfuck driver for open-source sex machines.

There are three improved firmwares currently active
- CANfuck (this one)
- [FuckIO](https://github.com/theelims/FuckIO)
- [OSSM-Stroke](https://github.com/ortlof/OSSM-Stroke) meant to be used with the [M5Remote](https://github.com/ortlof/OSSM-M5-Remote)

I'm working on polishing up the CANfuck repo. experimental/master is pushed to master now. It's still an unstable release, but it does actually work end to end under certain criteria for A1100 drivers.

I have the following roadmap
- Polish/Finish Web Controller UI
- Polish startup/connection with LinMot
- Add Machine Configuration and Status pages
- Add Board Configuration page (For CANfuck vs OSSM hardware)
- Add iHSV57 support to StrokeEngine Experimental
- Productionize CANfuck v3 hardware so it can be assembled by JLCPCB automatically
- Build a release/beta version with flashing instructions on both ESP32 and ESP32-S3 based setups
- Seek out end-user testing of the software on their own machines

Items on the roadmap might change order, or what they are, but I do want to try it out on other people's setups sometime soon.
We'll have to see how soon "soon" is though, as it is still a hobby project and it takes a lot of time.
Some of these items are entire mini-projects of themselves as well, like the CANfuck v3 hardware.

I've also gotta organize a lot of this work into tickets, put it into a Github Projects or Trello board of some time.
I also wanna write up a bunch of Github Wiki documentation on how the internals work. 
@Draconos hardware will likely be the first test-bed of the CANfuck software on someone elses machine. I've been working with them closely for a lot of the prototyping phase and know their setup should be compatible as they have one of my CANfuck v2 prototypes.


# How can I contribute?
Both end-users with hardware willing to test out the software, and other developers are both very much welcome.

Once the above steps are done and we have a stable version to build on top of, we should be able to accept user contributions a lot easier as well. I know we've got several users who are interested in contributing like @muddy.toes, among others.

Of course if anyone is interested in assisting with some of these roadmap items, it will be a bit more complex, but I'd be happy to accept the help.

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
