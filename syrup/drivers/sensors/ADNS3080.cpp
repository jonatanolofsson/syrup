#include <syrup/drivers/sensors/ADNS3080.hpp>
#include <wirish/wirish.h>
#include <wirish/dma_message.h>
#include <syrup/math/math.hpp>
#include <syrup/isr.hpp>

void print(int v, int base = 10);

namespace syrup {
    static volatile unsigned char triggerData = ADNS3080::MOTION_BURST;

    ADNS3080::ADNS3080(HardwareSPI* const dev_)
    :   SuperSensor(),
        device(dev_),
        sspin(dev_->nssPin())
    {
        setup();
    }

    void ADNS3080::setup() {
        trigger(&triggerData, 1, &dmaMemberCallback<ADNS3080, &ADNS3080::getData>, this);
        message(buffer, 7, &dmaMemberCallback<ADNS3080, &ADNS3080::saveData>, this);
        pinMode(sspin, OUTPUT);
    }

    void ADNS3080::getData(dma_message*, dma_irq_cause) {
        device->read(message);
    }

    void ADNS3080::saveData(dma_message*, dma_irq_cause) {
        digitalWrite(sspin, HIGH);
        data[bufferswitch][0] += (S8)buffer[1];
        samples[bufferswitch][0] = 1;
        data[bufferswitch][1] += (S8)buffer[2];
        samples[bufferswitch][1] = 1;
        isr::queue(&isr::invokeMember<ADNS3080, &ADNS3080::printBuffer>, this);
    }

    void ADNS3080::printBuffer() {
        print(data[bufferswitch][0]);
        Serial1.print("         ");
        print(data[bufferswitch][1]);
        Serial1.println("");
        clear();
    }

    void ADNS3080::sample()
    {
        digitalWrite(sspin, LOW);
        device->write(trigger);
    }
} // namespace syrup
