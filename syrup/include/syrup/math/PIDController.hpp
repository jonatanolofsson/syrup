#ifndef SYRUP_MATH_PIDCONTROLLER_HPP_
#define SYRUP_MATH_PIDCONTROLLER_HPP_

#include <syrup/math/math.hpp>
#include <wirish/wirish.h>

namespace syrup {
    template<typename T>
    class PIDController {
        public:
            typedef T type;
            int vmin, vmax;
            int eimin, eimax;
            int ep, ei, e, r, u, y;
            int K, Ti, Td;
            int P, I, D;
            int lastInvokation;
            PIDController()
            :
                vmin(-1024), vmax(1023),
                eimin(-1024), eimax(1023),
                ep(0), ei(0), e(0), r(0), u(0),
                K(10), Ti(500000), Td(3000),
                lastInvokation(0)
            {}

            T operator()(T y_) {
                int32_t Ts;
                y = y_;
                e = r - y;

                if(lastInvokation == 0) {
                    lastInvokation = micros();
                    ep = e;
                    return 0;
                }
                Ts = micros() - lastInvokation;

                P = K*e;
                //~ I = clamp(eimin, I + K*Ts*e/Ti, eimax);
                D = (K*Td*(e - ep))/Ts;

                u = P + D;
                u = clamp(vmin, u, vmax);

                //~ u = ei; // I

                ep = e;
                lastInvokation = micros();
                //~ Serial1.print("Control: ");
                //~ Serial1.print(" [ ");
                //~ Serial1.print(Ts);
                //~ Serial1.print(" ] ");
                //~ Serial1.print(e);
                //~ Serial1.print(" => ");
                //~ Serial1.print(P);
                //~ Serial1.print(" + ");
                //~ Serial1.print(I);
                //~ Serial1.print(" + ");
                //~ Serial1.print(D);
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
            void imc(T Tc, T To, T Kp, T L = 0) {
                T tau = L / (L + To);
                K = ((L + To) / (Kp * (Tc + L))) -  L/2  / (Kp * (Tc + L));
                Ti = (L + To) - L/2;
                Td = L * (1-tau) / (2-tau);
                Serial1.print("Setting IMC parameters: ");
                Serial1.print(K);
                Serial1.print(", ");
                Serial1.print(Ti);
                Serial1.print(", ");
                Serial1.println(Td);
            }
            void lambda(T Tc, T To, T Kp,  T L = 0) {
                K = To / (Kp * Tc + L);
                Ti = To;
                Td = 0;
                Serial1.print("Setting lambda parameters: ");
                Serial1.print(K);
                Serial1.print(", ");
                Serial1.print(Ti);
                Serial1.print(", ");
                Serial1.println(Td);
            }
    };
}
#endif
