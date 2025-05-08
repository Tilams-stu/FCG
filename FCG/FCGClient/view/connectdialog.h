#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>

class ConnectDialog : public QDialog
{

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    QString getHost() const;
    int getPort() const;
    QString getUsername() const;

private:
    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *usernameEdit;
    QPushButton *connectButton;
    QPushButton *cancelButton;

    void setupUI();
    bool validateInput() const;
    void onConnectClicked();
};

#endif // CONNECTDIALOG_H
