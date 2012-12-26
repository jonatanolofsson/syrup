#include <syrup/comm/ByteInterface.hpp>
//~ #include <assert.h>
#include <wirish/wirish.h>
#include <syrup/types.hpp>

#define MAX_NUMBER_OF_DESCRIPTORS           (10)
namespace syrup {
    ByteInterface* descriptors[MAX_NUMBER_OF_DESCRIPTORS];
    int currentDescriptor = 0;

    int open(ByteInterface* const iface) {
        //~ assert(currentDescriptor < MAX_NUMBER_OF_DESCRIPTORS);
        descriptors[currentDescriptor] = iface;
        return currentDescriptor++;
    }

    int write(const int descriptor, const U8* buf, const int length) {
        //~ assert(descriptor < currentDescriptor);
        //~ Serial3.print("Write to descriptor ");
        //~ Serial3.print(descriptor);
        //~ Serial3.print(", ");
        //~ Serial3.print(length);
        //~ Serial3.println("bytes");
        descriptors[descriptor]->send(buf, length);
        return length;
    }

    int read(const int descriptor, U8* const buf, const int length) {
        //~ assert(descriptor < currentDescriptor);
        descriptors[descriptor]->get(buf, length);
        return length;
    }
}
