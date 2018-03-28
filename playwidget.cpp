#include "playwidget.h"
#include "ui_playwidget.h"
#include <QDesktopWidget> // for the center show

PlayWidget::PlayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayWidget)
{
    ui->setupUi(this);

    //给窗口去掉边框，设置窗口的flags
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags() );

    //设置透明背景
    this->setAttribute(Qt::WA_TranslucentBackground);

    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    this->setMinimumSize(800, 480);
    this->setMaximumSize(800, 480);

    //this->setCursor(Qt::BlankCursor); // hide the cursor, for a8

    // set button or label image
    ui->buttonRegret->setImage(":/new/prefix1/image/regret.png");
    ui->buttonGoBack->setImage(":/new/prefix1/image/goback.png");
    ui->buttonStop->setImage(":/new/prefix1/image/stop.png");
    ui->buttonExit->setImage(":/new/prefix1/image/exitplay.png");
    ui->buttonRedo->setImage(":/new/prefix1/image/redo.png");
    ui->buttonYes->setImage(":/new/prefix1/image/yes.png");
    ui->buttonNo->setImage(":/new/prefix1/image/no.png");
    ui->buttonGoon->setImage(":/new/prefix1/image/goon.png");
    ui->buttonGoBackWin->setImage(":/new/prefix1/image/gobackwin.png");

    connect(ui->buttonExit, SIGNAL(released()), this, SLOT(dealButtonExit()));
    connect(ui->buttonGoBack, SIGNAL(released()), this, SLOT(dealButtonGoBack()));
    connect(ui->buttonGoBackWin, SIGNAL(released()), this, SLOT(dealButtonGoBackWin()));
    connect(ui->buttonGoon, SIGNAL(released()), this, SLOT(dealButtonGoon()));
    connect(ui->buttonRedo, SIGNAL(released()), this, SLOT(dealButtonRedo()));
    connect(ui->buttonNo, SIGNAL(released()), this, SLOT(dealButtonNo()));
    connect(ui->buttonRegret, SIGNAL(released()), this, SLOT(dealButtonRegret()));
    connect(ui->buttonStop, SIGNAL(released()), this, SLOT(dealButtonStop()));
    connect(ui->buttonYes, SIGNAL(released()), this, SLOT(dealButtonYes()));

    connect(&this->timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    connect(&this->machineTime, SIGNAL(timeout()), this, SLOT(PVEmethod()));

    this->setButton(Begin);

    // initialize setting
    this->step = 0;
    this->gridNum = 0;
    this->start.setX(200);
    this->start.setY(56);
    this->gridWidth = 50;
    this->gridHeight = 41;
}

PlayWidget::~PlayWidget()
{
    delete ui;
}

void PlayWidget::myShow()
{
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->show();
}

/*
 * paint event
*/
void PlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":chessboard.jpg"));


        QPen myPen;
        myPen.setWidth(4);  // 通过画笔设置线宽
        myPen.setColor(Qt::black);  // 设置画笔颜色
        painter.setPen(myPen);      // 将笔交给画家

        //背景图
        painter.drawPixmap(start.x(),start.y(),this->gridWidth*8,this->gridHeight*8, QPixmap(":/board1.jpg"));

            // 画棋盘
           for(int i = 0; i <= 8; i++){
                 painter.drawLine(start.x(), start.y()+i*this->gridHeight,
                                              start.x()+8*gridWidth, start.y()+i*gridHeight);    // 画横线
                 painter.drawLine(start.x()+i*gridWidth, start.y(),
                                              start.x()+gridWidth*i, start.y()+gridHeight*8);    // 画竖线
            }
       /* painter.drawPixmap(startX+gridWidth*i, startY+gridHeight*j,
                                           gridWidth, gridHeight,QPixmap(this->blackPath));*/


    for(int i=0; i<gridNum; i++)
    {
        for(int j=0; j<gridNum; j++)
        {
            // 注意线宽的处理
            if(chess[step][i][j] == Black)
            {
                // 画黑子
                painter.drawPixmap(start.x()+gridWidth*i,start.y()+gridHeight*j,
                                   gridWidth,gridHeight,QPixmap(":/black.png"));
            }
            else if((chess[step][i][j]) == White)
            {
                // 画白子
                painter.drawPixmap(start.x()+gridWidth*i,start.y()+gridHeight*j,
                                   gridWidth,gridHeight,QPixmap(":/white.png"));
            }
        }
    }
}

/*
 * description: beign game, initialize some data
 *
 * mode: sign the play mode
*/
void PlayWidget::beginGame(bool mode)
{
    this->isPVP = mode;
    this->isRedo = true;

    this->currentRole = Black;//枚举

    ui->labelBlack->show();
    ui->labelWhite->hide();

    this->timeNum=21;
    this->step = 0;
    this->gridNum = 8;

    ui->lcdNumberBlack->display(2);
    ui->lcdNumberWhite->display(2);
    ui->lcdNumberTime->display(20);

    // initialize setting
    if (false == ui->buttonRegret->isHidden())
    {
        ui->buttonRegret->hide();
        ui->buttonRedo->hide();
    }

    if (false == this->timer.isActive())
    {
        this->timer.start(1000);
    }

    //this->timer.start(1000);

    this->initChessInfo();
    this->update();
}

void PlayWidget::initChessInfo()
{
    // 64个格全部标志为Empty，标志位是在ChessBoard定义的枚举常量
    memset((void *)chess[0], Empty, 64*sizeof(int));

    // 将棋盘中间的4个空间分别标志位2黑子2白子
    chess[0][3][3]= Black;   // a[3][3]
    chess[0][3][4]= White;   // a[3][4]
    chess[0][4][3]= White;   // a[4][3]
    chess[0][4][4]= Black;   // a[4][4]
}

// 时间的处理函数，每1S执行一次
void PlayWidget::timerSlot()
{
    this->timeNum--;          // 每次减1
    ui->lcdNumberTime->display(this->timeNum);    // 时间显示到LCD上

    // show the current role
    if (Black == this->currentRole)
    {
        (true == ui->labelBlack->isHidden())?ui->labelBlack->show():ui->labelBlack->hide();
    }
    else
    {
        (true == ui->labelWhite->isHidden())?ui->labelWhite->show():ui->labelWhite->hide();

    }

    if(0 == this->timeNum)   // 时间到就改变棋子的角色
    {
        // 如果人机对战模式，改变isRedo的标志位，否则机器不会自动下
        if (false == this->isPVP && true == this->isRedo)
        {
            this->isRedo = false;
        }

        changeRole();       // 改变棋子的角色
    }
}

void PlayWidget::mousePressEvent(QMouseEvent *e)
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

    if (false == this->isPVP && White == this->currentRole)
    {
        return;
    }

    // 获取点击点x,y的坐标
    int x = e->x();
    int y = e->y();

    // 要保证点击点在棋盘范围里面
    if((x > start.x()) && (x < (start.x() + gridWidth*gridNum))&&
       (y > start.y())&&(y < (start.y() + gridHeight*gridNum)))
    {
        // 将坐标值转换为相对棋盘的数组下标
        // 类似于a[i][j]的i和j
        x = (x - start.x())/gridWidth;
        y = (y - start.y())/gridHeight;

        memcpy((void *)chess[step+1], (void *)chess[step], 64*sizeof(int));

        // 如果能吃子，num大于0
        // 吃子规则的参数：棋盘数组坐标位置、格数、棋子状态(chess[step+1])、棋子的优先权
        if(judgeRule(x,y,gridNum, (void *)chess[step+1], currentRole) > 0)
        {
            //this->failFlag=0;
            step++;         // 加1，这时step已经改变了，多了1
            if (true == ui->buttonRedo->isHidden())
            {
                ui->buttonRedo->show();
                ui->buttonRegret->show();
                this->isRedo = false;
            }

            changeRole();   // 改变棋子的优先权
            this->update();  // 更新棋盘，刷图
            this->showScore();      // 显示分数
        }
    }
}

// 人机对战函数
void PlayWidget::PVEmethod()
{
    // 如果按下重来，按钮，人机对战函数就进不去
    if (true == this->isRedo)
    {
        return;
    }

    this->machineTime.stop();   // 机器的时间先停止
    int num = 0;
    int max = 0;
    int px;
    int py;

    // 判断棋盘上哪些位置是能够吃子的
    for(int i= 0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess[step][i][j] == Empty)  // 为棋盘空时，才能下子
            {   // 判断能不能吃子，只是判断，不改变棋盘状态，注意false参数
                num = judgeRule(i,j,gridNum,(void*)chess[step],currentRole,false);
                if(num > max)   // 挑一个吃子最多的空格，并把坐标保存起来
                {
                    max = num;  // 改变max的值
                    px = i;
                    py = j;
                }
            }
        }
    }

    if(max == 0)    // 到这一步max为0，说明方格里都不能吃子，这是就改变棋子的角色别
    {
        this->changeRole(); // 改变棋子的角色
        return ;            // 中断此函数
    }

    memcpy((void *)chess[step+1], (void *)chess[step], 64*sizeof(int));

    // 如果能吃子，num大于0
    // 吃子规则的参数：棋盘数组坐标位置(这个坐标肯定能吃子)、格数、棋子状态(chess[step+1])、棋子的优先权
    if(judgeRule(px,py,gridNum, (void *)chess[step+1],currentRole) > 0)
    {
        //failFlag = 0;
        step++;         // 加1，这时step已经改变了，多了1
        changeRole();   // 改变棋子的优先权
        this->update();  // 更新状态
        this->showScore();      // 显示分数
    }
}

// 改变棋子的角色
void PlayWidget::changeRole()
{
    this->timeNum=21;     // 时间设置为21S

    // 如果当前为白子，就换为黑子
    if(this->currentRole == White)
    {
        this->currentRole = Black;
        ui->labelBlack->show();
        ui->labelWhite->hide();
    }
    else
    {
        this->currentRole = White;
        ui->labelWhite->show();
        ui->labelBlack->hide();
    }


    // 如果是人机对战模式，并且到白子下了
    if(this->isPVP == false && this->currentRole == White)
    {
        this->machineTime.start(1000);  // 启动机器的时间
    }
}

// 显示分数函数
void PlayWidget::showScore()
{
   int blackNum=0;
   int whiteNum=0;
   // 棋盘的棋子遍历一遍，累计黑白棋各自的个数
    for(int i=0;i<gridNum;i++)
    {
        for(int j=0;j<gridNum;j++)
        {
            if(chess[step][i][j] == White)      // 白子
              whiteNum++;
            else if(chess[step][i][j] == Black) // 黑子
               blackNum++;
        }
    }
    // 分别显示到对应的LCD上
    ui->lcdNumberBlack->display(blackNum);
    ui->lcdNumberWhite->display(whiteNum);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (judgeRule(i,j,8, (void *)chess[step],Black, false) > 0
                || judgeRule(i,j,8, (void *)chess[step],White, false) > 0)
            {
                return;
            }
        }
    }

    // show the win image
    if(blackNum > whiteNum)
    {
        ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/blackwin.jpg"));
        this->setButton(Judge);
    }
    else if (blackNum < whiteNum)
    {
        ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/whitewin.jpg"));
        this->setButton(Judge);
    }
    else
    {
        ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/same.jpg"));
        this->setButton(Judge);
    }
}

// 吃子规则的参数：棋盘数组坐标位置(x y)、横着或竖着的格数(gridNum)、
// 棋子状态(chess)、棋子的优先权(注意其变量的写法)、eatChess默认为true
int PlayWidget::judgeRule(int x, int y, int gridNum, void *chess, GridState currentRole, bool eatChess)
{
    // 棋盘的八个方向
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int temp_x = x, temp_y = y;             // 临时保存棋盘数组坐标位置
    int i = 0, eatNum = 0;                  // 初始化数据
    typedef int (*p)[gridNum];             // 自定义类型
    p chessBoard = p(chess);                // 类型转换

    if(chessBoard[temp_x][temp_y] != Empty)  // 如果此方格内已有棋子，返回;
        return 0;

    // 棋盘的8个方向
    for(i = 0 ; i <8; i++)
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];	// 准备判断相邻棋子
        // 如果没有出界，且相邻棋子是对方棋子，才有吃子的可能．
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            && (chessBoard[temp_x][temp_y] != currentRole) && (chessBoard[temp_x][temp_y] != Empty) )
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];	            // 继续判断下一个，向前走一步
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                if(chessBoard[temp_x][temp_y] == Empty) // 遇到空位跳出
                        break;
                if(chessBoard[temp_x][temp_y] == currentRole)       // 找到自己的棋子，代表可以吃子
                {
                    if(eatChess == true)                            // 确定吃子
                    {

                        chessBoard[x][y] = currentRole;          // 开始点标志为自己的棋子
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];// 后退一步
                        while((temp_x != x )||(temp_y != y))	 // 只要没有回到开始的位置就执行
                        {
                            chessBoard[temp_x][temp_y] = currentRole;   // 标志为自己的棋子
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 继续后退一步
                            eatNum ++;  // 累计
                        }
                    }
                    else    // 不吃子，只是判断这个位置能不能吃子
                    {

                        temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 后退一步
                        while((temp_x != x )||(temp_y != y))        // 只计算可以吃子的个数
                        {
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];// 继续后退一步
                            eatNum ++;
                        }
                    }
                    break;  // 跳出循环
                }// 没有找到自己的棋子，就向前走一步
                temp_x += dir[i][0]; temp_y += dir[i][1];   // 向前走一步
            }
        }// 如果这个方向不能吃子，就换一个方向
        temp_x = x; temp_y = y;
    }

    return eatNum;              // 返回能吃子的个数
}

/*
 * exit button
*/
void PlayWidget::dealButtonExit()
{
    ui->lcdNumberTime->display(20);
    this->timer.stop();
    this->hide();
    emit this->myCloseSignal();
}

/*
 * go back the welcome widget button
*/
void PlayWidget::dealButtonGoBack()
{
    ui->lcdNumberTime->display(20);
    this->timer.stop();
    emit this->myGoBackSignal();
    this->hide();
}

void PlayWidget::dealButtonGoBackWin()
{
    this->dealButtonGoBack();
    this->setButton(ChooseForJudge);
}

void PlayWidget::dealButtonGoon()
{
    this->beginGame(this->isPVP);
    this->setButton(ChooseForJudge);
}

void PlayWidget::dealButtonNo()
{
    this->timer.start(1000);
    this->setButton(ChooseForPause);
}

void PlayWidget::dealButtonRedo()
{
    this->beginGame(this->isPVP);
}

void PlayWidget::dealButtonRegret()
{
    // 如果是人人对战，并且已经下子了
    if(this->isPVP == true)
    {
        step--;             // 推一步

        if (0 == this->step || 0 > this->step)
        {
            qDebug() << "test";
            this->beginGame(this->isPVP);
            return;
        }

        this->changeRole();
        this->showScore();
        this->update();  // 更新棋盘
    }
    else if(this->isPVP == false) // 如果是人机对战
    {
        step = step -2;                               // 退两步

        if (0 == this->step || 0 > this->step)
        {
            this->beginGame(this->isPVP);
            return;
        }

        this->timeNum = 21;
        this->showScore();
        this->update();  // 更新棋盘
    }
}

/*
 * pause button
*/
void PlayWidget::dealButtonStop()
{
    this->timer.stop();
    this->setButton(Pause);
}

void PlayWidget::dealButtonYes()
{
    this->setButton(ChooseForPause);
    this->dealButtonGoBack();
}

/*
 * description: set button state
 *
 * state: sign button state
*/
void PlayWidget::setButton(ButtonState state)
{
    if (Begin == state)
    {
        ui->labelBackground->hide();
        ui->buttonGoon->hide();
        ui->buttonGoBackWin->hide();
        ui->buttonYes->hide();
        ui->buttonNo->hide();
        ui->labelBlack->hide();
        ui->labelWhite->hide();
    }
    if (Pause == state)
    {
        ui->labelBackground->show();
        ui->labelBackground->setPixmap(QPixmap(":/new/prefix1/image/pause.jpg"));
        ui->buttonYes->show();
        ui->buttonNo->show();

        ui->buttonGoBack->hide();
        ui->buttonExit->hide();
        ui->buttonStop->hide();
        if (0 < this->step)
        {
            ui->buttonRegret->hide();
            ui->buttonRedo->hide();
        }

        ui->lcdNumberBlack->hide();
        ui->lcdNumberTime->hide();
        ui->lcdNumberWhite->hide();
        ui->labelBlack->hide();
        ui->labelWhite->hide();
    }
    if (ChooseForPause == state)
    {
        ui->labelBackground->hide();
        ui->buttonYes->hide();
        ui->buttonNo->hide();

        ui->buttonGoBack->show();
        ui->buttonExit->show();
        ui->buttonStop->show();
        if (0 < this->step)
        {
            ui->buttonRegret->show();
            ui->buttonRedo->show();
        }

        ui->lcdNumberBlack->show();
        ui->lcdNumberTime->show();
        ui->lcdNumberWhite->show();
        if (Black == this->currentRole)
        {
            ui->labelBlack->show();
        }
        else
        {
            ui->labelWhite->show();
        }
    }
    if (Judge == state)
    {
        ui->labelBackground->show();
        ui->buttonGoon->show();
        ui->buttonGoBackWin->show();

        ui->buttonGoBack->hide();
        ui->buttonExit->hide();
        ui->buttonStop->hide();
        ui->buttonRegret->hide();
        ui->buttonRedo->hide();

        ui->lcdNumberBlack->hide();
        ui->lcdNumberTime->hide();
        ui->lcdNumberWhite->hide();
        ui->labelBlack->hide();
        ui->labelWhite->hide();
        this->timer.stop();
    }
    if (ChooseForJudge == state)
    {
        ui->labelBackground->hide();
        ui->buttonGoon->hide();
        ui->buttonGoBackWin->hide();

        ui->buttonGoBack->show();
        ui->buttonExit->show();
        ui->buttonStop->show();

        ui->lcdNumberBlack->show();
        ui->lcdNumberTime->show();
        ui->lcdNumberWhite->show();

        if (Black == this->currentRole)
        {
            ui->labelBlack->show();
        }
        else
        {
            ui->labelWhite->show();
        }
    }
}

void PlayWidget::mouseMoveEvent(QMouseEvent *e)
{
    //判断是不是左键拖动的窗口
    if(e->buttons() & Qt::LeftButton)
    {
        //让窗口跟着鼠标移动
        move(e->globalPos() - x);
    }
}
