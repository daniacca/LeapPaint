#include "mywindow.h"
#include "ui_mywindow.h"
#include "canvas.h"
#include <QtWidgets>

LeapPaintWindow::LeapPaintWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
    mycanvas = new canvas(this);
    globalConnectionFlag = false;
/*
    frameid_label = new QLabel(tr("frame id"));
    timestamp_label = new QLabel(tr("Timestamp"));
    hand_label = new QLabel(tr("Numero Mani"));
    fingers_label = new QLabel(tr("Dati dita"));
    gesture_label = new QLabel(tr("Numero Gesture cerchio :"));
    frameid = new QLineEdit();
    timestamp = new QLineEdit();
    hand = new QLineEdit();
    gesture = new QLineEdit();*/

    lostConnectionLabel = new QLabel(tr("Connessione con\nil sensore persa!"));
    lostConnectionLabel->setStyleSheet("QLabel { color : red; }");
    lostConnectionLabel->setVisible(false);

    fingers = new QTextEdit();
    connect_button = new QPushButton(tr("Collega leap"));

    imageLine.load("./img/line.png");
    line = new QPushButton;
    line->setIcon(QIcon(imageLine));
    line->setIconSize(QSize(30,30));

    imageEllipse.load("./img//ellipse.png");
    ellipse = new QPushButton;
    ellipse->setIcon(QIcon(imageEllipse));
    ellipse->setIconSize(QSize(30,30));

    imageRect.load("./img/rect.png");
    rect = new QPushButton;
    rect->setIcon(QIcon(imageRect));
    rect->setIconSize(QSize(30,30));

    imageRoundedRect.load("./img/rounded_rect.png");
    rounded_rect = new QPushButton;
    rounded_rect->setIcon(QIcon(imageRoundedRect));
    rounded_rect->setIconSize(QSize(30,30));

    imagePencil.load("./img/pencil.png");
    pencil = new QPushButton;
    pencil->setIcon(QIcon(imagePencil));
    pencil->setIconSize(QSize(30,30));

    connect(connect_button,SIGNAL(clicked(bool)),this,SLOT(connectEvent(void)));
    connect(line,SIGNAL(clicked(bool)),this,SLOT(lineEvent(void)));
    connect(ellipse,SIGNAL(clicked(bool)),this,SLOT(ellipseEvent(void)));
    connect(rect,SIGNAL(clicked(bool)),this,SLOT(rectEvent(void)));
    connect(rounded_rect,SIGNAL(clicked(bool)),this,SLOT(roundedRectEvent(void)));
    connect(pencil,SIGNAL(clicked(bool)),this,SLOT(pencilEvent(void)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(mycanvas, 0, 1, 10, 5);

    //mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setRowStretch(5,1);

    //mainLayout->addWidget(frameid_label, 5, 2, Qt::AlignRight);
    //mainLayout->addWidget(frameid,5,3);
    //mainLayout->addWidget(timestamp_label,6,2,Qt::AlignRight);
    //mainLayout->addWidget(timestamp,6,3);
    //mainLayout->addWidget(hand_label,7,2,Qt::AlignRight);
    //mainLayout->addWidget(hand,7,3);
    //mainLayout->addWidget(gesture_label,8,2,Qt::AlignRight);
    //mainLayout->addWidget(gesture,8,3);
    //mainLayout->addWidget(fingers_label,5,4,Qt::AlignRight);

    mainLayout->removeWidget(mycanvas);
    mainLayout->addWidget(mycanvas, 0, 1, 6, 5);
    mainLayout->addWidget(fingers,6,1,4,5);

    mainLayout->addWidget(connect_button, 9, 0);
    mainLayout->addWidget(lostConnectionLabel, 8, 0);
    mainLayout->addWidget(line, 0, 0);
    mainLayout->addWidget(rect, 1, 0);
    mainLayout->addWidget(ellipse, 2, 0);
    mainLayout->addWidget(rounded_rect, 3, 0);
    mainLayout->addWidget(pencil, 4, 0);
    QWidget *widget = new QWidget();
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    setWindowTitle(tr("Basic LeaPaint"));

    createActions();
    createMenus();
}

LeapPaintWindow::~LeapPaintWindow()
{
    delete ui;
}

void LeapPaintWindow::onConnection(bool connectionFlag)
{
    globalConnectionFlag = connectionFlag;
    QPalette pal = connect_button->palette();

    if(connectionFlag)
    {
        pal.setColor(QPalette::Button, QColor(Qt::green));
        connect_button->setText("Disabilita Leap");
    }
    else
    {
        pal.setColor(QPalette::Button, QColor(Qt::red));
        connect_button->setText("Collega Leap");
    }

    connect_button->setAutoFillBackground(true);
    connect_button->setPalette(pal);
    connect_button->update();
}

void LeapPaintWindow::lostConnection(bool eventNotification)
{
    lostConnectionLabel->setVisible(eventNotification);
}

void LeapPaintWindow::update(QString* dates)
{
    /*
    frameid->setText(dates[0]);
    timestamp->setText(dates[1]);
    hand->setText(dates[2]);
    gesture->setText(dates[3]); */
    fingers->setText("INDICE :" + dates[4] + " x ");
    fingers->append(dates[5] + " y ");
    fingers->append(dates[6] + " z ");
    fingers->append("POLLICE :" + dates[7] + " x ");
    fingers->append(dates[8] + " y ");
    fingers->append(dates[9] + " z ");
    fingers->append("Distanza tra le dita in x : "+dates[10]);
    fingers->append("Distanza tra le dita in y : "+dates[11]);
    fingers->append("Distanza tra le dita in z : "+dates[12]);
}

void LeapPaintWindow::connectEvent()
{
    if(!globalConnectionFlag)
    {
        emit connectLeap();
        rect->setFocus();
    }
    else
    {
        emit disconnectLeap();
    }
}

void LeapPaintWindow::drawSlots(QRect rect)
{
    mycanvas->draw(rect);
}

void LeapPaintWindow::changeShape(canvas::Shape shape)
{
    mycanvas->setShape(shape);
    emit updateShape((int)shape);
}

void LeapPaintWindow::lineEvent()
{
    changeShape(canvas::Line);
    line->setFocus();
}

void LeapPaintWindow::rectEvent()
{
    changeShape(canvas::Rect);
    rect->setFocus();
}

void LeapPaintWindow::roundedRectEvent()
{
    changeShape(canvas::RoundedRect);
    rounded_rect->setFocus();
}

void LeapPaintWindow::ellipseEvent()
{
    changeShape(canvas::Ellipse);
    ellipse->setFocus();
}

void LeapPaintWindow::pencilEvent()
{
    changeShape(canvas::Pencil);
    pencil->setFocus();
}

void LeapPaintWindow::fingerPosition(FingerVector* pos, bool touch, bool tap)
{
    mycanvas->setTapping(tap);
    mycanvas->setTouching(touch);
    mycanvas->setPos(pos);
}


void LeapPaintWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    newAct = new QAction(tr("&New File"), this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    brushColorAct = new QAction(tr("&Brush Color..."), this);
    connect(brushColorAct, SIGNAL(triggered()), this, SLOT(brushColor()));

    brushWidthAct = new QAction(tr("&Brush Width..."), this);
    connect(brushWidthAct, SIGNAL(triggered()), this, SLOT(brushWidth()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    brushTrasparentAct = new QAction(tr("&Brush Trasparent"), this);
    connect(brushTrasparentAct, SIGNAL(triggered()), this, SLOT(brushTrasparent()));

    brushActionGroup = new QActionGroup(this);
}

void LeapPaintWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    brushMenu = menuBar()->addMenu(tr("&Brush"));
    brushMenu->addAction(brushTrasparentAct);
    brushMenu->addAction(brushColorAct);
    brushMenu->addAction(brushWidthAct);
    brushMenu->addAction(penColorAct);
    brushMenu->addSeparator();

    menuBar()->addSeparator();

}

void LeapPaintWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        if (!mycanvas->openImage(fileName)) {
            QMessageBox::information(this, tr("Plug & Paint"), tr("Cannot load %1.").arg(fileName));
            return;
        }
        mycanvas->adjustSize();
    }
}

bool LeapPaintWindow::saveAs()
{
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath() + "/untitled.png");
    if (fileName.isEmpty()) {
        return false;
    } else {
        return mycanvas->saveImage(fileName, "png");
    }
}

void LeapPaintWindow::brushColor()
{
    const QColor newColor = QColorDialog::getColor(mycanvas->brushColor());
    if (newColor.isValid())
        mycanvas->setBrushColor(newColor);
}

void LeapPaintWindow::brushWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr("Basic LeaPaint"), tr("Selezione la dimensione del brush:"), mycanvas->brushWidth(), 1, 50, 1, &ok);
    if (ok)
        mycanvas->setBrushWidth(newWidth);
}

void LeapPaintWindow::penColor()
{
    const QColor newColor = QColorDialog::getColor(mycanvas->penColor());
    if (newColor.isValid())
        mycanvas->setPenColor(newColor);
}

void LeapPaintWindow::newFile()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Attenzione", "Sei sicuro di creare un nuovo file?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes){
        mycanvas->inizialize();
        rectEvent();
    }
}

void LeapPaintWindow::brushTrasparent()
{
    mycanvas->setBrushTrasparent();
}

void LeapPaintWindow::noHandValid()
{
    mycanvas->setPreviousVaraible(-1,-1);
    mycanvas->setAppVariable(-1,-1);
    mycanvas->setTapping(false);
    mycanvas->setTouching(false);
    mycanvas->update();
}
