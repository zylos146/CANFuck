# Linmot Talk Configuration for B1100 / E1100 class devices
# Motion Control SW
## 16 Bit Interface Scaling
- Position Scale = 1000
- Max Speed Scale = 1000
- Acceleration Scale = 10000
  
## Errors & Warnings > Error Behavior > Quick Stop Error Behavior
- Position Lag Always = False

# CANOpen Interface
## NMT Error Control > Node Guarding Protocol should be disabled
- Guard time = 0
  
## NMT Error Control > Heartbeat Protocol should be enabled
- Consumer Time = 150ms
- Consumed Node ID = 1

## TxPDO1
- Transmission Type = Asyncronous (254)
- Inhibit Time = 10 ms
- Event Time = 100 ms

| Name              | Size    | UPID  |
| -----------       | ------- | ----- |
| Status Word       | UInt16  |       |
| State Var         | UInt16  |       |
| Logged Error Code | UInt16  | 1D96h |
| Warn Word         | UInt16  | 1D8Eh |

</br>

## TxPDO2
- Transmission Type = Asyncronous (254)
- Inhibit Time = 10 ms
- Event Time = 100 ms

| Name                    | Size    | UPID  | Scale   |
| -----------             | ------- | ----- | -----   |
| Actual Position         | SInt32  | 1B8Dh | 0.1 um  |
| Actual Velocity         | SInt32  | 1BAFh | 1 um/s  |

</br>

## TxPDO3
- Transmission Type = Asyncronous (254)
- Inhibit Time = 10 ms
- Event Time = 100 ms

| Name                    | Size    | UPID  | Scale   |
| -----------             | ------- | ----- | -----   |
| Motor Supply Voltage    | UInt16  | 1BBDh | 0.01 V  |
| Motor Power Losses      | UInt16  | 1BDDh | 0.1 W   |
| Model Ph1 Temp          | SInt16  | 1BE4h | 0.1 C   |
| Motor Temp 1            | UInt16  | 1E28h | 0.980 C |

</br>

## RxPDO 1

| Name                      | Size    | UPID  |
| -----------               | ------- | ----- |
| Control Word              | UInt16  |       |

</br>

## RxPDO 2

| Name                      | Size    | UPID  |
| -----------               | ------- | ----- |
| Motion Cmd Slave Header   | UInt16  |       |
| Motion Cmd Par Byte 0..5  | UInt32  |       |

</br>

## RxPDO 3

| Name                      | Size    | UPID  |
| -----------               | ------- | ----- |
| Motion Cmd Slave Header   | UInt16  |       |
| Motion Cmd Par Byte 6..7  | UInt16  |       |