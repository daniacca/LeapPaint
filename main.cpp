#include "mywindow.h"
#include "controllerlistener.h"
#include <QApplication>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    // Application Window - View
    LeapPaintWindow w;

    // Application Controller
    controllerListener mycontroller;

    // Create the SIGNAL-SLOTS connection
    QObject::connect(&w,&LeapPaintWindow::connectLeap,&mycontroller,&controllerListener::connectLeap);
    QObject::connect(&w,&LeapPaintWindow::disconnectLeap,&mycontroller,&controllerListener::disconnectLeap);
    QObject::connect(&mycontroller,&controllerListener::draw,&w,&LeapPaintWindow::drawSlots);
    QObject::connect(&w,&LeapPaintWindow::updateShape,&mycontroller,&controllerListener::updateShape);
    QObject::connect(&mycontroller,&controllerListener::fingerPosition,&w,&LeapPaintWindow::fingerPosition);
    QObject::connect(&mycontroller,&controllerListener::noHandValid,&w,&LeapPaintWindow::noHandValid);
    QObject::connect(&mycontroller,&controllerListener::update,&w,&LeapPaintWindow::update);
    QObject::connect(&mycontroller,&controllerListener::onConnection,&w,&LeapPaintWindow::onConnection);
    QObject::connect(&mycontroller,&controllerListener::lostConnection,&w,&LeapPaintWindow::lostConnection);

    // Start Application
    w.show();
    return a.exec();
}
