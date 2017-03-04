#include "samplelistener.h"

samplelistener::samplelistener()
{

}

void samplelistener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
}

void samplelistener::onFrame(const Controller& controller) {
    std::cout << "Frame available" << std::endl;
}

