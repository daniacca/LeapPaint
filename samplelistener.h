#ifndef SAMPLELISTENER_H
#define SAMPLELISTENER_H

#include "Leap.h"
#include "LeapMath.h"
using namespace Leap;

class samplelistener : public Listener
{
public:
    samplelistener();
    void onConnect(const Controller& controller);
    void onFrame(const Controller& controller);
};

#endif // SAMPLELISTENER_H
