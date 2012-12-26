#include <syrup/graphics/Scope.hpp>
#include <syrup/math/math.hpp>

namespace syrup {
    Scope::Scope(Framebuffer& fb_) :
        fb(fb_),
        current_x(0),
        min_y(0), max_y(0),
        scale_y(1),
        autoscale(true)
    {}

    Scope& Scope::put(YValueType y)
    {
        //~ static const uint8_t fill[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        if (autoscale && y > max_y)
        {
            ylim(min_y, y);
        }
        else if (autoscale && y < min_y)
        {
            ylim(y, max_y);
        }
        YValueType norm_y = (y - min_y) * scale_y;
        DimensionType height = fb.limits.y2 - fb.limits.y1;
        DimensionType width = fb.limits.x2 - fb.limits.x1;
        DimensionType fb_y = round(height - norm_y - 1); // flip Y-axis
        fb_y = clamp(0, fb_y, height);
        fb.clearRectangle(current_x, 0, current_x, fb.limits.y2);
        fb.pset(current_x, fb_y);
        ++current_x;
        if (current_x >= width)
        {
            current_x = 0;
        }
        return *this;
    }
    Scope& Scope::operator<<(YValueType y)
    {
        return put(y);
    }

    Scope& Scope::clear()
    {
        current_x = 0;
        return *this;
    }

    Scope& Scope::ylim(YValueType ymin, YValueType ymax)
    {
        DimensionType height = fb.limits.y2 - fb.limits.y1;
        autoscale = false;
        min_y = ymin;
        max_y = ymax;
        scale_y = height / (max_y - min_y);
        offset_y = -min_y;
        return *this;
    }
}
