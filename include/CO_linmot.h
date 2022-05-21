#ifndef CO_LINMOT_H
#define CO_LINMOT_H

#include "CANopen.h"
#include "301/CO_config.h"
#include "OD.h"

// LinMot CMD
extern uint8_t *OD_variable_linMotCMDParameters_flagsPDO;

// LinMot Control Word
extern uint8_t *OD_variable_linMotControlWord_flagsPDO;

// Linmot Heartbeat via RPDO
extern volatile uint32_t linmotPDOCounter;

void linmot_co_init();

#endif