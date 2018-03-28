#include <QApplication>
#include <QDesktopWidget> // for the center show
#include "choosewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChooseWidget w;
    w.show();

     w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);

    return a.exec();
}
