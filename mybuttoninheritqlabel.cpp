#include "mybuttoninheritqlabel.h"

MyButtonInheritQLabel::MyButtonInheritQLabel(QWidget *parent) :
    QLabel(parent)
{
    // initialize the image path
    this->imagePath = ":/new/prefix1/image/start/1.png";
    this->uImagePath = "";

    this->isButtonEnabled = true; // the button is enabled

    // default setting;
    startxStep = 5;
    startyStep = 5;
    widthStep = 10;
    heightStep = 10;
}

/*
 * mouse press event
 * when mouse presses, the window will become a little bigger
*/
void MyButtonInheritQLabel::mousePressEvent(QMouseEvent *)
{
    if (false == this->isButtonEnabled)
    {
        return;
    }

    this->setGeometry(x()-startxStep, y()-startyStep, width()+widthStep, height()+heightStep);
    emit pressed();
}

/*
 * mouse release event
 * when mouse releases, the window will regain the same
*/
void MyButtonInheritQLabel::mouseReleaseEvent(QMouseEvent *)
{
    if (false == this->isButtonEnabled)
    {
        return;
    }

    this->setGeometry(x()+startxStep, y()+startyStep, width()-widthStep, height()-heightStep);
    emit released();
}

/*
 * set button eable or not
*/
void MyButtonInheritQLabel::setButtonEnable(const bool temp)
{
    this->isButtonEnabled = temp;
    this->setEnabled(temp);

    if (true == this->isButtonEnabled)
    {
        this->setPixmap(QPixmap(imagePath));
    }
    else
    {
        this->setPixmap(QPixmap(uImagePath));
    }
}

/*
 * set the image path
*/
void MyButtonInheritQLabel::setImage(const QString &str, const QString &ustr)
{
    // save the image path
    this->imagePath = str;
    this->uImagePath = ustr;

    if (true == this->isButtonEnabled)
    {
        this->setPixmap(QPixmap(str));
    }
    else
    {
        this->setPixmap(QPixmap(ustr));
    }
}

/*
* set change step
*/
void MyButtonInheritQLabel::setChangeStep(const int x, const int y, const int w, const int h)
{
    startxStep = x;
    startyStep = y;
    widthStep = w;
    heightStep = h;
}
