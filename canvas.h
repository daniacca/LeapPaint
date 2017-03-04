#ifndef CANVAS_H
#define CANVAS_H
#include <QPen>
#include <QPixmap>
#include <QBrush>
#include <QWidget>
#include <fingervector.h>

class canvas : public QWidget
{
public:
    enum Shape { Rect=1, RoundedRect, Ellipse,Line, Pencil };
    canvas(QWidget *parent = 0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    Shape shape;
public slots:
    void inizialize();
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void draw(QRect rect);

    void setPos(FingerVector* posizione);
    void setTouching(bool touch);
    void setTapping(bool tap);
    void setAppVariable(int x,int y);
    void setPreviousVaraible(int x, int y);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setImage(const QPixmap &image);

    void setPenColor(const QColor &color);
    void setBrushColor(const QColor &color);
    QColor brushColor() const { return brush.color(); }
    QColor penColor() const { return pen.color(); }
    void setBrushTrasparent();
    int brushWidth() const { return thickness; }
    void setBrushWidth(int width);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

private:
    QPen pen;
    QPixmap pixmap;
    FingerVector* pos;
    QPainterPath pendingPath;

    QBrush brush;
    QColor color;
    int thickness;

    bool touching=false;
    bool tapping = false;
    int appTouchx = -1;
    int appTouchy = -1;
    int previusX = -1;
    int previusY = -1;
};

#endif // CANVAS_H
