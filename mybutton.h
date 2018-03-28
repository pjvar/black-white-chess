#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QEvent>
#include <QString>

/*
* when press or release, the window will change(become bigger or smaller)
*/
class MyButton : public QWidget
{
    Q_OBJECT

public:
    explicit MyButton(QWidget *parent = 0);
    void setImage(const QString &str = "");   // set the image

protected:
    void paintEvent(QPaintEvent *);         // paint background picture
    void mousePressEvent(QMouseEvent *);    // mouse press event
    void mouseReleaseEvent(QMouseEvent *);  // mouse release event

private:
    QString imagePath;  // save the image path
    int startxStep, startyStep, widthStep, heightStep;

signals:
    void pressed();
    void released();

public slots:

};

#endif // MYBUTTON_H
