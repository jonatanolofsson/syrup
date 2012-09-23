#ifndef SYRUP_DRIVERS_ACTUATORS_HPP_
#define SYRUP_DRIVERS_ACTUATORS_HPP_

namespace syrup {
    namespace drivers {
        namespace actuators {
            class Actuator {
                public:
                    virtual void setControlOutput(const int u) = 0;
            };
        }
    }
}

#endif
