#include "canvas.h"
#include "QPainter"
#include <QRect>
#include <QResizeEvent>
#include <iostream>
#include <QColor>
#include <QLinearGradient>

template <typename T>
static T map(T n, T a1, T b1, T a2, T b2){
    return (((n - a1)/(b1-a1)) * (b2-a2)) + a2;
}

template <typename T>
static T mapv(T n, T a1, T b1, T a2, T b2){
    return ((n/(b1-a1)) * (b2-a2));
}

canvas::canvas(QWidget *parent): QWidget(parent), pixmap(parent->width(),parent->height())
{
    shape = Rect;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    this->brush = QBrush(QColor::fromRgb(255,255,255,0));
    QPainter painter(&pixmap);
    QBrush brush(QColor::fromRgb(255,255,255));
    painter.fillRect(0,0,pixmap.width(),pixmap.height(),brush);
    pos=nullptr;
}

QSize canvas::minimumSizeHint() const
{
    return QSize(400, 200);
}

QSize canvas::sizeHint() const
{
    return QSize(800, 600);
}

void canvas::setShape(Shape shape)
{
    this->shape = shape;
    update();
}

void canvas::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void canvas::draw(QRect r)
{
    QRect rect(map<double>(r.x(), -150, 150, 0, width()),
               height() - map<double>(r.y(), 200, 400, 0, height()),
               mapv<double>(r.width(), -150, 150, 0, width()),
               -mapv<double>(r.height(),200, 400, 0, height()));

    QPainter painter(&pixmap);
    painter.setBrush(brush);
    painter.setPen(pen);
    switch (shape)
    {
        case Line:
            painter.drawLine(rect.topLeft().x(),rect.topLeft().y(),
                             rect.bottomRight().x(),rect.bottomRight().y());
            break;
        case Rect:
            painter.drawRect(rect);
            break;
        case RoundedRect:
            painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
            break;
        case Ellipse:
            painter.drawEllipse(rect);
            break;
        case Pencil:
            QColor color = pen.color();
            QBrush brushPencil = QBrush(QColor(color.red(),color.green(),color.blue(),255));
            painter.setBrush(brushPencil);
            if(previusX >= 0 && previusY >= 0)
            painter.drawLine(previusX,previusY,
                             rect.topLeft().x(),rect.topLeft().y());
            break;
    }
    previusX = rect.topLeft().x();
    previusY = rect.topLeft().y();
    touching = false;
    tapping = false;
    if (shape!= Pencil)
    {
        appTouchx = -1;
        appTouchy = -1;
    }
    update();
}

void canvas::paintEvent(QPaintEvent * /* event */)
{
    int x,y;
    int raggio=5;
    int lenght=2;
    int i=0;
    if(shape == Pencil)
    {
        raggio=15;
        i=1;
    }
    shape == Ellipse ? (lenght=4) : lenght =2;
    QPainter painter(this);
    painter.setPen(pen);
    painter.drawPixmap(0,0,pixmap);
    if(!pos) return;
    for(i; i<lenght; i++)
    {
        x = map<double>(pos[i].x, -150, 150, 0, width());
        y = map<double>(pos[i].y, 200, 400, 0, height());
        painter.drawEllipse(QPoint(x, height()-y), raggio,raggio);
    }

    if((touching == true || tapping == true) && (appTouchx <0 || appTouchy <0))
    {
        appTouchx = x;
        appTouchy = y;

    }
    else if((touching == true || tapping == true) && (appTouchx>=0 && appTouchy >=0))
    {
        switch (shape)
        {
            case Line:
                painter.drawLine(QPoint(appTouchx,height()-appTouchy), QPoint(x, height()-y));
                break;
            case Rect:
                painter.drawRect(QRect(QPoint(appTouchx,height()-appTouchy),QPoint(x, height()-y)));
                break;
            case RoundedRect:
                painter.drawRoundedRect(QRect(QPoint(appTouchx,height()-appTouchy),QPoint(x, height()-y)), 25, 25, Qt::RelativeSize);
                break;
            case Ellipse:
                painter.drawEllipse(QRect(QPoint(map<double>(pos[3].x, -150, 150, 0, width()),height()-map<double>(pos[3].y, 200, 400, 0, height())),
                                          QPoint(map<double>(pos[1].x, -150, 150, 0, width()),height()-map<double>(pos[1].y, 200, 400, 0, height()))));
                break;
            case Pencil:
                painter.drawText(QPoint(x-12, height()-y+6),QString::number(pos[1].z));
            break;
        }
    }

    if(pos[1].z >=0){
        previusX = -1;
        previusY = -1;
    }

    delete [] pos;
    pos = nullptr;
}

void canvas::resizeEvent(QResizeEvent* event)
{
    QWidget::resize(event->size().width(), event->size().height());
    QPixmap newPixmap = QPixmap(event->size().width(), event->size().height());
    QPainter painter(&newPixmap);
    painter.fillRect(0,0,event->size().width(), event->size().height(),QBrush(QColor::fromRgb(255,255,255)));
    painter.drawPixmap(0,0, pixmap);
    pixmap = newPixmap;

}

void canvas::setPos(FingerVector* posizione)
{
    pos=posizione;
    update();
}

void canvas::setTouching(bool touch)
{
    touching=touch;
}

void canvas::setTapping(bool tap)
{
    tapping=tap;
}

bool canvas::openImage(const QString &fileName)
{
    pixmap = QPixmap(fileName);
    update();
    updateGeometry();
    return true;
}

bool canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    return pixmap.save(fileName, fileFormat);
}

void canvas::setBrushColor(const QColor &color)
{
    brush.setColor(color);
}

void canvas::setBrushWidth(int width)
{
    pen.setWidth(width);
}

void canvas::setPenColor(const QColor &color)
{
    pen.setColor(color);
}

void canvas::setAppVariable(int x,int y)
{
    appTouchx = x;
    appTouchy = y;
}

void canvas::setBrushTrasparent()
{
    QColor color = brush.color();
    brush.setColor(QColor(color.red(),color.green(),color.blue(),0));
}

void canvas::inizialize()
{
    pen = QPen(QColor::fromRgb(0,0,0));
    this->brush = QBrush(QColor::fromRgb(255,255,255,0));
    QPainter painter(&pixmap);
    QBrush brush(QColor::fromRgb(255,255,255));
    painter.fillRect(0,0,pixmap.width(),pixmap.height(),brush);
    pos=nullptr;
    touching = false;
    tapping = false;
    appTouchx = -1;
    appTouchy = -1;
    previusX = -1;
    previusY= -1;
    update();
}

void canvas::setPreviousVaraible(int x, int y)
{
    previusX = x;
    previusY= y;
}
