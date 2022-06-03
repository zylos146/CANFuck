# CANFuck - FieldBus-driven Fucking Machine Driver
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

CANFuck is built on PlatformIO through VSCode.

```# Talk about how to use ESP32 Parameters to configure drive without needing to use code. Intent is to allow just flashing an ESP32, and attaching it to any Machine after configuration via Web```

<a name="motion-supported"></a>
# Motion Generation
Two types of Motion Generation are planned
- Streaming
- Patterns

## Streaming
CANFuck will support ingesting Motion Commands or T-Codes and sending them directly to the Motor. These will act in a similar way to G-Code. Either a pre-built file can be loaded, or an external integration can stream them in.

Streaming/Loading T-Codes is not currently supported, but is a intended goal

## Patterns
Patterns allow a set of pre-built, real-time configurable strokes to be generated on-demand.
Pattern Generation happens all through the [StrokeEngine](https://github.com/theelims/StrokeEngine)


The following Patterns are offered by default


### Simple Stroke
Simple Stroke Pattern. It creates a trapezoidal stroke profile with 1/3 acceleration, 1/3 coasting, 1/3 deceleration. Sensation has no effect.

### Teasing or Pounding
The sensation value can change the speed ratio between in and out. Sensation > 0 makes the in-move faster (up to 5x) giving a hard pounding sensation. Values < 0 make the out-move going faster. This gives a more pleasing sensation. The time for the overall stroke remains always the same and only depends on the global speed parameter.

### Robo Stroke
Sensation controls the acceleration of the stroke. Positive value increase acceleration until it is a constant speed motion (feels robotic). Neutral is equal to simple stroke (1/3, 1/3, 1/3). Negative reduces acceleration into a triangle profile.

### Half'n'Half
Similar to Teasing or Pounding, but every second stroke is only half the depth. The sensation value can change the speed ratio between in and out. Sensation > 0 make the in move faster (up to 5x) giving a hard pounding sensation. Values < 0 make the out move going faster. This gives a more pleasing sensation. The time for the overall stroke remains the same for all strokes, even half ones.

### Deeper
The insertion depth ramps up gradually with each stroke until it reaches its maximum. It then resets and restarts. Sensations controls how many strokes there are in a ramp.

### Stop'n'Go
Pauses between a series of strokes. The number of strokes ramps from 1 stroke to 5 strokes and back. Sensation changes the length of the pauses between stroke series.

### Insist
Sensation reduces the effective stroke length while keeping the stroke speed constant to the full stroke. This creates interesting vibrational pattern at higher sensation values. With positive sensation the strokes will wander towards the front, with negative values towards the back.

### Jack Hammer
Vibrational pattern that works like a jack hammer. Vibrates on the way in and pulls out smoothly in one go. Sensation sets the vibration amplitude from 3mm to 25mm.

### Stroke Nibbler
Simple vibrational overlay pattern. Vibrates on the way in and out. Sensation sets the vibration amplitude from 3mm to 25mm.

### Custom Patterns
Further documentation on Custom Patterns can be found in [StrokeEngine Patterns](https://github.com/theelims/StrokeEngine/blob/main/Pattern.md)

<a name="drives-supported"></a>
# Drives Supported
CANFuck started as a driver for CANOpen-based LinMot drivers, but now supports a variety of integation options
- EtherCat
  - CiA 402 - Not yet supported, but might be in the future
    - :question: Rtelligent ECT60 / ECT40 Closed-Loop Servos
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
  - :heavy_check_mark: iHSV57 V6xx Servos 100/140/180W - **TODO, not yet implemented, but definitely will be**
  - :x: iHSV57 V5xx is **NOT SUPPORTED** due to differing communication protocol with V6xx drivers
- Step/Dir Pulses
  - :heavy_check_mark: TB6600 drives
  - :heavy_check_mark: Other generic Step/Dir stepper/servo drives

<a name="controllers-supported"></a>
# Controllers Supported
- :heavy_check_mark: Blynk (See below for instructions on setting Blynk up)
- :heavy_check_mark: CANFuck Controller (WIP)
- :heavy_check_mark: MIDI support is exposed, so any Control Surface using MIDI can interface with CANFuck
  - For example [Control Surface](https://github.com/tttapa/Control-Surface)
  - Many other communites have built resources for custom MIDI Controllers, and any of them can be interfaced to CANFuck

## MIDI Dictionary (TODO - Provide Table of MIDI CC sent / accepted)

## CANFuck Controller (TODO)
The Controller has 
- 4 Knobs w/ Buttons & LEDs - Provided by [Adafruit I2C QT Rotary Encoder](https://www.adafruit.com/product/4991)
- 480px x 128px Capacitive Touch Screen - FTDI EVE3 Video Accelerator - [CrystalFonts Screen](https://www.crystalfontz.com/product/cfaf480128a0039tca11-480x128-eve-bar-tft-lcd)
  - Optionally, the touch screen can be ignored for a cheaper screen - [Touchless Screen](https://www.crystalfontz.com/product/cfaf480128a0039tna11-480x128-eve-1u-display)

The UI will be built in LVGL and driven by a FTDI EVE3 Video Accelerator.
The Controller is currently intended to be attached to the ESP32 driving the Fucking Machine via MIDI over a M12 connection.

![Controller Mockup](/docs/controller-mockup.png)

## Blynk Dashboard (TODO)
Blynk is used for a very simple, but quick way of getting instant interaction with your Sex Machine.

```cpp
#define BLYNK_LINMOT_STATUS_PIN 0
#define BLYNK_LINMOT_STATE_PIN 1
#define BLYNK_LINMOT_ACTUAL_POSITION 4
#define BLYNK_LINMOT_DEMAND_CURRENT 5
#define BLYNK_LINMOT_DEMAND_POSITION 11
#define BLYNK_LINMOT_MODEL_TEMP 12
#define BLYNK_LINMOT_REAL_TEMP 13
#define BLYNK_LINMOT_MOTOR_VOLTAGE 14
#define BLYNK_LINMOT_POWER_LOSS 15

#define BLYNK_STATE_PIN 2
#define BLYNK_STATUS_PIN 3

#define BLYNK_DEPTH 6
#define BLYNK_STROKE 7
#define BLYNK_SPEED 8
#define BLYNK_SENSATION 9

#define BLYNK_LOG 10
```

| Blynk Pin  | Function |
| ------------- | ------------- |
| V0 | Motor Status  |
| V1  | Motor State  |

<a name="dashboards-supported"></a>
# Dashboards
## ESP32 Self-hosted Dashboard (TODO)


# Libraries Used
### CANOpenNode
Used for all CANOpen access, and provided by the separate [CANOpenNode-ESP32](https://github.com/zylos146/CANOpenNode-ESP32) library.

