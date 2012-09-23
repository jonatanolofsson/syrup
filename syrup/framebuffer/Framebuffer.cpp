#include <syrup/framebuffer/Framebuffer.hpp>
#include <syrup/math/math.hpp>
#include <string.h>

namespace syrup {
    namespace graphics {
        Framebuffer::Framebuffer(BitDisplay* display_) :
                    display(display_),
                    width(display->width()),
                    height(display->height()),
                    kSize(width * height/8),
                    limits(FBRectangle(0,0,width-1,height-1))
        {
            data = (uint8_t*)malloc(kSize);
            memset(data, 0, kSize);
            display->flush(this);
        }

        Framebuffer::Framebuffer(BitDisplay* display_, Scalar* data_) :
                    display(display_),
                    width(display->width()),
                    height(display->height()),
                    kSize(width * height/8),
                    limits(FBRectangle(0,0,width-1,height-1)),
                    data(data_)
        {
            display->flush(this);
        }

        Framebuffer::Framebuffer(BitDisplay* display_, Scalar* data_, Scalar xpos, Scalar ypos, Scalar limwidth, Scalar limheight) :
                    display(display_),
                    width(display->width()),
                    height(display->height()),
                    kSize(width * height/8),
                    limits(FBRectangle(xpos,ypos,xpos+limwidth-1,ypos+limheight-1)),
                    data(data_)
        {
        }

        Framebuffer::Framebuffer(BitDisplay* display_, Scalar* data_, const FBRectangle& limits_) :
                    display(display_),
                    width(display->width()),
                    height(display->height()),
                    kSize(width * height/8),
                    limits(limits_),
                    data(data_)
        {
        }

        Framebuffer* Framebuffer::subBuffer(FBRectangle window)
        {
            adjustCoords(window);
            return new Framebuffer(display, data, window);
        }

        void Framebuffer::init()
        {
            display->init();
        }

        void Framebuffer::flush()
        {
            display->flush(this);
        }

        void Framebuffer::clear()
        {
            clearRectangle(0, 0, limits.x2-limits.x1, limits.y2-limits.y1);
        }

        void Framebuffer::fillRectangle(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
        {
            if(!adjustCoords(x1,y1,x2,y2)) return;
            Scalar mask = (0xff << (y1 % 8));
            for (Scalar y = (y1 / 8); y <= (y2 / 8); ++y)
            {
                if (y == (y2 / 8))
                {
                    mask &= ~(0xfe << (y2 % 8));
                }
                for (Scalar x = x1; x <= x2; ++x)
                {
                    data[x + y * width] |= mask;
                }
                mask = 0xff;
            }
        }

        void Framebuffer::clearRectangle(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
        {
            if(!adjustCoords(x1,y1,x2,y2)) return;
            Scalar mask = ~(0xff << (y1 % 8));
            for (Scalar y = (y1 / 8); y <= (y2 / 8); ++y)
            {
                if (y == (y2 / 8))
                {
                    mask |= (0xfe << (y2 % 8));
                }
                for (Scalar x = x1; x <= x2; ++x)
                {
                    data[x + y * width] &= mask;
                }
                mask = 0x00;
            }
        }

        void Framebuffer::pset(Scalar x, Scalar y)
        {
            if(!adjustCoords(x,y)) return;
            data[x + (y / 8) * width] |= (1 << (y % 8));
        }

        void Framebuffer::pclr(Scalar x, Scalar y)
        {
            if(!adjustCoords(x,y)) return;
            data[x + (y / 8) * width] &= ~(1 << (y % 8));
        }

        /// Bresenham's line drawing algorithm
        void Framebuffer::line(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
        {
            if(!adjustLineCoords(x1,y1,x2,y2)) return;
            int8_t dx = abs(x2 - x1);
            int8_t dy = abs(y2 - y1);
            //~ if (dx == 1 && dy == 1)
            //~ {
                //~ /// \bug ugly hack fix for too small e2 and err variable
                //~ ++dx;
            //~ }

            int8_t sx = 1; // Step direction
            int8_t sy = 1; // Step direction
            if (x1 > x2)
            {
                sx = -1;
            }
            if (y1 > y2)
            {
                sy = -1;
            }

            int16_t err = dx-dy; // Error counter
            Scalar x = x1;
            Scalar y = y1;
            Scalar* pixel = &data[y/8*width + x];
            Scalar by = y % 8; // sub-byte (bit) position
            Scalar bitmask = 1<<by;
            //~ Serial.print("x1 = ");
            //~ Serial.print(x1);
            //~ Serial.print(" x2 = ");
            //~ Serial.print(x2);
            //~ Serial.print(" y1 = ");
            //~ Serial.print(y1);
            //~ Serial.print(" y2 = ");
            //~ Serial.println(y2);
            while (x != x2 || y != y2)
            {
                if (x < limits.x1 || y < limits.y1 || x > limits.x2 || y > limits.y2)
                {
                    // Outside allowed buffer!
                    break;
                }
                //~ Serial.print("x = ");
                //~ Serial.print(x);
                //~ Serial.print(" y = ");
                //~ Serial.print(y);
                pixel = &data[y/8*width + x];
                by = y % 8; // sub-byte (bit) position
                bitmask = 1<<by;
                *pixel |= bitmask;
                int16_t e2 = 2*err;
                //~ Serial.print(" err = ");
                //~ Serial.print(err);
                //~ Serial.print(" e2 = ");
                //~ Serial.println(e2);
                if (e2 > -dy)
                {
                    err -= dy;
                    x   += sx;
                    //~ pixel += sx;
                }
                if (e2 < dx)
                {
                    err += dx;
                    y   += sy;
                }

                //~ Scalar By = y1/8; // Byte position, 1 byte per 8 Y values
                //~ Scalar by = y1%8; // sub-byte (bit) position
                //~ Scalar Bx = x2; // Byte position, 1 byte per 1 X value
                                 //~ // No sub-byte position in X
                //~ Scalar bitmask = 1<<by;
            }
        }

        void Framebuffer::blit(Scalar x0, Scalar y0, const Scalar buffer_width, const Scalar buffer_height, const Scalar* buf, Scalar buffer_stride)
        {
            if(!buffer_stride) buffer_stride = buffer_width;
            //~ SerialUSB.print("Blit: x0=");
            //~ SerialUSB.print(x0); SerialUSB.print(" y0=");
            //~ SerialUSB.print(y0); SerialUSB.print(" buffer_width=");
            //~ SerialUSB.print(buffer_width); SerialUSB.print(" buffer_height=");
            //~ SerialUSB.print(buffer_height); SerialUSB.print(" buffer_stride=");
            //~ SerialUSB.print(buffer_stride); SerialUSB.print(" width=");
            //~ SerialUSB.println(width);

            if(!adjustCoords(x0,y0)) return;
            if (buffer_height == 0 || buffer_width == 0)
            {
                return;
            }
            //~ SerialUSB.println("Blitcheck ok.");
            //~ SerialUSB.print("Data: ");
            //~ SerialUSB.println((int)data);
            //~ SerialUSB.print("Sample: ");
            //~ SerialUSB.println(data[132]);

            Scalar yshifts = y0 % 8; // if y0 is not a multiple of 8 we need to shift the data bytes some bits

            Scalar ysbound = (buffer_height - 1) / 8 + 1; // calculate source buffer bound
            Scalar ytbound = limits.y2 / 8 + 1; // calculate framebuffer bound
            Scalar yt = y0 / 8; // position in framebuffer
            Scalar ys = 0; // position in source buffer
            while (ys < ysbound && yt <= ytbound)
            {
                Scalar source_mask = 0xFF; // rowmask defines which bit rows are blitted
                if ((ys+1)*8 > buffer_height)
                {
                    // partial byte row needs to mask some pixel rows
                    source_mask >>= ((ys+1)*8 - buffer_height);
                }
                uint16_t target_mask = ~(source_mask << yshifts);
                Scalar upper_target_mask = ((target_mask & 0xFF00) >> 8);
                Scalar lower_target_mask = ((target_mask & 0xFF) >> 0);

                // if the target rectangle is unaligned with the framebuffer's byte rows
                // we will need to split the data somewhere and do two writes to the FB
                Scalar xt = x0; // position in framebuffer
                Scalar xs = 0; // position in source buffer
                while (xs < buffer_width && xt <= limits.x2)
                {
                    //~ SerialUSB.print("Buffer data: ");
                    //~ SerialUSB.println(buf[xs + ys * buffer_stride] & source_mask);
                    // source buffer data for upper part of this row
                    uint16_t blit_data =
                        ((buf[xs + ys * buffer_stride] & source_mask) << yshifts);
                        //~ ((0xff & source_mask) << yshifts);
                    Scalar upper_byte = ((blit_data & 0xFF00) >> 8);
                    Scalar lower_byte = ((blit_data & 0xFF) >> 0);
                    lower_byte |= (data[xt + yt * width] & lower_target_mask); // target buffer data that should be unaffected by the blit
                    //~ SerialUSB.print("Previous data: ");
                    //~ SerialUSB.println(data[xt + yt * width]);
                    data[xt + yt * width] = lower_byte;
                    //~ data[xt + yt * width] = 0x55;

                    //~ SerialUSB.print("Wrote to (");
                    //~ SerialUSB.print(xt);
                    //~ SerialUSB.print(",");
                    //~ SerialUSB.print(yt);
                    //~ SerialUSB.print(") @ ");
                    //~ SerialUSB.print(xt + yt * width);
                    //~ SerialUSB.print(" :: ");
                    //~ SerialUSB.println(lower_byte);

                    if (yt + 1 < ytbound && upper_target_mask != 0xFF)
                    {
                        upper_byte |= (data[xt + (yt+1) * width] & upper_target_mask); // target buffer data that should be unaffected by the blit
                        data[xt + (yt+1) * width] = upper_byte;
                        //~ SerialUSB.print("Wrote to (");
                        //~ SerialUSB.print(xt);
                        //~ SerialUSB.print(",");
                        //~ SerialUSB.print(yt+1);
                        //~ SerialUSB.print(") @ ");
                        //~ SerialUSB.print(xt + (yt+1) * width);
                        //~ SerialUSB.print(" :: ");
                        //~ SerialUSB.println(upper_byte);
                        //~ data[xt + (yt+1) * width] = 0x00;
                    }
                    ++xt; // increment target x position
                    ++xs; // increment source x position
                }
                ++yt;
                ++ys;
            }
        }
        bool Framebuffer::adjustCoords(Scalar& x1, Scalar& y1)
        {
            bool rval = true;
            x1 += limits.x1; if(x1 > limits.x2) {rval = false; x1 = limits.x2;}
            y1 += limits.y1; if(y1 > limits.y2) {rval = false; y1 = limits.y2;}

            return rval;
        }
        bool Framebuffer::adjustCoords(Scalar& x1, Scalar& y1, Scalar& x2, Scalar& y2)
        {
            bool rval = true;
            x1 += limits.x1; if(x1 > limits.x2) {rval = false; x1 = limits.x2;}
            y1 += limits.y1; if(y1 > limits.y2) {rval = false; y1 = limits.y2;}
            x2 = min(limits.x2, (uint8_t)(x2 + limits.x1));
            y2 = min(limits.y2, (uint8_t)(y2 + limits.y1));

            return rval;
        }
        bool Framebuffer::adjustLineCoords(Scalar& x1, Scalar& y1, Scalar& x2, Scalar& y2)
        {
            x1 += limits.x1;
            y1 += limits.y1;
            x2 += limits.x1;
            y2 += limits.y1;

            return true;
        }
        bool Framebuffer::adjustCoords(FBRectangle& r)
        {
            return adjustCoords(r.x1, r.y1, r.x2, r.y2);
        }

        void Framebuffer::vscroll(int8_t offset)
        {

        }
    }
}
