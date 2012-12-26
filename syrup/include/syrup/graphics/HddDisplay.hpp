#ifndef SYRUP_GRAPHICS_HDDDISPLAY
#define SYRUP_GRAPHICS_HDDDISPLAY

#include <stdint.h>
#include <syrup/graphics/Drawer.hpp>
#include <syrup/drivers/actuators/HDDVoiceCoil.hpp>
#include <syrup/drivers/actuators/Laser.hpp>

namespace syrup {
    class HddDisplay : public Drawer {
        private:
            typedef uint8_t Scalar;
            HddControllerType* x;
            HddControllerType* y;
            Laser* laser;

        public:
            HddDisplay(HddControllerType* x_, HddControllerType* y_, Laser* l_)
             :  x(x_),
                y(y_),
                laser(l_)
            {}

            void line(const Scalar x1, const Scalar y1, const Scalar x2, const Scalar y2);
    };
}

#endif
