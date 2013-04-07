/********************************************************************************
** Form generated from reading UI file 'qwmain.ui'
**
** Created: Sun Apr 7 14:40:21 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QWMAIN_H
#define UI_QWMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QWMain
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextBrowser *taConsole;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbLoad;
    QSpacerItem *horizontalSpacer;
    QCheckBox *cbDebug;
    QPushButton *pbAssemble;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QWMain)
    {
        if (QWMain->objectName().isEmpty())
            QWMain->setObjectName(QString::fromUtf8("QWMain"));
        QWMain->resize(528, 315);
        centralWidget = new QWidget(QWMain);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        taConsole = new QTextBrowser(centralWidget);
        taConsole->setObjectName(QString::fromUtf8("taConsole"));

        verticalLayout->addWidget(taConsole);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbLoad = new QPushButton(centralWidget);
        pbLoad->setObjectName(QString::fromUtf8("pbLoad"));

        horizontalLayout->addWidget(pbLoad);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cbDebug = new QCheckBox(centralWidget);
        cbDebug->setObjectName(QString::fromUtf8("cbDebug"));
        cbDebug->setEnabled(false);

        horizontalLayout->addWidget(cbDebug);

        pbAssemble = new QPushButton(centralWidget);
        pbAssemble->setObjectName(QString::fromUtf8("pbAssemble"));
        pbAssemble->setEnabled(false);

        horizontalLayout->addWidget(pbAssemble);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        QWMain->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QWMain);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QWMain->setStatusBar(statusBar);

        retranslateUi(QWMain);

        QMetaObject::connectSlotsByName(QWMain);
    } // setupUi

    void retranslateUi(QMainWindow *QWMain)
    {
        QWMain->setWindowTitle(QApplication::translate("QWMain", "Libra Assembler -- GUI", 0, QApplication::UnicodeUTF8));
        pbLoad->setText(QApplication::translate("QWMain", "Load", 0, QApplication::UnicodeUTF8));
        cbDebug->setText(QApplication::translate("QWMain", "Generate Debug File", 0, QApplication::UnicodeUTF8));
        pbAssemble->setText(QApplication::translate("QWMain", "Assemble", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QWMain: public Ui_QWMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QWMAIN_H
