# PDO Configuration for A1100 class devices

## TxPDO1

| Name              | Size    | UPID  |
| -----------       | ------- | ----- |
| Status Word       | UInt16  |       |
| State Var         | UInt16  |       |
| Logged Error Code | UInt16  | 1D96h |
| Warn Word         | UInt16  | 1D8Eh |

## TxPDO2

| Name                    | Size    | UPID  | Scale   |
| -----------             | ------- | ----- | -----   |
| Actual Position         | SInt32  | 1B8Dh | 0.1 um  |
| Actual Velocity         | SInt32  | 1BAFh | 1 um/s  |

## TxPDO3

| Name                    | Size    | UPID  | Scale   |
| -----------             | ------- | ----- | -----   |
| Motor Supply Voltage    | UInt16  | 1BBDh | 0.01 V  |
| Motor Power Losses      | UInt16  | 1BDDh | 0.1 W   |
| Model Ph1 Temp          | SInt16  | 1BE4h | 0.1 C   |
| Motor Temp 1            | UInt16  | 1E28h | 0.980 C |

## TxPDO4

| Name                    | Size    | UPID  | Scale |
| -----------             | ------- | ----- | ----- |
| Actual Force            | SInt32  | 1BFFh | 0.1 N |

## RxPDO 1

| Name                      | Size    | UPID  |
| -----------               | ------- | ----- |
| Control Word              | UInt16  |       |
| Motion Cmd Header         | UInt16  |       |
| Motion Cmd Par Byte 0..3  | UInt32  |       |

## RxPDO 2

| Name                      | Size    | UPID  |
| -----------               | ------- | ----- |
| Motion Cmd Slave Header   | UInt16  |       |
| Motion Cmd Par Byte 4..7  | UInt32  |       |