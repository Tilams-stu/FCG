/********************************************************************************
** Form generated from reading UI file 'mainview.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEW_H
#define UI_MAINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainView
{
public:

    void setupUi(QWidget *MainView)
    {
        if (MainView->objectName().isEmpty())
            MainView->setObjectName("MainView");
        MainView->resize(1200, 800);
        MainView->setMinimumSize(QSize(1200, 800));
        MainView->setMaximumSize(QSize(1200, 800));

        retranslateUi(MainView);

        QMetaObject::connectSlotsByName(MainView);
    } // setupUi

    void retranslateUi(QWidget *MainView)
    {
        MainView->setWindowTitle(QCoreApplication::translate("MainView", "MainView", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainView: public Ui_MainView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEW_H
