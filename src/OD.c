/*******************************************************************************
    CANopen Object Dictionary definition for CANopenNode V4

    This file was automatically generated by CANopenEditor v4.0-51-g2d9b1ad

    https://github.com/CANopenNode/CANopenNode
    https://github.com/CANopenNode/CANopenEditor

    DON'T EDIT THIS FILE MANUALLY, UNLESS YOU KNOW WHAT YOU ARE DOING !!!!
*******************************************************************************/

#define OD_DEFINITION
#include "301/CO_ODinterface.h"
#include "OD.h"

#if CO_VERSION_MAJOR < 4
#error This Object dictionary is compatible with CANopenNode V4.0 and above!
#endif

/*******************************************************************************
    OD data initialization of all groups
*******************************************************************************/
OD_ATTR_PERSIST_COMM OD_PERSIST_COMM_t OD_PERSIST_COMM = {
    .x1000_deviceType = 0x000F0191,
    .x1005_COB_ID_SYNCMessage = 0x00000080,
    .x1006_communicationCyclePeriod = 0x00000000,
    .x1007_synchronousWindowLength = 0x00000000,
    .x1012_COB_IDTimeStampObject = 0x00000100,
    .x1014_COB_ID_EMCY = 0x00000080,
    .x1015_inhibitTimeEMCY = 0x0000,
    .x1016_consumerHeartbeatTime_sub0 = 0x08,
    .x1016_consumerHeartbeatTime = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
    .x1017_producerHeartbeatTime = 0x0064,
    .x1018_identity = {
        .highestSub_indexSupported = 0x04,
        .vendor_ID = 0x00000000,
        .productCode = 0x00000000,
        .revisionNumber = 0x00000000,
        .serialNumber = 0x00000000
    },
    .x1019_synchronousCounterOverflowValue = 0x00,
    .x1280_SDOClientParameter = {
        .highestSub_indexSupported = 0x03,
        .COB_IDClientToServerTx = 0x80000000,
        .COB_IDServerToClientRx = 0x80000000,
        .node_IDOfTheSDOServer = 0x01
    },
    .x1400_RPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByRPDO = 0x000001BF,
        .transmissionType = 0xFF
    },
    .x1401_RPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByRPDO = 0x000002BF,
        .transmissionType = 0xFF
    },
    .x1402_RPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByRPDO = 0x000003BF,
        .transmissionType = 0xFF
    },
    .x1600_RPDOMappingParameter = {
        .numberOfMappedObjects = 0x04,
        .mappedObject_1 = 0x21100110,
        .mappedObject_2 = 0x21100210,
        .mappedObject_3 = 0x21100310,
        .mappedObject_4 = 0x21100410
    },
    .x1601_RPDOMappingParameter = {
        .numberOfMappedObjects = 0x02,
        .mappedObject_1 = 0x21150120,
        .mappedObject_2 = 0x21150220
    },
    .x1602_RPDOMappingParameter = {
        .numberOfMappedObjects = 0x04,
        .mappedObject_1 = 0x21100510,
        .mappedObject_2 = 0x21140110,
        .mappedObject_3 = 0x21100710,
        .mappedObject_4 = 0x21100610
    },
    .x1800_TPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByTPDO = 0x0000023F,
        .transmissionType = 0xFF,
        .inhibitTime = 0x0000,
        .compatibilityEntry = 0x00,
        .eventTimer = 0x0000,
        .SYNCStartValue = 0x00
    },
    .x1801_TPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByTPDO = 0x0000033F,
        .transmissionType = 0xFF,
        .inhibitTime = 0x0000,
        .compatibilityEntry = 0x00,
        .eventTimer = 0x0000,
        .SYNCStartValue = 0x00
    },
    .x1802_TPDOCommunicationParameter = {
        .maxSub_index = 0x06,
        .COB_IDUsedByTPDO = 0x0000043F,
        .transmissionType = 0xFF,
        .inhibitTime = 0x0000,
        .compatibilityEntry = 0x00,
        .eventTimer = 0x0000,
        .SYNCStartValue = 0x00
    },
    .x1A00_TPDOMappingParameter = {
        .numberOfMappedObjects = 0x01,
        .mappedObject_1 = 0x21110010
    },
    .x1A01_TPDOMappingParameter = {
        .numberOfMappedObjects = 0x07,
        .mappedObject_1 = 0x21120010,
        .mappedObject_2 = 0x21130108,
        .mappedObject_3 = 0x21130208,
        .mappedObject_4 = 0x21130308,
        .mappedObject_5 = 0x21130608,
        .mappedObject_6 = 0x21130408,
        .mappedObject_7 = 0x21130508
    },
    .x1A02_TPDOMappingParameter = {
        .numberOfMappedObjects = 0x03,
        .mappedObject_1 = 0x21120010,
        .mappedObject_2 = 0x21130708,
        .mappedObject_3 = 0x21130808
    }
};

OD_ATTR_RAM OD_RAM_t OD_RAM = {
    .x1001_errorRegister = 0x00,
    .x1003_pre_definedErrorField_sub0 = 0x00,
    .x1003_pre_definedErrorField = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
    .x1010_storeParameters_sub0 = 0x06,
    .x1010_storeParameters = {0x00000003, 0x00000001, 0x00000001, 0x00000003, 0x00000001, 0x00000003},
    .x1011_restoreDefaultParameters_sub0 = 0x06,
    .x1011_restoreDefaultParameters = {0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001},
    .x1200_SDOServerParameter = {
        .highestSub_indexSupported = 0x02,
        .COB_IDClientToServerRx = 0x00000600,
        .COB_IDServerToClientTx = 0x00000580
    },
    .x2100_errorStatusBits = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .x2105_version = {
        .highestSub_indexSupported = 0x02
    },
    .x2110_linMotStatusUInt16_sub0 = 0x07,
    .x2110_linMotStatusUInt16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    .x2111_linMotControlWord = 0x0000,
    .x2112_linMotCMD_Header = 0x0000,
    .x2113_linMotCMD_Parameters_sub0 = 0x08,
    .x2113_linMotCMD_Parameters = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .x2114_linMotStatusSInt16_sub0 = 0x01,
    .x2114_linMotStatusSInt16 = {0},
    .x2115_linMotStatusSInt32_sub0 = 0x03,
    .x2115_linMotStatusSInt32 = {0, 0, 0}
};

OD_ATTR_PERSIST_TEST_AUTO OD_PERSIST_TEST_AUTO_t OD_PERSIST_TEST_AUTO = {
    .x2106_power_onCounter = 0x00000000
};



/*******************************************************************************
    All OD objects (constant definitions)
*******************************************************************************/
typedef struct {
    OD_obj_var_t o_1000_deviceType;
    OD_obj_var_t o_1001_errorRegister;
    OD_obj_array_t o_1003_pre_definedErrorField;
    OD_obj_var_t o_1005_COB_ID_SYNCMessage;
    OD_obj_var_t o_1006_communicationCyclePeriod;
    OD_obj_var_t o_1007_synchronousWindowLength;
    OD_obj_array_t o_1010_storeParameters;
    OD_obj_array_t o_1011_restoreDefaultParameters;
    OD_obj_var_t o_1012_COB_IDTimeStampObject;
    OD_obj_var_t o_1014_COB_ID_EMCY;
    OD_obj_var_t o_1015_inhibitTimeEMCY;
    OD_obj_array_t o_1016_consumerHeartbeatTime;
    OD_obj_var_t o_1017_producerHeartbeatTime;
    OD_obj_record_t o_1018_identity[5];
    OD_obj_var_t o_1019_synchronousCounterOverflowValue;
    OD_obj_record_t o_1200_SDOServerParameter[3];
    OD_obj_record_t o_1280_SDOClientParameter[4];
    OD_obj_record_t o_1400_RPDOCommunicationParameter[3];
    OD_obj_record_t o_1401_RPDOCommunicationParameter[3];
    OD_obj_record_t o_1402_RPDOCommunicationParameter[3];
    OD_obj_record_t o_1600_RPDOMappingParameter[5];
    OD_obj_record_t o_1601_RPDOMappingParameter[3];
    OD_obj_record_t o_1602_RPDOMappingParameter[5];
    OD_obj_record_t o_1800_TPDOCommunicationParameter[7];
    OD_obj_record_t o_1801_TPDOCommunicationParameter[7];
    OD_obj_record_t o_1802_TPDOCommunicationParameter[7];
    OD_obj_record_t o_1A00_TPDOMappingParameter[2];
    OD_obj_record_t o_1A01_TPDOMappingParameter[8];
    OD_obj_record_t o_1A02_TPDOMappingParameter[4];
    OD_obj_var_t o_2100_errorStatusBits;
    OD_obj_record_t o_2105_version[3];
    OD_obj_var_t o_2106_power_onCounter;
    OD_obj_array_t o_2110_linMotStatusUInt16;
    OD_obj_var_t o_2111_linMotControlWord;
    OD_obj_var_t o_2112_linMotCMD_Header;
    OD_obj_array_t o_2113_linMotCMD_Parameters;
    OD_obj_array_t o_2114_linMotStatusSInt16;
    OD_obj_array_t o_2115_linMotStatusSInt32;
} ODObjs_t;

static CO_PROGMEM ODObjs_t ODObjs = {
    .o_1000_deviceType = {
        .dataOrig = &OD_PERSIST_COMM.x1000_deviceType,
        .attribute = ODA_SDO_R | ODA_MB,
        .dataLength = 4
    },
    .o_1001_errorRegister = {
        .dataOrig = &OD_RAM.x1001_errorRegister,
        .attribute = ODA_SDO_R | ODA_TRPDO,
        .dataLength = 1
    },
    .o_1003_pre_definedErrorField = {
        .dataOrig0 = &OD_RAM.x1003_pre_definedErrorField_sub0,
        .dataOrig = &OD_RAM.x1003_pre_definedErrorField[0],
        .attribute0 = ODA_SDO_RW,
        .attribute = ODA_SDO_R | ODA_MB,
        .dataElementLength = 4,
        .dataElementSizeof = sizeof(uint32_t)
    },
    .o_1005_COB_ID_SYNCMessage = {
        .dataOrig = &OD_PERSIST_COMM.x1005_COB_ID_SYNCMessage,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 4
    },
    .o_1006_communicationCyclePeriod = {
        .dataOrig = &OD_PERSIST_COMM.x1006_communicationCyclePeriod,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 4
    },
    .o_1007_synchronousWindowLength = {
        .dataOrig = &OD_PERSIST_COMM.x1007_synchronousWindowLength,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 4
    },
    .o_1010_storeParameters = {
        .dataOrig0 = &OD_RAM.x1010_storeParameters_sub0,
        .dataOrig = &OD_RAM.x1010_storeParameters[0],
        .attribute0 = ODA_SDO_R,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataElementLength = 4,
        .dataElementSizeof = sizeof(uint32_t)
    },
    .o_1011_restoreDefaultParameters = {
        .dataOrig0 = &OD_RAM.x1011_restoreDefaultParameters_sub0,
        .dataOrig = &OD_RAM.x1011_restoreDefaultParameters[0],
        .attribute0 = ODA_SDO_R,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataElementLength = 4,
        .dataElementSizeof = sizeof(uint32_t)
    },
    .o_1012_COB_IDTimeStampObject = {
        .dataOrig = &OD_PERSIST_COMM.x1012_COB_IDTimeStampObject,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 4
    },
    .o_1014_COB_ID_EMCY = {
        .dataOrig = &OD_PERSIST_COMM.x1014_COB_ID_EMCY,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 4
    },
    .o_1015_inhibitTimeEMCY = {
        .dataOrig = &OD_PERSIST_COMM.x1015_inhibitTimeEMCY,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 2
    },
    .o_1016_consumerHeartbeatTime = {
        .dataOrig0 = &OD_PERSIST_COMM.x1016_consumerHeartbeatTime_sub0,
        .dataOrig = &OD_PERSIST_COMM.x1016_consumerHeartbeatTime[0],
        .attribute0 = ODA_SDO_R,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataElementLength = 4,
        .dataElementSizeof = sizeof(uint32_t)
    },
    .o_1017_producerHeartbeatTime = {
        .dataOrig = &OD_PERSIST_COMM.x1017_producerHeartbeatTime,
        .attribute = ODA_SDO_RW | ODA_MB,
        .dataLength = 2
    },
    .o_1018_identity = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1018_identity.highestSub_indexSupported,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1018_identity.vendor_ID,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1018_identity.productCode,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1018_identity.revisionNumber,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1018_identity.serialNumber,
            .subIndex = 4,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1019_synchronousCounterOverflowValue = {
        .dataOrig = &OD_PERSIST_COMM.x1019_synchronousCounterOverflowValue,
        .attribute = ODA_SDO_RW,
        .dataLength = 1
    },
    .o_1200_SDOServerParameter = {
        {
            .dataOrig = &OD_RAM.x1200_SDOServerParameter.highestSub_indexSupported,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_RAM.x1200_SDOServerParameter.COB_IDClientToServerRx,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_RAM.x1200_SDOServerParameter.COB_IDServerToClientTx,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1280_SDOClientParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1280_SDOClientParameter.highestSub_indexSupported,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1280_SDOClientParameter.COB_IDClientToServerTx,
            .subIndex = 1,
            .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1280_SDOClientParameter.COB_IDServerToClientRx,
            .subIndex = 2,
            .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1280_SDOClientParameter.node_IDOfTheSDOServer,
            .subIndex = 3,
            .attribute = ODA_SDO_RW,
            .dataLength = 1
        }
    },
    .o_1400_RPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1400_RPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1400_RPDOCommunicationParameter.COB_IDUsedByRPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1400_RPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1401_RPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1401_RPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1401_RPDOCommunicationParameter.COB_IDUsedByRPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1401_RPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1402_RPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1402_RPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1402_RPDOCommunicationParameter.COB_IDUsedByRPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1402_RPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1600_RPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1600_RPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1600_RPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1600_RPDOMappingParameter.mappedObject_2,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1600_RPDOMappingParameter.mappedObject_3,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1600_RPDOMappingParameter.mappedObject_4,
            .subIndex = 4,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1601_RPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1601_RPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1601_RPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1601_RPDOMappingParameter.mappedObject_2,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1602_RPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1602_RPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1602_RPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1602_RPDOMappingParameter.mappedObject_2,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1602_RPDOMappingParameter.mappedObject_3,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1602_RPDOMappingParameter.mappedObject_4,
            .subIndex = 4,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1800_TPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.COB_IDUsedByTPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.inhibitTime,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.compatibilityEntry,
            .subIndex = 4,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.eventTimer,
            .subIndex = 5,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1800_TPDOCommunicationParameter.SYNCStartValue,
            .subIndex = 6,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1801_TPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.COB_IDUsedByTPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.inhibitTime,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.compatibilityEntry,
            .subIndex = 4,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.eventTimer,
            .subIndex = 5,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1801_TPDOCommunicationParameter.SYNCStartValue,
            .subIndex = 6,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1802_TPDOCommunicationParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.maxSub_index,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.COB_IDUsedByTPDO,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.transmissionType,
            .subIndex = 2,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.inhibitTime,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.compatibilityEntry,
            .subIndex = 4,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.eventTimer,
            .subIndex = 5,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 2
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1802_TPDOCommunicationParameter.SYNCStartValue,
            .subIndex = 6,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        }
    },
    .o_1A00_TPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1A00_TPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A00_TPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1A01_TPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_2,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_3,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_4,
            .subIndex = 4,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_5,
            .subIndex = 5,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_6,
            .subIndex = 6,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A01_TPDOMappingParameter.mappedObject_7,
            .subIndex = 7,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_1A02_TPDOMappingParameter = {
        {
            .dataOrig = &OD_PERSIST_COMM.x1A02_TPDOMappingParameter.numberOfMappedObjects,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A02_TPDOMappingParameter.mappedObject_1,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A02_TPDOMappingParameter.mappedObject_2,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        },
        {
            .dataOrig = &OD_PERSIST_COMM.x1A02_TPDOMappingParameter.mappedObject_3,
            .subIndex = 3,
            .attribute = ODA_SDO_R | ODA_MB,
            .dataLength = 4
        }
    },
    .o_2100_errorStatusBits = {
        .dataOrig = &OD_RAM.x2100_errorStatusBits[0],
        .attribute = ODA_SDO_R | ODA_TPDO,
        .dataLength = 10
    },
    .o_2105_version = {
        {
            .dataOrig = &OD_RAM.x2105_version.highestSub_indexSupported,
            .subIndex = 0,
            .attribute = ODA_SDO_R,
            .dataLength = 1
        },
        {
            .dataOrig = NULL,
            .subIndex = 1,
            .attribute = ODA_SDO_R | ODA_STR,
            .dataLength = 0
        },
        {
            .dataOrig = NULL,
            .subIndex = 2,
            .attribute = ODA_SDO_R | ODA_STR,
            .dataLength = 0
        }
    },
    .o_2106_power_onCounter = {
        .dataOrig = &OD_PERSIST_TEST_AUTO.x2106_power_onCounter,
        .attribute = ODA_SDO_R | ODA_MB,
        .dataLength = 4
    },
    .o_2110_linMotStatusUInt16 = {
        .dataOrig0 = &OD_RAM.x2110_linMotStatusUInt16_sub0,
        .dataOrig = &OD_RAM.x2110_linMotStatusUInt16[0],
        .attribute0 = ODA_SDO_R | ODA_TRPDO,
        .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
        .dataElementLength = 2,
        .dataElementSizeof = sizeof(uint16_t)
    },
    .o_2111_linMotControlWord = {
        .dataOrig = &OD_RAM.x2111_linMotControlWord,
        .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
        .dataLength = 2
    },
    .o_2112_linMotCMD_Header = {
        .dataOrig = &OD_RAM.x2112_linMotCMD_Header,
        .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
        .dataLength = 2
    },
    .o_2113_linMotCMD_Parameters = {
        .dataOrig0 = &OD_RAM.x2113_linMotCMD_Parameters_sub0,
        .dataOrig = &OD_RAM.x2113_linMotCMD_Parameters[0],
        .attribute0 = ODA_SDO_R | ODA_TPDO,
        .attribute = ODA_SDO_R | ODA_TPDO,
        .dataElementLength = 1,
        .dataElementSizeof = sizeof(uint8_t)
    },
    .o_2114_linMotStatusSInt16 = {
        .dataOrig0 = &OD_RAM.x2114_linMotStatusSInt16_sub0,
        .dataOrig = &OD_RAM.x2114_linMotStatusSInt16[0],
        .attribute0 = ODA_SDO_R | ODA_TRPDO,
        .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
        .dataElementLength = 2,
        .dataElementSizeof = sizeof(int16_t)
    },
    .o_2115_linMotStatusSInt32 = {
        .dataOrig0 = &OD_RAM.x2115_linMotStatusSInt32_sub0,
        .dataOrig = &OD_RAM.x2115_linMotStatusSInt32[0],
        .attribute0 = ODA_SDO_R | ODA_TRPDO,
        .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
        .dataElementLength = 4,
        .dataElementSizeof = sizeof(int32_t)
    }
};


/*******************************************************************************
    Object dictionary
*******************************************************************************/
static OD_ATTR_OD OD_entry_t ODList[] = {
    {0x1000, 0x01, ODT_VAR, &ODObjs.o_1000_deviceType, NULL},
    {0x1001, 0x01, ODT_VAR, &ODObjs.o_1001_errorRegister, NULL},
    {0x1003, 0x09, ODT_ARR, &ODObjs.o_1003_pre_definedErrorField, NULL},
    {0x1005, 0x01, ODT_VAR, &ODObjs.o_1005_COB_ID_SYNCMessage, NULL},
    {0x1006, 0x01, ODT_VAR, &ODObjs.o_1006_communicationCyclePeriod, NULL},
    {0x1007, 0x01, ODT_VAR, &ODObjs.o_1007_synchronousWindowLength, NULL},
    {0x1010, 0x07, ODT_ARR, &ODObjs.o_1010_storeParameters, NULL},
    {0x1011, 0x07, ODT_ARR, &ODObjs.o_1011_restoreDefaultParameters, NULL},
    {0x1012, 0x01, ODT_VAR, &ODObjs.o_1012_COB_IDTimeStampObject, NULL},
    {0x1014, 0x01, ODT_VAR, &ODObjs.o_1014_COB_ID_EMCY, NULL},
    {0x1015, 0x01, ODT_VAR, &ODObjs.o_1015_inhibitTimeEMCY, NULL},
    {0x1016, 0x09, ODT_ARR, &ODObjs.o_1016_consumerHeartbeatTime, NULL},
    {0x1017, 0x01, ODT_VAR, &ODObjs.o_1017_producerHeartbeatTime, NULL},
    {0x1018, 0x05, ODT_REC, &ODObjs.o_1018_identity, NULL},
    {0x1019, 0x01, ODT_VAR, &ODObjs.o_1019_synchronousCounterOverflowValue, NULL},
    {0x1200, 0x03, ODT_REC, &ODObjs.o_1200_SDOServerParameter, NULL},
    {0x1280, 0x04, ODT_REC, &ODObjs.o_1280_SDOClientParameter, NULL},
    {0x1400, 0x03, ODT_REC, &ODObjs.o_1400_RPDOCommunicationParameter, NULL},
    {0x1401, 0x03, ODT_REC, &ODObjs.o_1401_RPDOCommunicationParameter, NULL},
    {0x1402, 0x03, ODT_REC, &ODObjs.o_1402_RPDOCommunicationParameter, NULL},
    {0x1600, 0x05, ODT_REC, &ODObjs.o_1600_RPDOMappingParameter, NULL},
    {0x1601, 0x03, ODT_REC, &ODObjs.o_1601_RPDOMappingParameter, NULL},
    {0x1602, 0x05, ODT_REC, &ODObjs.o_1602_RPDOMappingParameter, NULL},
    {0x1800, 0x07, ODT_REC, &ODObjs.o_1800_TPDOCommunicationParameter, NULL},
    {0x1801, 0x07, ODT_REC, &ODObjs.o_1801_TPDOCommunicationParameter, NULL},
    {0x1802, 0x07, ODT_REC, &ODObjs.o_1802_TPDOCommunicationParameter, NULL},
    {0x1A00, 0x02, ODT_REC, &ODObjs.o_1A00_TPDOMappingParameter, NULL},
    {0x1A01, 0x08, ODT_REC, &ODObjs.o_1A01_TPDOMappingParameter, NULL},
    {0x1A02, 0x04, ODT_REC, &ODObjs.o_1A02_TPDOMappingParameter, NULL},
    {0x2100, 0x01, ODT_VAR, &ODObjs.o_2100_errorStatusBits, NULL},
    {0x2105, 0x03, ODT_REC, &ODObjs.o_2105_version, NULL},
    {0x2106, 0x01, ODT_VAR, &ODObjs.o_2106_power_onCounter, NULL},
    {0x2110, 0x08, ODT_ARR, &ODObjs.o_2110_linMotStatusUInt16, NULL},
    {0x2111, 0x01, ODT_VAR, &ODObjs.o_2111_linMotControlWord, NULL},
    {0x2112, 0x01, ODT_VAR, &ODObjs.o_2112_linMotCMD_Header, NULL},
    {0x2113, 0x09, ODT_ARR, &ODObjs.o_2113_linMotCMD_Parameters, NULL},
    {0x2114, 0x02, ODT_ARR, &ODObjs.o_2114_linMotStatusSInt16, NULL},
    {0x2115, 0x04, ODT_ARR, &ODObjs.o_2115_linMotStatusSInt32, NULL},
    {0x0000, 0x00, 0, NULL, NULL}
};

static OD_t _OD = {
    (sizeof(ODList) / sizeof(ODList[0])) - 1,
    &ODList[0]
};

OD_t *OD = &_OD;
