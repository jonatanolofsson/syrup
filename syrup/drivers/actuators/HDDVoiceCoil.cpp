#include <syrup/drivers/actuators/HDDVoiceCoil.hpp>
#include <syrup/utils/utils.hpp>
#include <syrup/utils/Controller.hpp>
#include <wirish/wirish.h>


#define POWER_PRESCALER     1
#define DUTY_PRESCALER      10


#define POWER0          32768
#define DUTY0           32768

#define POWER_SCALE     1
#define DUTY_SCALE      1

#define STEPLEN         500
#define STEPSIZE        1024

namespace syrup {
    namespace drivers {
        namespace actuators {
            HDDVoiceCoil::HDDVoiceCoil(const int pwr_pin_, const int duty_pin_)
            :
                power0(POWER0),
                center(DUTY0),
                pwr_pin(pwr_pin_),
                duty_pin(duty_pin_)
            {
                timer_dev *pwrT = PIN_MAP[pwr_pin].timer_device;
                timer_dev *dtyT = PIN_MAP[duty_pin].timer_device;

                pinMode(pwr_pin, PWM);
                timer_set_prescaler(pwrT, POWER_PRESCALER);
                pwrT->regs.gen->CR1 |= TIMER_CR1_CKD_CMS_CENTER3;

                pinMode(duty_pin, PWM);
                timer_set_prescaler(dtyT, DUTY_PRESCALER);

                timer_generate_update(pwrT);
                timer_generate_update(dtyT);
            }

            void HDDVoiceCoil::setControlOutput(const int u) {
                int d = center + DUTY_SCALE * u;
                Serial1.print("Output: "); Serial1.println(d);
                pwmWrite(duty_pin,  d);
                pwmWrite(pwr_pin,   power0 + POWER_SCALE * abs(u));
            }

            uint16_t stepdata[STEPLEN];

            void* initiateStep(HddControllerType* const that) {
                Serial1.println("Stepping... ");
                that->sensor->clear();
                DELAY(1);
                taskENTER_CRITICAL();
                that->stopController();
                pwmWrite(that->actuator->duty_pin, that->settings.zero + STEPSIZE);
                for(int i = 0; i < STEPLEN; ++i) {
                    that->sensor->measure(&stepdata[i]);
                    DELAY(1);
                    //~ for(int w = 0; w < 720; ++w){};
                }
                taskEXIT_CRITICAL();
                for(int i = 0; i < STEPLEN; ++i) {
                    Serial1.println(stepdata[i]);
                }

                return (void*) NULL;
            }
            void* center(HddControllerType* const that) {
                Serial1.print("Center... ");
                that->controller.setReference(0);
                that->startController();
                DELAY(100000);
                that->actuator->center = that->controller.ei;
                return (void*)initiateStep;
            }
            void* identifyHigh(HddControllerType* const that) {
                Serial1.print("Identify high... ");
                pwmWrite(that->actuator->duty_pin, 0xffff);

                DELAY(500);

                that->sensor->average(&that->settings.high, 20, 20);
                Serial1.println(that->settings.high);
                that->settings.zero = (that->settings.high + that->settings.low) / 2;
                return (void*)center;
            }
            void* identifyLow(HddControllerType* const that) {
                Serial1.print("Identify low... ");
                pwmWrite(that->actuator->duty_pin, 0x00);

                DELAY(500);

                that->sensor->average(&(that->settings.low), 20, 20);
                Serial1.print(that->settings.low);
                return (void*)identifyHigh;
            }
            void* HDDidentification(HddControllerType* const that) {
                Serial1.println("Identification started... ");
                pwmWrite(that->actuator->pwr_pin, 16384);
                return (void*)identifyLow;
            }
        }
    }
}
