#include "mainview.h"

#include <QApplication>

#include <view/connectdialog.h>
#include <model/gamemodel.h>
#include <controller/gamecontroller.h>

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);

    GameModel model;
    ConnectDialog dialog;
    if(dialog.exec() == QDialog::Accepted){
        QString host = dialog.getHost();
        int port = dialog.getPort();
        QString username = dialog.getUsername();

        GameController* controller= new GameController(&model,host,port);
        MainView* mainView = new MainView(controller,username);

        controller->setParent(mainView);
        controller->setView(mainView);
        mainView->show();
    }
    else {
        return 0;
    }
    return a.exec();
}
