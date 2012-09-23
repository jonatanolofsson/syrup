#ifndef SYRUP_MATH_PIDCONTROLLER_HPP_
#define SYRUP_MATH_PIDCONTROLLER_HPP_

#include <syrup/math/math.hpp>
#include <wirish/wirish.h>

namespace syrup {
    namespace math {
        template<typename T>
        class PIDController {
            public:
                T vmin, vmax;
                T eimin, eimax;
                T ep, ei, e, r, u;
                T K, Ti, Td;
                int lastInvokation;
                PIDController()
                :
                    vmin(-1000), vmax(1000),
                    eimin(-1000), eimax(1000),
                    ep(0), ei(0), e(0), r(0), u(0),
                    K(1), Ti(1), Td(1000),
                    lastInvokation(0)
                {}

                T operator()(T y) {
                    int32_t Ts;
                    e = r - y;

                    if(lastInvokation == 0) {
                        lastInvokation = millis();
                        ep = e;
                        return 0;
                    }
                    Ts = millis() - lastInvokation;

                    ei  = clamp(eimin, ei + K*Ts*e/Ti, eimax);
                    //~ u   = clamp(vmin, K*e + ei + K*Td/Ts*(e - ep), vmax);

                    u = K*e + ei; // P + I

                    ep = e;
                    lastInvokation = millis();
                    //~ Serial1.print("Control: ");
                    //~ Serial1.print(" [ ");
                    //~ Serial1.print(Ts);
                    //~ Serial1.print(" ] ");
                    //~ Serial1.print(e);
                    //~ Serial1.print(" => ");
                    //~ Serial1.print(K*e);
                    //~ Serial1.print(" + ");
                    //~ Serial1.print(ei);
                    //~ Serial1.print(" = ");
                    //~ Serial1.println(u);
                    return u;
                }
                void setReference(T r_) {
                    r = r_;
                }
                void configure(T K_, T Ti_, T Td_) {
                    K   = K_;
                    Ti  = Ti_;
                    Td  = Td_;
                }
        };
    }
}
#endif
