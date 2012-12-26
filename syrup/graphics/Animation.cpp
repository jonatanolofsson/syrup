#include <syrup/graphics/Animation.hpp>
#include <syrup/utils/utils.hpp>
#include <syrup/math/math.hpp>

#define STACK_SIZE      512

namespace syrup {
    Animation::Animation(Drawer* display_) :
        display(display_),
        frame(0),
        subframe(0)
        //~ started(0)
    {
        //~ xTaskCreate(utils::classThreadInitializer<Animation>,
                    //~ (signed portCHAR *)"AnimationTask",
                    //~ STACK_SIZE,
                    //~ this,
                    //~ tskIDLE_PRIORITY + 2,
                    //~ NULL);
    }
    void Animation::mix(float* mixed, const float* v1, const float* v2, float alpha)
    {
        for (uint8_t i = 0; i < 2; ++i)
        {
            mixed[i] = alpha * v1[i] + (1.f - alpha) * v2[i];
        }
    }

    void Animation::setAnimation(AnimationData* new_animation)
    {
        current_animation = new_animation;
    }

    void Animation::drawFrame()
    {
        for (uint8_t i = 0; i < current_animation->num_lines; ++i)
        {
            int16_t x1 = current_vertices[current_animation->indices[2*i]][0];
            int16_t y1 = current_vertices[current_animation->indices[2*i]][1];
            int16_t x2 = current_vertices[current_animation->indices[2*i+1]][0];
            int16_t y2 = current_vertices[current_animation->indices[2*i+1]][1];
            display->line(x1, y1, x2, y2);
        }
    }

    void Animation::timeUpdate()
    {
        ++subframe;
        if (subframe >= current_animation->frame_lengths[frame])
        {
            subframe = 0;
            ++frame;
            if (frame >= current_animation->num_frames)
            {
                frame = 0;
            }
        }
        float t = subframe;
        float alpha = t * (1.f / current_animation->frame_lengths[frame]);
        uint8_t next_frame = (frame + 1);
        if (next_frame >= current_animation->num_frames)
        {
            next_frame = 0;
        }
        for (uint8_t i = 0; i < current_animation->num_vertices; ++i)
        {
            mix(&current_vertices[i][0], &current_animation->vertices[frame][i*2], &current_animation->vertices[next_frame][i*2], alpha);
            // Device transformation
            current_vertices[i][0] -= 500.f;
            current_vertices[i][0] *= 300.f/800.f;
            current_vertices[i][1] -= 1000.f;
            current_vertices[i][1] *= 1500.f/1000.f;
        }
    }

    void Animation::run()
    {
        init();
        //~ while(started < 2) taskYIELD();
        for (;;)
        {
            timeUpdate();
            display->clear();
            drawFrame();
            display->flush();
        }
    }
}
