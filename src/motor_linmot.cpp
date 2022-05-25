#include "motor_linmot.hpp"

void LinmotMotor::goToPos(float position, float speed, float acceleration) {
  this->CO_sendCmd(
    0x0900, 
    static_cast<uint16_t>(position * 10), 
    static_cast<uint16_t>(speed), 
    static_cast<uint16_t>(acceleration), 
    static_cast<uint16_t>(acceleration)
  );
}

void LinmotMotor::stopMotion() {
  
}