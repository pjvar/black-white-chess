#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMovie>
#include "playwidget.h"

namespace Ui {
    class ChooseWidget;
}

class ChooseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWidget(QWidget *parent = 0);
    ~ChooseWidget();

public slots:
    void dealMyTimer(); // deal time

    // button slots
    void dealButtonExit();
    void dealButtonPVM();
    void dealButtonPVP();
    void goBackShow();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    Ui::ChooseWidget *ui;
    PlayWidget *playWin;
    QTimer timer;       // timer, for show gif
    QMovie *movie;      // for start gif
    int myTime;

    QPoint x;

};

#endif // CHOOSEWIDGET_H
