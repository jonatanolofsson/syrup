#ifndef SYRUP_FRAMEBUFFER_HPP_
#define SYRUP_FRAMEBUFFER_HPP_

#include <stdint.h>
#include <syrup/math/math.hpp>
#include <syrup/drivers/display/BitDisplay.hpp>

namespace syrup {
    namespace graphics {

        class Framebuffer;
        using syrup::drivers::display::BitDisplay;

        class FBRectangle {
            public:
                typedef uint8_t Scalar;
                Scalar x1,y1,x2,y2;
                FBRectangle(const Scalar x1_, const Scalar y1_, const Scalar x2_, const Scalar y2_)     : x1(x1_),  y1(y1_),    x2(x2_),    y2(y2_) {}
                FBRectangle(const FBRectangle& r)                                                       : x1(r.x1), y1(r.y1),   x2(r.x2),   y2(r.y2) {}
                FBRectangle operator+=(const FBRectangle& b)
                {
                    x1 = min(x1, b.x1);
                    y1 = min(y1, b.y1);
                    x2 = max(x2, b.x2);
                    y2 = max(y2, b.y2);
                    return *this;
                }

                FBRectangle operator+(const FBRectangle& b)
                {
                    FBRectangle a(*this);
                    a += b;
                    return a;
                }

                void move(const Scalar x, const Scalar y) {
                    x1 += x; y1 += y;
                    x2 += x; y2 += y;
                }
        };

        /// 1 bit per pixel framebuffer
        class Framebuffer
        {
            private:
                BitDisplay* display;

            public:
                typedef uint8_t FBDimensionType;
                typedef uint8_t Scalar;
                typedef uint16_t FBLengthType;

                const FBDimensionType width;
                const FBDimensionType height;
                const FBLengthType kSize;
                FBRectangle limits;
                FBDimensionType offset_y;
                //~ FBDimensionType offset_x;

            public:
                Framebuffer(BitDisplay* display_);
                Framebuffer(BitDisplay* display_, Scalar* data_);
                Framebuffer(BitDisplay* display_, Scalar* data_, Scalar xpos, Scalar ypos, Scalar limwidth, Scalar limheight);
                Framebuffer(BitDisplay* display_, Scalar* data_, const FBRectangle& limits);
                Framebuffer(const Framebuffer&);

                Framebuffer* subBuffer(FBRectangle window);
                inline Framebuffer* subBuffer(const Scalar& x1, const Scalar& y1, const Scalar& width, const Scalar& height)
                    { return subBuffer(FBRectangle(x1,y1,x1 + width - 1, y1 + height - 1)); };

                void init();
                void flush();
                void clear();

                virtual void fillRectangle(Scalar x1, Scalar y1, Scalar x2, Scalar y2);
                virtual void clearRectangle(Scalar x1, Scalar y1, Scalar x2, Scalar y2);
                virtual void pset(Scalar x, Scalar y);
                virtual void pclr(Scalar x, Scalar y);

                /// Bresenham's line drawing algorithm
                virtual void line(Scalar x1, Scalar y1, Scalar x2, Scalar y2);

                /// copy a block into framebuffer at arbitrary non divide-by-8 y position
                virtual void blit(Scalar x0, Scalar y0, const Scalar buffer_width, const Scalar buffer_height, const Scalar* buf, Scalar buffer_stride = 0);

                /// Vertical scroll by a multiple of 8
                virtual void vscroll(int8_t offset);

                Scalar* data;

                bool adjustCoords(Scalar& x1, Scalar& y1);
                bool adjustCoords(Scalar& x1, Scalar& y1, Scalar& x2, Scalar& y2);
                bool adjustCoords(FBRectangle& r);
                bool adjustLineCoords(Scalar& x1, Scalar& y1, Scalar& x2, Scalar& y2);
        };

    }
}
#endif
