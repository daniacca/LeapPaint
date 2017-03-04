#include "controllerlistener.h"
#include <QMetaType>

controllerListener::controllerListener(QObject *parent) : QObject(parent)
{

}

void controllerListener::connectLeap()
{
    controller.addListener(*this);
}

void controllerListener::disconnectLeap()
{
    controller.removeListener(*this);
    emit onConnection(false);
}

void controllerListener::onConnect(const Controller& controller)
{
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    emit onConnection(true);
    emit lostConnection(false);
}

void controllerListener::onDisconnect(const Controller& controller)
{
    emit lostConnection(true);
}

void controllerListener::onServiceConnect(const Controller& controller)
{
   std::cout << "Service Connected" << std::endl;
}

void controllerListener::onServiceDisconnect(const Controller& controller)
{
   std::cout << "Service Disconnected" << std::endl;
}

void controllerListener::onFrame(const Controller& controller)
{
    const Frame frame = controller.frame();

    if(frame.hands().isEmpty())
    {
        touching= false;
        tapping = false;
        emit noHandValid();
        return;
    }

    FingerVector* pos = new FingerVector[4];

    for(int i=0; i<4 ;i++)
    {
         pos[i].x = 1000;
         pos[i].y = 1000;
         pos[i].z = 1000;
    }

    pos[0] = FingerVector(frame.hands()[0].fingers()[0].stabilizedTipPosition().x,
                          frame.hands()[0].fingers()[0].stabilizedTipPosition().y,
                          frame.hands()[0].fingers()[0].tipPosition().z);
    pos[1] = FingerVector(frame.hands()[0].fingers()[1].stabilizedTipPosition().x,
                          frame.hands()[0].fingers()[1].stabilizedTipPosition().y,
                          frame.hands()[0].fingers()[1].tipPosition().z);

    if(shapeNumber== 3 && frame.hands()[1].isValid())
    {
        pos[2] = FingerVector(frame.hands()[1].fingers()[1].stabilizedTipPosition().x,
                              frame.hands()[1].fingers()[1].stabilizedTipPosition().y,
                              frame.hands()[1].fingers()[1].tipPosition().z);
        pos[3] = FingerVector(frame.hands()[1].fingers()[0].stabilizedTipPosition().x,
                              frame.hands()[1].fingers()[0].stabilizedTipPosition().y,
                              frame.hands()[1].fingers()[0].tipPosition().z);
    }


    if(validArea(pos))
    {
        pos[0].clamp();
        pos[1].clamp();

        switch(shapeNumber)
        {
            case 1:
                fingersTouching(pos);
                break;
            case 2:
                fingersTouching(pos);
                break;
            case 3:
                twoHandGesture(pos,frame);
                break;
            case 4:
                tappingScreen(frame,pos[1].x,pos[1].y,pos[1].z);
                break;
            case 5:
                fingerIntensitive(pos);
                break;
        }

        qRegisterMetaType<FingerVector>("FingerVector");
        emit fingerPosition(pos,touching, tapping);

        dati[0] = QString::number(frame.id());
        dati[1] = QString::number(frame.timestamp());
        dati[2] = QString::number(frame.hands().count());
        dati[3] = QString::number(frame.gestures().count());
        dati[4] = QString::number(pos[1].x);
        dati[5] = QString::number(pos[1].y);
        dati[6] = QString::number(pos[1].z);
        dati[7] = QString::number(pos[0].x);
        dati[8] = QString::number(pos[0].y);
        dati[9] = QString::number(pos[0].z);
        dati[10] = QString::number(abs(pos[1].x-pos[0].x));
        dati[11] = QString::number(abs(pos[1].y-pos[0].y));
        dati[12] = QString::number(abs(pos[1].z-pos[0].z));

        emit update(dati);
    }
    else
    {
        touching= false;
        tapping = false;
        emit noHandValid();
    }
}

void controllerListener::tappingScreen(Frame frame, int x, int y, int z)
{
    if(!tapping && frame.gestures()[0].isValid())
    {
        tapping = true;
        appx=x;
        appy=y;
        appz=z;
    }
    else if(tapping && frame.gestures()[0].isValid())
    {
        tapping=false;
        QRect rect(appx,appy,(x-appx),(y-appy));
        emit draw(rect);
    }
}

void controllerListener::fingersTouching(FingerVector *vetPos)
{
    int dx=abs(vetPos[0].x-vetPos[1].x);
    int dy=abs(vetPos[0].y-vetPos[1].y);
    int dz=abs(vetPos[0].z-vetPos[1].z);
    if(!touching && (dx<xr && dy<yr && dz<zr))
    {
        touching=true;
        appx=vetPos[1].x;
        appy=vetPos[1].y;
        appz=vetPos[1].z;
    }
    else if(touching && !(dx<xr && dy<yr && dz<zr))
    {
        touching = false;
        QRect rect(appx,appy,vetPos[1].x - appx,vetPos[1].y - appy);
        emit draw(rect);
    }
}

void controllerListener::updateShape(int s)
{
    if(s<=5 && s>0)
    {
        shapeNumber = s;
        tapping=false;
        touching=false;
    }
}

void controllerListener::fingerIntensitive(FingerVector *vetPos)
{
    vetPos[1].z= vetPos[1].z/100;
    tapping=true;
    if(vetPos[1].z < 0)
    {
        emit draw(QRect (vetPos[1].x,vetPos[1].y,1,1));
    }
}

bool controllerListener::validArea(FingerVector *vetPos)
{
    bool results=true;
    int lenght=0;
    int i=0;
    shapeNumber == 3 ? lenght=4 : lenght=2;
    shapeNumber == 5 ? i=1 : i=0;

    for( i; i<lenght; i++)
    {
        if(!(abs(vetPos[i].x)<180 && vetPos[i].y<430 && vetPos[i].y>170 && std::abs(vetPos[i].z)<100)
                && (vetPos[i].x!=1000 || vetPos[i].y!=1000 || vetPos[i].z!=1000))
            results=false;
    }
    return results;
}

void controllerListener::twoHandGesture(FingerVector *vetPos, Frame frame)
{
    if(frame.hands()[1].isValid())
    {
        int dx=abs(vetPos[0].x-vetPos[1].x);
        int dy=abs(vetPos[0].y-vetPos[1].y);
        int dz=abs(vetPos[0].z-vetPos[1].z);

        int dx2=abs(vetPos[2].x-vetPos[3].x);
        int dy2=abs(vetPos[2].y-vetPos[3].y);
        int dz2=abs(vetPos[2].z-vetPos[3].z);

        if(!touching && (dx<xr && dy<yr && dz<zr) && (dx2<xr && dy2<yr && dz2<zr))
        {
            touching=true;
        }
        else if(touching && (!(dx<xr && dy<yr && dz<zr) || !(dx2<xr && dy2<yr && dz2<zr)))
        {
            touching = false;
            emit draw(QRect(QPoint(vetPos[3].x,vetPos[3].y),QPoint(vetPos[1].x,vetPos[1].y)));
        }
    }
}
