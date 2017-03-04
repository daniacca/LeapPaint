#ifndef FINGERVECTOR_H
#define FINGERVECTOR_H

#include <QMetaType>

#define CLAMP(n, l, h) ((n<l) ? l : ((n>h) ? h : n))

struct FingerVector
{
    static const int BOUNDXL = -150;
    static const int BOUNDXH = 150;
    static const int BOUNDYL = 200;
    static const int BOUNDYH = 400;
    static const int BOUNDZL = -100;
    static const int BOUNDZH = 100;

    double x, y, z;
    FingerVector(){

    }

    FingerVector(int x, int y,int z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void clamp()
    {
        x = CLAMP(x, BOUNDXL, BOUNDXH);
        y = CLAMP(y, BOUNDYL, BOUNDYH);
        z = CLAMP(z, BOUNDZL, BOUNDZH);
    }
};

Q_DECLARE_METATYPE(FingerVector)

#endif // FINGERVECTOR_H
