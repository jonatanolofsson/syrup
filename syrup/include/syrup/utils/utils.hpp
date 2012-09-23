#ifndef SYRUP_UTILS_HPP_
#define SYRUP_UTILS_HPP_

#define USE_FREERTOS

#ifdef USE_FREERTOS
    #include <FreeRTOS/MapleFreeRTOS.h>
    #define DELAY(T)    vTaskDelay((T) / portTICK_RATE_MS)
#else
    #define DELAY(T)    delay((T))
#endif

#include <wirish/wirish.h>
namespace syrup {
    namespace utils {
        template<typename T>
        static void classThreadInitializer(void *object) {
            ((T*)object)->run();
        }
        template<typename T>
        static void classThreadInitializer2(void *object) {
            ((T*)object)->run2();
        }

        template<typename T>
        void classInterruptHandler(void* ptr) {((T*)ptr)->run();}
    }
}

#endif
