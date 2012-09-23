#ifndef SYRUP_STATEMACHINE_HPP_
#define SYRUP_STATEMACHINE_HPP_

namespace syrup {
    namespace utils {
        template<typename T>
        class StateMachine {
            public:
                typedef void*(*stateMachineFunction)(T* const);
                stateMachineFunction currentState;
                bool running;

                StateMachine(stateMachineFunction initialState)
                : currentState(initialState), running(true) {}
                void tick() {
                    if(currentState) {
                        currentState = (stateMachineFunction)currentState((T*)this);
                    } else {
                        taskYIELD();
                    }
                }
                virtual void run() { while(running) tick(); }
        };
    }
}

#endif
