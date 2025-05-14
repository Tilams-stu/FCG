#include "gameserver.h"
#include <QApplication>
#include <QBoxLayout>
#include <QTextEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget widget;
    widget.setWindowTitle("飞行棋服务器");
    widget.show();
    /*QTextEdit *logView = new QTextEdit(&widget);
    logView->setReadOnly(true);
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    layout->addWidget(logView);

    // 重定向qInfo输出到界面
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg){
        Q_UNUSED(context)
        logView->append(QString("[%1] %2").arg(QTime::currentTime().toString()).arg(msg));
    });
*/
    GameServer server(&widget);
    server.startServer();

    return a.exec();
}
