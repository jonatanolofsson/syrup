#ifndef SYRUP_ISR_HPP_
#define SYRUP_ISR_HPP_

namespace syrup {
    namespace isr {
        typedef void(*isrCallback)(void*);

        void queue(isrCallback, void* argument = nullptr);
        void serveOne();
        void serviceLoop();
        void kill();
    }
}

#endif
