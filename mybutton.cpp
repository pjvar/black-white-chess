#include "mybutton.h"

MyButton::MyButton(QWidget *parent) :
    QWidget(parent)
{
    // initialize the image path
    this->imagePath = ":/new/prefix1/image/start/1.png";

    // default setting;
    startxStep = 8;
    startyStep = 8;
    widthStep = 16;
    heightStep = 16;
}

/*
 * paint event, paint background image
*/
void MyButton::paintEvent(QPaintEvent *)
{
    QPainter paint(this); // create a painter
    paint.drawPixmap(0, 0, width(), height(), QPixmap(imagePath));
}

/*
 * set the image path
*/
void MyButton::setImage(const QString &str)
{
    // save the image path
    this->imagePath = str;
    this->update();         // update the window
}

/*
 * mouse press event
 * when mouse presses, the window will become a little bigger
*/
void MyButton::mousePressEvent(QMouseEvent *)
{
    this->setGeometry(x()-startxStep, y()-startyStep, width()+widthStep, height()+heightStep);
    emit pressed();
}

/*
 * mouse release event
 * when mouse releases, the window will regain the same
*/
void MyButton::mouseReleaseEvent(QMouseEvent *)
{

    this->setGeometry(x()+startxStep, y()+startyStep, width()-widthStep, height()-heightStep);
    emit released();
}
