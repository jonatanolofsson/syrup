#include <syrup/graphics/HddDisplay.hpp>
#include <wirish/wirish.h>

#define YOFFSET     50

namespace syrup {
    void HddDisplay::line(const Scalar x1, const Scalar y1, const Scalar x2, const Scalar y2) {
        x->setReference(x1);
        y->setReference(y1 + YOFFSET);
        x->waitForIt();
        y->waitForIt();
        laser->on();
        x->setReference(x2);
        y->setReference(y2 + YOFFSET);
        x->waitForIt();
        y->waitForIt();
        laser->off();
    }
}
