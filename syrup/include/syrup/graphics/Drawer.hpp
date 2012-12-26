#ifndef SYRUP_GRAPHICS_DRAWER_HPP_
#define SYRUP_GRAPHICS_DRAWER_HPP_

namespace syrup {
    class Drawer {
        public:
            typedef uint8_t Scalar;
            virtual void line(const Scalar x1, const Scalar y1, const Scalar x2, const Scalar y2) = 0;
            virtual void clear() {};
            virtual void flush() {};
    };
}

#endif
