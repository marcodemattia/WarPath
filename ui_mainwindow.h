/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 19. Dec 12:15:33 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionSave;
    QAction *actionSaveToPdf;
    QAction *actionPrint_2;
    QAction *actionPrint;
    QAction *actionLoad;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_23;
    QComboBox *listTypeSelectionComboBox;
    QComboBox *listSelectionComboBox;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *horizontalSpacer_18;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_2;
    QListView *fullList;
    QLabel *label_3;
    QTableView *mainList;
    QVBoxLayout *verticalLayout_21;
    QSpacerItem *verticalSpacer_17;
    QSpacerItem *horizontalSpacer_13;
    QToolButton *upButton;
    QToolButton *downButton;
    QSpacerItem *verticalSpacer_18;
    QVBoxLayout *verticalLayout_22;
    QSpacerItem *verticalSpacer_19;
    QTableView *optionsTable;
    QSpacerItem *verticalSpacer_20;
    QTabWidget *optionsTabWidget;
    QWidget *tab_13;
    QVBoxLayout *verticalLayout_23;
    QScrollArea *optionsScrollArea;
    QWidget *scrollAreaWidgetContents_5;
    QVBoxLayout *verticalLayout_24;
    QGroupBox *optionsGroup;
    QVBoxLayout *verticalLayout_25;
    QSpacerItem *verticalSpacer_21;
    QSpacerItem *verticalSpacer_22;
    QSpacerItem *horizontalSpacer_14;
    QWidget *tab_14;
    QVBoxLayout *verticalLayout_26;
    QScrollArea *specialOptionsScrollArea;
    QWidget *scrollAreaWidgetContents_6;
    QVBoxLayout *verticalLayout_27;
    QGroupBox *specialOptionsGroup;
    QVBoxLayout *verticalLayout_28;
    QSpacerItem *verticalSpacer_23;
    QSpacerItem *verticalSpacer_24;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label;
    QLCDNumber *lcdTotal;
    QSpacerItem *horizontalSpacer_16;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuWarPath;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(843, 620);
        MainWindowClass->setStyleSheet(QString::fromUtf8(""));
        actionSave = new QAction(MainWindowClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSaveToPdf = new QAction(MainWindowClass);
        actionSaveToPdf->setObjectName(QString::fromUtf8("actionSaveToPdf"));
        actionPrint_2 = new QAction(MainWindowClass);
        actionPrint_2->setObjectName(QString::fromUtf8("actionPrint_2"));
        actionPrint = new QAction(MainWindowClass);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionLoad = new QAction(MainWindowClass);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        listTypeSelectionComboBox = new QComboBox(centralWidget);
        listTypeSelectionComboBox->setObjectName(QString::fromUtf8("listTypeSelectionComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listTypeSelectionComboBox->sizePolicy().hasHeightForWidth());
        listTypeSelectionComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_23->addWidget(listTypeSelectionComboBox);

        listSelectionComboBox = new QComboBox(centralWidget);
        listSelectionComboBox->setObjectName(QString::fromUtf8("listSelectionComboBox"));
        sizePolicy.setHeightForWidth(listSelectionComboBox->sizePolicy().hasHeightForWidth());
        listSelectionComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_23->addWidget(listSelectionComboBox);

        horizontalSpacer_17 = new QSpacerItem(248, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_17);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_18);


        verticalLayout->addLayout(horizontalLayout_23);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAutoFillBackground(false);
        label_2->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_20->addWidget(label_2);

        fullList = new QListView(centralWidget);
        fullList->setObjectName(QString::fromUtf8("fullList"));
        fullList->setStyleSheet(QString::fromUtf8(""));
        fullList->setResizeMode(QListView::Fixed);

        verticalLayout_20->addWidget(fullList);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAutoFillBackground(false);
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 200);"));

        verticalLayout_20->addWidget(label_3);

        mainList = new QTableView(centralWidget);
        mainList->setObjectName(QString::fromUtf8("mainList"));
        mainList->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_20->addWidget(mainList);


        horizontalLayout_8->addLayout(verticalLayout_20);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_21->addItem(verticalSpacer_17);

        horizontalSpacer_13 = new QSpacerItem(13, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_21->addItem(horizontalSpacer_13);

        upButton = new QToolButton(centralWidget);
        upButton->setObjectName(QString::fromUtf8("upButton"));
        upButton->setArrowType(Qt::UpArrow);

        verticalLayout_21->addWidget(upButton);

        downButton = new QToolButton(centralWidget);
        downButton->setObjectName(QString::fromUtf8("downButton"));
        downButton->setArrowType(Qt::DownArrow);

        verticalLayout_21->addWidget(downButton);

        verticalSpacer_18 = new QSpacerItem(20, 160, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_21->addItem(verticalSpacer_18);


        horizontalLayout_8->addLayout(verticalLayout_21);


        horizontalLayout_7->addLayout(horizontalLayout_8);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        verticalSpacer_19 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_22->addItem(verticalSpacer_19);

        optionsTable = new QTableView(centralWidget);
        optionsTable->setObjectName(QString::fromUtf8("optionsTable"));
        optionsTable->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_22->addWidget(optionsTable);

        verticalSpacer_20 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_22->addItem(verticalSpacer_20);

        optionsTabWidget = new QTabWidget(centralWidget);
        optionsTabWidget->setObjectName(QString::fromUtf8("optionsTabWidget"));
        optionsTabWidget->setAutoFillBackground(false);
        optionsTabWidget->setStyleSheet(QString::fromUtf8(""));
        tab_13 = new QWidget();
        tab_13->setObjectName(QString::fromUtf8("tab_13"));
        tab_13->setAutoFillBackground(false);
        tab_13->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_23 = new QVBoxLayout(tab_13);
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setContentsMargins(9, 9, 9, 9);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        optionsScrollArea = new QScrollArea(tab_13);
        optionsScrollArea->setObjectName(QString::fromUtf8("optionsScrollArea"));
        optionsScrollArea->setAutoFillBackground(false);
        optionsScrollArea->setStyleSheet(QString::fromUtf8(""));
        optionsScrollArea->setFrameShape(QFrame::StyledPanel);
        optionsScrollArea->setLineWidth(0);
        optionsScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 379, 178));
        scrollAreaWidgetContents_5->setAutoFillBackground(true);
        scrollAreaWidgetContents_5->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_24 = new QVBoxLayout(scrollAreaWidgetContents_5);
        verticalLayout_24->setSpacing(0);
        verticalLayout_24->setContentsMargins(0, 0, 0, 0);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        optionsGroup = new QGroupBox(scrollAreaWidgetContents_5);
        optionsGroup->setObjectName(QString::fromUtf8("optionsGroup"));
        optionsGroup->setAutoFillBackground(false);
        optionsGroup->setStyleSheet(QString::fromUtf8(""));
        optionsGroup->setFlat(false);
        optionsGroup->setCheckable(false);
        verticalLayout_25 = new QVBoxLayout(optionsGroup);
        verticalLayout_25->setSpacing(6);
        verticalLayout_25->setContentsMargins(11, 11, 11, 11);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        verticalSpacer_21 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_25->addItem(verticalSpacer_21);

        verticalSpacer_22 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_25->addItem(verticalSpacer_22);

        horizontalSpacer_14 = new QSpacerItem(338, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_25->addItem(horizontalSpacer_14);


        verticalLayout_24->addWidget(optionsGroup);

        optionsScrollArea->setWidget(scrollAreaWidgetContents_5);

        verticalLayout_23->addWidget(optionsScrollArea);

        optionsTabWidget->addTab(tab_13, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QString::fromUtf8("tab_14"));
        tab_14->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_26 = new QVBoxLayout(tab_14);
        verticalLayout_26->setSpacing(6);
        verticalLayout_26->setContentsMargins(11, 11, 11, 11);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        verticalLayout_26->setContentsMargins(-1, -1, -1, 9);
        specialOptionsScrollArea = new QScrollArea(tab_14);
        specialOptionsScrollArea->setObjectName(QString::fromUtf8("specialOptionsScrollArea"));
        specialOptionsScrollArea->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        specialOptionsScrollArea->setLineWidth(0);
        specialOptionsScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_6 = new QWidget();
        scrollAreaWidgetContents_6->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_6"));
        scrollAreaWidgetContents_6->setGeometry(QRect(0, 0, 98, 142));
        scrollAreaWidgetContents_6->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        verticalLayout_27 = new QVBoxLayout(scrollAreaWidgetContents_6);
        verticalLayout_27->setSpacing(0);
        verticalLayout_27->setContentsMargins(0, 0, 0, 0);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        specialOptionsGroup = new QGroupBox(scrollAreaWidgetContents_6);
        specialOptionsGroup->setObjectName(QString::fromUtf8("specialOptionsGroup"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(specialOptionsGroup->sizePolicy().hasHeightForWidth());
        specialOptionsGroup->setSizePolicy(sizePolicy1);
        specialOptionsGroup->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        specialOptionsGroup->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        specialOptionsGroup->setFlat(false);
        verticalLayout_28 = new QVBoxLayout(specialOptionsGroup);
        verticalLayout_28->setSpacing(6);
        verticalLayout_28->setContentsMargins(9, 9, 9, 9);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        verticalSpacer_23 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_28->addItem(verticalSpacer_23);

        verticalSpacer_24 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_28->addItem(verticalSpacer_24);

        horizontalSpacer_15 = new QSpacerItem(338, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_28->addItem(horizontalSpacer_15);


        verticalLayout_27->addWidget(specialOptionsGroup);

        specialOptionsScrollArea->setWidget(scrollAreaWidgetContents_6);

        verticalLayout_26->addWidget(specialOptionsScrollArea);

        optionsTabWidget->addTab(tab_14, QString());

        verticalLayout_22->addWidget(optionsTabWidget);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_9->addWidget(label);

        lcdTotal = new QLCDNumber(centralWidget);
        lcdTotal->setObjectName(QString::fromUtf8("lcdTotal"));
        lcdTotal->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_9->addWidget(lcdTotal);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_16);


        verticalLayout_22->addLayout(horizontalLayout_9);


        horizontalLayout_7->addLayout(verticalLayout_22);


        verticalLayout->addLayout(horizontalLayout_7);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 843, 21));
        menuWarPath = new QMenu(menuBar);
        menuWarPath->setObjectName(QString::fromUtf8("menuWarPath"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuWarPath->menuAction());
        menuWarPath->addAction(actionLoad);
        menuWarPath->addAction(actionSave);
        menuWarPath->addAction(actionSaveToPdf);
        menuWarPath->addAction(actionPrint);

        retranslateUi(MainWindowClass);

        optionsTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "WarPath", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindowClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSaveToPdf->setText(QApplication::translate("MainWindowClass", "Save to pdf", 0, QApplication::UnicodeUTF8));
        actionPrint_2->setText(QApplication::translate("MainWindowClass", "Print", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("MainWindowClass", "Print", 0, QApplication::UnicodeUTF8));
        actionLoad->setText(QApplication::translate("MainWindowClass", "Load", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindowClass", "Item Selection", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindowClass", "Selected Items", 0, QApplication::UnicodeUTF8));
        upButton->setText(QString());
        downButton->setText(QString());
        optionsGroup->setTitle(QString());
        optionsTabWidget->setTabText(optionsTabWidget->indexOf(tab_13), QApplication::translate("MainWindowClass", "Options", 0, QApplication::UnicodeUTF8));
        specialOptionsGroup->setTitle(QString());
        optionsTabWidget->setTabText(optionsTabWidget->indexOf(tab_14), QApplication::translate("MainWindowClass", "Special Options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindowClass", "Total", 0, QApplication::UnicodeUTF8));
        menuWarPath->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
