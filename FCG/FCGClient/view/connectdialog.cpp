#include "connectdialog.h"


ConnectDialog::ConnectDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle(tr("连接到服务器"));
    setModal(true);
}

QString ConnectDialog::getHost() const
{
    return hostEdit->text().trimmed();
}

int ConnectDialog::getPort() const
{
    return portEdit->text().toInt();
}

QString ConnectDialog::getUsername() const
{
    return usernameEdit->text().trimmed();
}

void ConnectDialog::setupUI()
{
    hostEdit = new QLineEdit(this);
    hostEdit->setText("localhost");
    hostEdit->setPlaceholderText(tr("Server IP or hostname"));

    portEdit = new QLineEdit(this);
    portEdit->setText("12345");
    portEdit->setValidator(new QIntValidator(1,65535,this));
    portEdit->setPlaceholderText(tr("Port number (1-65535)"));

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText(tr("Your nickname"));

    connectButton = new QPushButton(tr("Connect"),this);
    cancelButton = new QPushButton(tr("Cancel"),this);

    //布局
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(tr("Server:"),hostEdit);
    formLayout->addRow(tr("Port:"),portEdit);
    formLayout->addRow(tr("Username:"),usernameEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(connectButton, &QPushButton::clicked,this,&ConnectDialog::onConnectClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ConnectDialog::onConnectClicked()
{
    if(validateInput()){
        accept();
    }
    else{
        QMessageBox::warning(this,
                             tr("Invalid Input"),
                             tr("Please check:\n"
                                "- Valid host address\n"
                                "- Port between 1-65535\n"
                                "- Non-empty username"));
        hostEdit->setText("localhost");
        portEdit->setText("12345");
        usernameEdit->clear();
        hostEdit->setFocus();
    }
}
bool ConnectDialog::validateInput() const
{
    return !hostEdit->text().isEmpty() && portEdit->hasAcceptableInput() && !usernameEdit->text().trimmed().isEmpty() ;
}

