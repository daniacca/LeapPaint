#ifndef CONTROLLERLISTENER_H
#define CONTROLLERLISTENER_H

#include "Leap.h"
#include "LeapMath.h"
#include "QObject"
#include "samplelistener.h"
#include "mywindow.h"
#include "canvas.h"
#include "fingervector.h"
using namespace Leap;

class controllerListener : public QObject,Listener
{
    Q_OBJECT
    const int xr = 30;
    const int yr = 30;
    const int zr = 30;
    int shapeNumber = 1;
    int appx,appy,appz;
    bool touching =false;
    bool tapping =false;

    Controller controller;
    QString dati[15];

public:
    explicit controllerListener(QObject *parent = 0);
    void fingersTouching(FingerVector *vetPos);
    void tappingScreen(Frame frame, int x, int y, int z);
    void fingerIntensitive(FingerVector *vetPos);
    bool validArea(FingerVector *vetPos);
    void twoHandGesture(FingerVector *vetPos, Frame frame);
private slots:
    void onConnect(const Controller& controller);
    void onFrame(const Controller& controller);
    void onDisconnect(const Controller& controller);
    void onServiceConnect(const Controller& controller);
    void onServiceDisconnect(const Controller& controller);
public slots:
    void connectLeap();
    void disconnectLeap();
    void updateShape(int s);
signals:
    void update(QString* dates);
    void draw(QRect rect);
    void fingerPosition(FingerVector* pos, bool touch, bool tap);
    void noHandValid();
    void onConnection(bool connectionFlag);
    void lostConnection(bool eventNotification);
};

#endif // CONTROLLERLISTENER_H
