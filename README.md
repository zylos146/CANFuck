# CANFuck - FieldBus-driven Fucking Machine Driver !!EXPERIMENTAL - WIP!!
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
> <span class="mono">¶</span>&emsp;[Controllers Supported](#controllers-supported)  
> <span class="mono">¶</span>&emsp;[Dashboards Supported](#dashboards-supported)  

<a name="getting-started"></a>
# Getting Started (TODO)
CANFuck is only meant to run on ESP32 hardware, as it has some tight integrations with in.
Raspberry Pi's are not a supported target for this software.

CANFuck is built on PlatformIO through VSCode. Arduino IDE is not supported.

`git clone --recursive https://github.com/zylos146/CANFuck/`

<a name="motion-supported"></a>
# Motion Generation
Motion Generation is provided via StrokeEngine

<a name="drives-supported"></a>
# Drives Supported
CANFuck started as a driver for CANOpen-based LinMot drivers, but now supports a variety of integation options
- EtherCat
  - CiA 402 - Not yet supported, but might be in the future
    - :question: LinMot EtherCat CiA 402 capable drives (E1200-DC, etc)
- CANOpen
  - LinMot CiA 402 CANOpen capable drives
    - :x: E1200-EC / E1200-DC are not supported unless EtherCat support is built
    - :heavy_check_mark: E1200-GP-xx (Haven't confirmed yet, but should work)
    - :heavy_check_mark: E1100-GP-xx (Haven't confirmed yet, but should work)
    - :heavy_check_mark: E1100-CO-xx (Haven't confirmed yet, but should work)
    - :heavy_check_mark: B1100-GP-xx
    - :heavy_check_mark: A1100-GP-LC
  - Other CiA 402 drives could likely be ported with minimal effort
- ModBus
  - :clock10: iHSV57 V6xx Servos 100/140/180W
  - :x: iHSV57 V5xx is **NOT SUPPORTED** due to differing communication protocol with V6xx drivers
- Step/Dir Pulses
  - :heavy_check_mark: TB6600 drives
  - :heavy_check_mark: Other generic Step/Dir stepper/servo drives

<a name="controllers-supported"></a>
# Controllers Supported
- :clock10: [M5 Remote](https://github.com/ortlof/OSSM-M5-Remote)
- :clock10: MIDI support, so any Control Surface using MIDI can interface with CANFuck
  - For example [Control Surface](https://github.com/tttapa/Control-Surface)
  - Many other communites have built resources for custom MIDI Controllers, and any of them can be interfaced to CANFuck

## MIDI Dictionary (TODO - Provide Table of MIDI CC sent / accepted)

# Libraries Used

- [CANOpenNode](https://github.com/CANopenNode/CANopenNode)
- [SvelteKit](https://kit.svelte.dev/)
- [Tailwind CSS](https://tailwindcss.com/)
- [DaisyUI](https://daisyui.com/)
- [tabler ICONS](https://tabler-icons.io/)
- [unplugin-icons](https://github.com/antfu/unplugin-icons)
- [svelte-modals](https://svelte-modals.mattjennings.io/)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)
