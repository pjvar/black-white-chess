#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QDebug>

namespace Ui {
    class PlayWidget;
}

class PlayWidget : public QWidget
{
    Q_OBJECT

public:
    // sign button state
    enum ButtonState{Begin, Pause, Judge, ChooseForPause, ChooseForJudge};
    // Empty标志为空，Black标志为黑子，White标志为白子
    enum GridState{Empty,Black, White};

    explicit PlayWidget(QWidget *parent = 0);
    ~PlayWidget();
    void setButton(ButtonState state);  // set button state
    void beginGame(bool);               // begin game
    void initChessInfo();
    void changeRole();
    int judgeRule(int, int, int , void *, GridState,bool eatChess=true);
    void showScore();

    void myShow();

protected:
    void paintEvent(QPaintEvent *);     // paint event
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

signals:
    void myCloseSignal();
    void myGoBackSignal();

public slots:
    void dealButtonRegret();
    void dealButtonGoBack();
    void dealButtonStop();
    void dealButtonExit();
    void dealButtonRedo();
    void dealButtonYes();
    void dealButtonNo();
    void dealButtonGoon();
    void dealButtonGoBackWin();
    void timerSlot();   // 倒计时的槽函数
    void PVEmethod();   // 机器的槽函数

private:
    Ui::PlayWidget *ui;
    int step;          // sign chess step
    bool isPVP;        // is pvp mode or pvm mode
    QPoint start;
    int gridWidth, gridHeight;
    int chess[64][8][8];    // 三维数组，方便标志棋盘状态
    int gridNum;
    GridState currentRole;
    int timeNum;
    bool isRedo;
    QTimer timer;           // 倒计时
    QTimer machineTime;     // 机器的时间，延时作用

    QPoint x;
};

#endif // PLAYWIDGET_H
