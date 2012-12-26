#ifndef SYRUP_BitDisplay_HPP
#define SYRUP_BitDisplay_HPP

namespace syrup {
    class Framebuffer;
}

namespace syrup {
    class BitDisplay {
        public:
            virtual uint8_t width() = 0;
            virtual uint8_t height() = 0;
            virtual void init() = 0;
            virtual void flush(Framebuffer*) = 0;
    };
}
#endif
