#ifndef SYRUP_UTILS_CONTROLLER_HPP_
#define SYRUP_UTILS_CONTROLLER_HPP_

#include <syrup/drivers/sensors/Sensor.hpp>
#include <syrup/drivers/actuators/Actuator.hpp>
#include <wirish/wirish.h>
#include <FreeRTOS/MapleFreeRTOS.h>

#define STACK_SIZE      256

namespace syrup {
    template<typename ControllerType, typename ST = Sensor, typename AT = Actuator>
    class Controller {
        public:
            typedef ST SensorType;
            typedef AT ActuatorType;
            typedef Controller<ControllerType, ST, AT> MyType;
            typedef void*(*stateMachineFunction)(MyType* const);
            stateMachineFunction currentState;

            SensorType* sensor;
            ActuatorType* actuator;
            bool active;
            bool running;
            ControllerType controller;

            struct settings_s {
                uint16_t low;
                uint16_t high;
                uint16_t zero;
                int16_t scale;
            };
            settings_s settings;

            struct areWeThereYet_s {
                bool flag;
                bool positive;
            } areWeThereYet;

            Controller(SensorType* sensor_, AT* actuator_, stateMachineFunction initialState_)
            :
                currentState(initialState_),
                sensor(sensor_),
                actuator(actuator_),
                active(false),
                running(true),
                controller()
            {
                settings = (settings_s){0,0,0,1};
                //~ xTaskCreate(utils::classThreadInitializer<MyType>,
                            //~ (signed portCHAR *)"ControllerStateMachineTask",
                            //~ STACK_SIZE,
                            //~ this,
                            //~ tskIDLE_PRIORITY + 2,
                            //~ NULL);
                //~ xTaskCreate(utils::classThreadInitializer2<MyType>,
                            //~ (signed portCHAR *)"ControllerTask",
                            //~ STACK_SIZE,
                            //~ this,
                            //~ tskIDLE_PRIORITY + 2,
                            //~ NULL);
            }

            void startController() {
                active = true;
            }

            void stopController() {
                active = false;
            }
            void waitForIt() {
                while(!areWeThereYet.flag){};
            }

            void setReference(int16_t r) {
                areWeThereYet.flag = false;
                controller.setReference(r);
                areWeThereYet.positive = (controller.r - controller.y) > 0;
            }

            void tick() {
                if(currentState) {
                    currentState = (stateMachineFunction)currentState(this);
                } else {
                    vTaskSuspend(NULL);
                }
            }

            void run() {
                Serial1.println("Voicecoil statemachine task: started");
                while(running) {
                    tick();
                }
            }

            void loop()
            {
                Sensor::measurement_t y;
                sensor->measure(&y);
                actuator->setControlOutput(controller(settings.scale * (y - settings.zero)));
                if(!areWeThereYet.flag && ((controller.e > 0) != areWeThereYet.positive)) {
                    areWeThereYet.flag = true;
                }
                // NB: DON'T USE delayMicroseconds() as it is only (somewhat) accurate if running without a scheduler (it will busy wait for C*x cycles)
                //~ delayMicroseconds(50);
                //~ Serial1.println(y);

                //~ vTaskDelay(1);
            }

            void run2()
            {
                Serial1.println("Voicecoil controller task: started");
                while(running) {
                    if(active) {
                        loop();
                        vTaskDelay(1);
                    }
                    else {
                        taskYIELD();
                    }
                }
            }
    };
}

#endif
