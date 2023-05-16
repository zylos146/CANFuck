#pragma once

#include "hsmcpp/Hsm.hpp"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "logging.hpp"
using namespace esp32m;

using ActorID_t = int32_t;

#define ACTOR_QUEUE_SIZE 64
#define ACTOR_QUEUE_WARNING_SIZE 32 
#define ACTOR_QUEUE_ERROR_SIZE 16

#define ACTOR_EVENT_SIZE sizeof (Event)

#define ACTOR_STACK_SIZE 4096

#define ACTOR_TASK_PRIORITY 1

struct Event {
  ActorID_t actorId;
  hsmcpp::EventID_t eventId;
};

class Actor :: public hsmcpp::HierarchicalStateMachine {
public:
  Actor() {
    m_queueHandle = xQueueCreateStatic(
      ACTOR_QUEUE_SIZE, ACTOR_EVENT_SIZE, 
      todo, &m_queueControlBlock
    );

    m_taskHandle = xTaskCreateStatic(
      task, this->getTaskName(), 
      ACTOR_STACK_SIZE, this, ACTOR_TASK_PRIORITY, 
      &m_taskStack, &m_taskControlBlock
    );
  }
  virtual ~Actor() = default;

  virtual const char * const getTaskName() const {
    return "Actor";
  }

  bool sendEvent(const Event& event) {
    uint spacesAvailable = uxQueueSpacesAvailable(m_queueHandle);
    if (spacesAvailable < ACTOR_QUEUE_WARNING_SIZE) {
      log_w("Actor queue is almost full. Spaces available: %d", spacesAvailable);
    } else if (spacesAvailable < ACTOR_QUEUE_ERROR_SIZE) {
      log_e("Actor queue is critically full. Spaces available: %d", spacesAvailable);
      return false;
    }

    int sendStatus = xQueueSend(m_queueHandle, &event, portMAX_DELAY);
    if (sendStatus != pdTRUE) {
      log_e("Failed to send event to actor queue. Status: %d", sendStatus);
      return false;
    }

    return true;
  }

protected:
  TaskHandle_t m_taskHandle;
  StaticTask_t m_taskControlBlock;
  StackType_t m_taskStack[ ACTOR_STACK_SIZE ];

  QueueHandle_t m_queueHandle;
  StaticQueue_t m_queueControlBlock;
  uint8_t m_queueStorage[ ACTOR_QUEUE_SIZE * ACTOR_EVENT_SIZE ];

  static void task(void * param) {
    Actor * actor = (Actor *) param;
    Event event;
    while (true) {
      xQueueReceive(m_queueHandle, &event, portMAX_DELAY);
      actor->sendEvent(event);
    }
  }
};