#ifndef MOTOR_VIRTUAL_H
#define MOTOR_VIRTUAL_H

#include <time.h>

#include "CANopen.h"
#include "motor.hpp"

class VirtualMotor: public MotorInterface {
  public:
    VirtualMotor();
    // Motion
    void goToHome() { this->state = MotorState::HOMING; this->removeStatusFlag(MOTOR_FLAG_HOMED); }
    void stopMotion();

    // General
    void registerTasks();

    // Tasks
    void task_motion();

  protected:
    void unsafeGoToPos(float position, float speed, float acceleration);

  private:
    float position = 0;
    float velocity = 0;
    float acceleration = 0;

    float targetPosition = 0;
    float targetVelocity = 0;
    float targetAcceleration = 0;
};

#endif