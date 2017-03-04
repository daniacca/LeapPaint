#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "canvas.h"
#include "QWidget"
#include "QTextEdit"
#include "QLineEdit"
#include "QLabel"
#include "QPushButton"
#include "QActionGroup"
#include <QMainWindow>

namespace Ui {
class MyWindow;
}

class LeapPaintWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LeapPaintWindow(QWidget *parent = 0);
    ~LeapPaintWindow();

public slots:
     void update(QString* dates);
     void drawSlots(QRect rect);
     void fingerPosition(FingerVector* pos, bool touch, bool tap);
     void noHandValid();
     void onConnection(bool globalConnectionFlag);
     void lostConnection(bool eventNotification);

private slots:
    void connectEvent();
    void changeShape(canvas::Shape shape);
    void lineEvent();
    void rectEvent();
    void roundedRectEvent();
    void ellipseEvent();
    void pencilEvent();
    void penColor();
    void brushColor();
    void brushWidth();
    void brushTrasparent();
    void createActions();
    void createMenus();
    void open();
    void newFile();
    bool saveAs();

signals:
    void connectLeap();
    void disconnectLeap();
    void updateShape(int s);

private:
    Ui::MyWindow *ui;
    canvas *mycanvas;

   /* QLabel *fingers_label;
    QLabel *hand_label;
    QLabel *timestamp_label;
    QLabel *gesture_label;
    QLabel *frameid_label;
    QLineEdit *hand;
    QLineEdit *timestamp;
    QLineEdit *gesture;
    QLineEdit *frameid; */
    QTextEdit *fingers;
    QLabel *lostConnectionLabel;

    QPushButton *connect_button;
    QPushButton *line;
    QPushButton *rect;
    QPushButton *rounded_rect;
    QPushButton *ellipse;
    QPushButton *pencil;

    QPixmap imageLine;
    QPixmap imageRect;
    QPixmap imageRoundedRect;
    QPixmap imageEllipse;
    QPixmap imagePencil;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QActionGroup *brushActionGroup;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *brushWidthAct;
    QAction *brushColorAct;
    QAction *penColorAct;
    QAction *brushTrasparentAct;
    QAction *newAct;

    bool globalConnectionFlag;

};

#endif // MYWINDOW_H
