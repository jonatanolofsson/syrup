#ifndef SYRUP_DRIVERS_BUTTON_HPP_
#define SYRUP_DRIVERS_BUTTON_HPP_
#include <wirish/wirish.h>

namespace syrup {
    template<int PIN>
    class Button {
        public:
            Button() {
                pinMode(PIN, INPUT_PULLDOWN);
            }
            bool pressed() {
                return digitalRead(PIN);
            }
    };
}

#endif
