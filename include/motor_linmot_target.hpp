#ifndef MOTOR_LINMOT_TARGET_H
#define MOTOR_LINMOT_TARGET_H

#include "CO_main.h"
#include "motor_linmot.hpp"

MotorInterface* setupTargetMotor() {
  LinmotMotor* motor = new LinmotMotor();

  motionBounds bounds = {
    .start = 110, // mm
    .end = 0, // mm
    .keepout = 5 // mm
  };
  motor->setMaxSpeed(5000); // 5 m/s
  motor->setMaxAcceleration(25000); // 25 m/s^2
  motor->setBounds(bounds);

  motor->CO_setNodeId(NODE_ID_LINMOT);
  motor->CO_setStatus(OD_ENTRY_H2110_linMotStatusUInt16);
  motor->CO_setMonitor(OD_ENTRY_H2114_linMotStatusSInt16);
  motor->CO_setControl(OD_ENTRY_H2111_linMotControlWord);
  motor->CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
  motor->CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

  return motor;
}

#endif