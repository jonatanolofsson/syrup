#ifndef SYRUP_UTILS_CONTROLLER_HPP_
#define SYRUP_UTILS_CONTROLLER_HPP_

#include <syrup/drivers/sensors/Sensor.hpp>
#include <syrup/drivers/actuators/Actuator.hpp>
#include <syrup/utils/StateMachine.hpp>
#include <wirish/wirish.h>

#define STACK_SIZE      512

namespace syrup {
    namespace utils {
        using drivers::sensors::Sensor;
        using drivers::actuators::Actuator;
        template<typename ControllerType, typename ST = Sensor, typename AT = Actuator>
        class Controller {
            public:
                typedef Controller<ControllerType, ST, AT> MyType;
                typedef void*(*stateMachineFunction)(MyType* const);
                stateMachineFunction currentState;

                ST* sensor;
                AT* actuator;
                bool active;
                bool running;
                ControllerType controller;

                struct settings_s {
                    uint16_t low;
                    uint16_t high;
                    uint16_t zero;
                } settings;

                Controller(ST* sensor_, AT* actuator_, stateMachineFunction initialState_)
                :
                    currentState(initialState_),
                    sensor(sensor_),
                    actuator(actuator_),
                    active(false),
                    running(true),
                    controller()
                {
                    settings = (settings_s){0,0,0};
                    xTaskCreate(utils::classThreadInitializer<MyType>,
                                (signed portCHAR *)"ControllerStateMachineTask",
                                STACK_SIZE,
                                this,
                                tskIDLE_PRIORITY + 2,
                                NULL);
                    xTaskCreate(utils::classThreadInitializer2<MyType>,
                                (signed portCHAR *)"ControllerTask",
                                STACK_SIZE,
                                this,
                                tskIDLE_PRIORITY + 2,
                                NULL);
                }

                void startController() {
                    active = true;
                }

                void stopController() {
                    active = true;
                }

                void tick() {
                    if(currentState) {
                        currentState = (stateMachineFunction)currentState(this);
                    } else {
                        taskYIELD();
                    }
                }
                void run() {
                    Serial1.println("Voicecoil statemachine task: started");
                    while(running) {
                        tick();
                    }
                }
                void run2() {
                    Sensor::measurement_t y;
                    Serial1.println("Voicecoil controller task: started");
                    while(running) {
                        if(active) {
                            sensor->measure(&y);
                            actuator->setControlOutput(controller(y-settings.zero));
                        } else {
                            taskYIELD();
                        }
                    }
                }
        };
    }
}

#endif
