#include <syrup/drivers/actuators/HDDVoiceCoil.hpp>
#include <syrup/utils/utils.hpp>
#include <syrup/utils/Controller.hpp>
#include <wirish/wirish.h>

#define DUTY_PRESCALER      1
#define DUTY_PERIOD         2048

#define STEPLEN         300
#define STEPVALUE       1700

#define SQUARE_LOW      -200
#define SQUARE_STEP     400

namespace syrup {
    namespace drivers {
        namespace actuators {
            HDDVoiceCoil::HDDVoiceCoil(const int pwr_pin_, const int duty_pin_)
            :
                center(DUTY_PERIOD/2),
                pwr_pin(pwr_pin_),
                duty_pin(duty_pin_)
            {
                pinMode(pwr_pin, OUTPUT);
                pinMode(duty_pin, PWM);
                timer_dev *dtyT = PIN_MAP[duty_pin].timer_device;
                timer_set_reload(dtyT, DUTY_PERIOD);
                timer_set_prescaler(dtyT, DUTY_PRESCALER);

                timer_generate_update(dtyT);
            }

            void HDDVoiceCoil::setControlOutput(const int u) {
                //~ Serial1.print("u = "); Serial1.println(u + center);

                pwmWrite(duty_pin,  u + center);
            }

            void* squareReference(HddControllerType* const that) {
                that->startController();
                bool longline = false;
                bool high = false;
                for(;;) {
                    for(int i = 0; i < 100; ++i) {
                        that->controller.setReference((SQUARE_LOW + high * SQUARE_STEP) * (1 + longline));
                        high = !high;
                        DELAY(10);
                    }
                    longline = !longline;
                }
                return (void*)NULL;
            }


            uint16_t stepdata[STEPLEN];
            void* initiateStep(HddControllerType* const that) {
                that->stopController();
                pwmWrite(that->actuator->duty_pin, 0);
                DELAY(500);
                that->sensor->clear();
                DELAY(1);
                that->sensor->record(stepdata, STEPLEN);
                pwmWrite(that->actuator->duty_pin, STEPVALUE);
                for(int i = 0; i < STEPLEN; ++i) {
                    delayMicroseconds(100);
                    that->sensor->measure(&stepdata[i]);
                }

                Serial1.print("data = [");
                for(int i = 0; i < STEPLEN; ++i) {
                    Serial1.println(stepdata[i]);
                }
                Serial1.println("];");
                Serial1.println("plot(scale * (data - zero));");

                return (void*) NULL;
            }
            void* center(HddControllerType* const that) {
                Serial1.print("Center... ");
                that->controller.setReference(0);
                that->sensor->clear();
                DELAY(1);
                that->startController();
                DELAY(100000);
                //~ that->actuator->center += that->controller.ei;
                //~ Serial1.println(that->actuator->center);
                return (void*)NULL;
            }
            void* identifyLow(HddControllerType* const that) {
                Serial1.print("low = ");
                pwmWrite(that->actuator->duty_pin, 0x00);

                DELAY(500);

                that->sensor->average(&(that->settings.low), 20, 20);
                Serial1.print(that->settings.low);
                Serial1.println(";");

                that->settings.zero = (that->settings.high + that->settings.low) / 2;
                Serial1.print("zero = ");
                Serial1.print(that->settings.zero);
                Serial1.println(";");
                that->settings.scale = sign(that->settings.high - that->settings.low);
                Serial1.print("scale = ");
                Serial1.print(that->settings.scale);
                Serial1.println(";");
                return (void*)squareReference;
            }
            void* identifyHigh(HddControllerType* const that) {
                Serial1.print("high = ");
                pwmWrite(that->actuator->duty_pin, 0xffff);

                DELAY(500);

                that->sensor->average(&that->settings.high, 20, 20);
                Serial1.print(that->settings.high);
                Serial1.println(";");
                return (void*)identifyLow;
            }
            void* HDDidentification(HddControllerType* const that) {
                Serial1.println("Identification started... ");
                Serial1.println("clear;figure(1);clf;");
                digitalWrite(that->actuator->pwr_pin, HIGH);
                //~ that->controller.lambda(200, 700, 1, 50);
                return (void*)identifyHigh;
            }
        }
    }
}
