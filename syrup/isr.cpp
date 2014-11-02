#include <syrup/isr.hpp>
#include <wirish/wirish.h>

#define NUM_ISRS    50
namespace syrup {
    namespace isr {
        volatile bool dying = false;
        int lastIsr = 0;
        int nextIsr = 0;
        struct isrCallbackStruct {
            isrCallback cb;
            void* arg;
        };
        isrCallbackStruct isrs[NUM_ISRS];
        void queue(isrCallback cb, void* arg) {
            if((lastIsr + 1) % NUM_ISRS != nextIsr) {
                isrs[lastIsr] = (isrCallbackStruct){cb, arg};
                lastIsr = (lastIsr+1)%NUM_ISRS;
            } //FIXME error reporting
        }
        void serveOne() {
            bool hasMore = (nextIsr != lastIsr);
            //digitalWrite(BOARD_LED_PIN, !hasMore);
            if(hasMore) {
                (*isrs[nextIsr].cb)(isrs[nextIsr].arg);
                nextIsr = (nextIsr+1)%NUM_ISRS;
            }
        }
        void serviceLoop() {
            while(!dying) {
                serveOne();
            }
        }
        void kill() {
            dying = true;
        }
    }
}
