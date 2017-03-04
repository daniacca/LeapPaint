#ifndef CONTROLLERMODULE_H
#define CONTROLLERMODULE_H

#include "Leap.h"
#include "LeapMath.h"
#include "QObject"
using namespace Leap;

//classe che gestisce il controller leap (Controller del pattern MVC)

class ControllerListener : public Listener, QObject
{
    Q_OBJECT
    Controller controller;
public:
    ControllerListener();

private:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&controllery);
private slots:
    void connectLeap();
signals:
    void update();

};
#endif // CONTROLLERMODULE_H
