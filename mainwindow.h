// Copyright 2009 Marco De Mattia, destroyar0@gmail.com
//
// This file is part of WarPath.
//
// WarPath is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// WarPath is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WarPath.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QPainter>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include "datamodel.h"
#include "optionsdatamodel.h"
#include "data.h"
#include "checkbox.h"
#include "qevent.h"
#include "spinboxdelegate.h"
#include "spinbox.h"
#include "button.h"
#include <algorithm>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // void do_mainList_clicked(const QModelIndex & index);
    void clickedMainList(const QModelIndex & index);
    void clickedMainList(const int row);
    void clickedOptionsTable(const QModelIndex & index);
    void clickedSpinBox(const int row);
    void clickedList(const int row, const QMap<QString, OptionsDataModel*> * children, const bool main);
    void doubleClickedFullList(const QModelIndex & index);
    void check(const int state, const int checkIndex, const QString & tabName);
    void spinCheck(const int spinBoxIndex, const QString & tabName, const int value);
    void optionsCheck(const int state, const int checkBoxIndex, const QString & tabName);
    void optionsSpinCheck(const int spinBoxIndex, const QString & tabName, const int value);
    void addToTotalSlot(const int num) { addToTotal(num); }
    void save();
    void saveToPdf();
    void print();
    void load();
    void loadFile(const int index);
    void showOptionsSlot(OptionsDataModel * options);
    void showOptionsSlot(OptionsDataModel * options, const int);
    void selectRowSlot(const int row);
    void upButtonSlot();
    void downButtonSlot();
    /// Keeping it separate from createSpinBox because that one is used also in other parts.
    void createSpinBoxSlot(const DataModel* model, const int row);
    void removedSpinBoxSlot(const DataModel* model, const int removedRow);
    void addedSpinBoxSlot(const DataModel* model, const int addedRow);
    void selectType(const int index);
    void connectOptionsSlot(const DataModel*);
    void updateCountSlot();

signals:
    void displayTotal(const int total);

private:
    void addToTotal(const int num);
    void keyPressEvent(QKeyEvent * event);
    void clearOptions();
    SpinBox * createSpinBox(const DataModel * model, const int row, const QString & tab = "Options");
    void connectSpinBox(SpinBox * spinBox, const int row);
    void syncSpinBoxes(const DataModel * model, const int fromRow, const int shift);
    QWidget * createTab(const QString & tabName, QGridLayout * gridBox);
    void drawBackgroundImage(const QString & imageName);
    void setStyle(const QString & alpha);
    bool getOutFileName(QString & fileName, const QString & extension);
    void fillOutTextStream(QTextStream & out);
    int findIndex(const QList<QString> & list, const QString & name);
    int findRow(const DataModel * model, const QString & name);
    // std::pair<const DataModel *, int> setOption(const DataModel * model, const QString & name,
    //                                             const int selectedModelRow, const bool subOption = false);
    void setOption(const DataModel * model, const QString & name,
                   const int selectedModelRow, const bool subOption = false);
    int findUnfilledSpinBox(const DataModel * model, const int row, const QString & tabName);

    Ui::MainWindowClass *ui;
    QMap<int, QList<QString> > fileList_;
    QMap<int, QList<QString> > backgroundImageList_;
    QMap<int, QList<QString> > alphaList_;
    QList<QString> fileTypeList_;
    DataModel *model_;
    DataModel *selectedModel_;
    int total_;
    int activeFileIndex_;
    int activeTypeIndex_;
    QList<QObject*> tabs_;
    QString currentOptionType_;
    QString specialString_;
    //SpinBoxDelegate *delegate_;

    // Used only during loading
    bool loading_;

    /// Small struct used to pass the values from the setOptions method
    struct OptionsModelRowAndTab
    {
        OptionsModelRowAndTab() :
                model(0),
                row(-1),
                tabName("")
        {}

        OptionsModelRowAndTab(const DataModel * inputModel, const int inputRow, const QString & inputTabName) :
                model(inputModel),
                row(inputRow),
                tabName(inputTabName)
        {}

        bool operator==(const OptionsModelRowAndTab & compare) const
        {
            if( (model == compare.model) && (row == compare.row) && (tabName == compare.tabName) ) return true;
            return false;
        }
        const DataModel * model;
        int row;
        QString tabName;
    } modelRowAndTab_, optionModelRowAndTab_;

    QHash<SpinBox *, OptionsModelRowAndTab> subOptionSpinBoxes_;
    QList<std::pair<OptionsModelRowAndTab, int> > unfilledSpinBoxes_;
};

#endif // MAINWINDOW_H
