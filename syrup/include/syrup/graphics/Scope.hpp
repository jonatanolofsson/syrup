#ifndef FRAMEBUFFER_SCOPE_HPP
#define FRAMEBUFFER_SCOPE_HPP
#include <syrup/graphics/TextConsole.hpp>

namespace syrup {
    class Scope
    {
        public:
            typedef float YValueType;
            typedef Framebuffer::FBDimensionType DimensionType;
            Framebuffer& fb;
            DimensionType current_x;
            YValueType min_y, max_y;
            YValueType offset_y, scale_y;
            bool autoscale;

            Scope(Framebuffer& fb_);

            Scope& put(YValueType y);
            Scope& operator<<(YValueType y);
            Scope& clear();
            Scope& ylim(YValueType ymin, YValueType ymax);
    };
}
#endif // FRAMEBUFFER_SCOPE_HPP
