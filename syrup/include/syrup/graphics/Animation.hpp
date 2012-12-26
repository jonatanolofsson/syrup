#ifndef SYRUP_GRAPHICS_ANIMATION_HPP_
#define SYRUP_GRAPHICS_ANIMATION_HPP_

#include <stdint.h>
#include <syrup/graphics/Drawer.hpp>

namespace syrup {
    struct AnimationData
    {
        public:
            static const uint8_t max_frames = 16; // Arbitrary limit
            uint8_t num_frames;
            uint8_t num_lines;
            uint8_t num_vertices;
            const float* vertices[max_frames];
            const uint8_t* indices; // start and end of each line
            const uint16_t* frame_lengths;
    };

    class Animation
    {
        //~ private:
        public:
            Drawer* display;
            static const uint8_t max_vertices = 32; // Arbitrary limit
            uint8_t frame;
            uint16_t subframe;
            float current_vertices[max_vertices][2];
            AnimationData* current_animation;

            static void mix(float* mixed, const float* v1, const float* v2, float alpha);

        public:
            uint8_t started;
            Animation(Drawer* display_);

            void setAnimation(AnimationData*);
            void drawFrame();
            void timeUpdate();
            void run();
    };
}

#endif
