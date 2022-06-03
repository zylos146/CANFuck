# CANFuck - FieldBus-driven Fucking Machine Controller
CANFuck started as a driver for CANOpen-based LinMot drivers, but now supports a variety of integation options
- EtherCat
  - CiA 402 - Not yet supported, but might be in the future
    - Rtelligent ECT60 
    - LinMot EtherCat CiA 402 capable drives
- CANOpen
  - LinMot CANOpen capable drives (Motion / Monitoring / Configuration)
- ModBus
  - iHSV-57 (Monitoring / Configuration)
- Step/Dir Pulses

## Controller

### CANOpenNode

Currently loaded with
https://github.com/CANopenNode/CANopenNode/commit/8c7d852902b2d307e8b91a43332c14e366641e00