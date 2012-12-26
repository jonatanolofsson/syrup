#ifndef SYRUP_DRIVERS_LASER_HPP_
#define SYRUP_DRIVERS_LASER_HPP_

#include <stdint.h>

namespace syrup {
    class Laser {
        private:
            uint8_t pin;
        public:
            Laser(uint8_t p_) : pin(p_) {}
            void on() { digitalWrite(pin, HIGH); }
            void off() { digitalWrite(pin, LOW); }
    };
}
#endif
