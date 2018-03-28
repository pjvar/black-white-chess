#include "choosewidget.h"
#include "ui_choosewidget.h"
#include <QDesktopWidget> // for the center show

ChooseWidget::ChooseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseWidget)
{
    ui->setupUi(this);

    //给窗口去掉边框，设置窗口的flags
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags() );

    //设置透明背景
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setMinimumSize(800, 480);
    this->setMaximumSize(800, 480);

    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

   //this->setCursor(Qt::BlankCursor); // hide the cursor, for a8
   //this->showFullScreen();   // for a8

    // set button image
    ui->buttonExit->setImage(":/new/prefix1/image/exitchoose.png");
    ui->buttonPVM->setImage(":/new/prefix1/image/pvm.png");
    ui->buttonPVP->setImage(":/new/prefix1/image/pvp.png");
    ui->buttonExit->hide();
    ui->buttonPVM->hide();
    ui->buttonPVP->hide();

    // Firstly, show the welcome background
    ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/welcome.jpg"));
    this->movie = new QMovie(":/new/prefix1/image/choosewin.gif");

    connect(&this->timer, SIGNAL(timeout()), this, SLOT(dealMyTimer()));
    this->timer.start(1000);    // start the time

    // button connect
    connect(ui->buttonExit, SIGNAL(released()), this, SLOT(dealButtonExit()));
    connect(ui->buttonPVM, SIGNAL(released()), this, SLOT(dealButtonPVM()));
    connect(ui->buttonPVP, SIGNAL(released()), this, SLOT(dealButtonPVP()));

    this->playWin = new PlayWidget;
    this->myTime = 2;

    connect(this->playWin, SIGNAL(myCloseSignal()), this, SLOT(dealButtonExit()));
    //connect(this->playWin, SIGNAL(myGoBackSignal()), this, SLOT(show()));
    //connect(this->playWin, SIGNAL(myGoBackSignal()), this, SLOT(showFullScreen()));
    connect(this->playWin, SIGNAL(myGoBackSignal()), this, SLOT(goBackShow()));
}

ChooseWidget::~ChooseWidget()
{
    delete ui;
    delete this->movie;
    delete this->playWin;
}

void ChooseWidget::goBackShow()
{
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    show();
}

/*
 * slots, deal time
*/
void ChooseWidget::dealMyTimer()
{
    myTime--;

    if (0 == myTime)
    {
        ui->labelMaker->hide();

        this->timer.stop();     // stop the time
        ui->labelBackground->setMovie(this->movie);
        this->movie->start();   // start the movie

        // show the button
        ui->buttonExit->show();
        ui->buttonPVM->show();
        ui->buttonPVP->show();
    }
}

/*
 * exit button
*/
void ChooseWidget::dealButtonExit()
{
    if (true == this->isHidden())
    {
        QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
        move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

        this->show();
    }

    this->myTime = 2;
    this->movie->stop();
    this->timer.start(1000);

    ui->labelMaker->show();
    ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/welcome.jpg"));

    ui->buttonExit->hide();
    ui->buttonPVM->hide();
    ui->buttonPVP->hide();

}

/*
 * choose pvm mode
*/
void ChooseWidget::dealButtonPVM()
{
    //this->playWin->show();
    this->playWin->myShow();
    //this->playWin->showFullScreen();    // for a8
    this->hide();
    this->playWin->beginGame(false);
}

/*
 * choose pvp mode
*/
void ChooseWidget::dealButtonPVP()
{
    //this->playWin->show();
    this->playWin->myShow();
    this->hide();
    this->playWin->beginGame(true);
}

void ChooseWidget::mousePressEvent(QMouseEvent *e)
{
    //判断按下的是哪个键
    if(e->button() == Qt::LeftButton)
    {
        //求差值 = 当前屏幕坐标 - 窗口左上角坐标
        //x = e->globalPos() - this->geometry().topLeft();
        x = e->globalPos() - this->frameGeometry().topLeft();
    }
    else if(e->button() == Qt::RightButton)
    {
        //关闭当前窗口
        close();
    }
}

void ChooseWidget::mouseMoveEvent(QMouseEvent *e)
{
    //判断是不是左键拖动的窗口
    if(e->buttons() & Qt::LeftButton)
    {
        //让窗口跟着鼠标移动
        move(e->globalPos() - x);
    }
}
