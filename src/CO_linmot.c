
#include "CANopen.h"
#include "301/CO_config.h"
#include "OD.h"

#include "CO_linmot.h"


// LinMot CMD
OD_extension_t OD_variable_linMotCMDParameters_extension = {
    .object = NULL,
    .read = OD_readOriginal,
    .write = OD_writeOriginal
};
uint8_t *OD_variable_linMotCMDParameters_flagsPDO = NULL;

// LinMot Control Word
OD_extension_t OD_variable_linMotControlWord_extension = {
    .object = NULL,
    .read = OD_readOriginal,
    .write = OD_writeOriginal
};
uint8_t *OD_variable_linMotControlWord_flagsPDO = NULL;

// Linmot Heartbeat via RPDO
volatile uint32_t linmotPDOCounter = 0U;
ODR_t track_status_update(
  OD_stream_t *stream, const void *buf,
  OD_size_t count, OD_size_t *countWritten
) {
  linmotPDOCounter = 0;
  return OD_writeOriginal(stream, buf, count, countWritten);
}

OD_extension_t OD_variable_linMotStatus_extension = {
    .object = NULL,
    .read = OD_readOriginal,
    .write = track_status_update
};

void linmot_co_init() {
  // Initialize LinMot CMD Parameters as TPDO Mapped Object
  //  - Must be before CO_CANopenInitPDO
  //  - Allows using OD_requestTPDO for triggering
  OD_extension_init(
    OD_ENTRY_H2113_linMotCMD_Parameters,
    &OD_variable_linMotCMDParameters_extension
  );
  OD_variable_linMotCMDParameters_flagsPDO = OD_getFlagsPDO(OD_ENTRY_H2113_linMotCMD_Parameters);

  // Initialize LinMot Control Word as TPDO Mapped Object
  OD_extension_init(
    OD_ENTRY_H2111_linMotControlWord,
    &OD_variable_linMotControlWord_extension
  );
  OD_variable_linMotControlWord_flagsPDO = OD_getFlagsPDO(OD_ENTRY_H2111_linMotControlWord);

  // Initialize LinMot Status Words as RPDO Mapped Object
  //  - Used as a pesudo-heartbeat to detect alive-ness of LinMot
  OD_extension_init(
    OD_ENTRY_H2110_linMotStatus,
    &OD_variable_linMotStatus_extension
  );
}