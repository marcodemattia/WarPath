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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass), model_(0), selectedModel_(0),
    total_(0), activeFileIndex_(0), activeTypeIndex_(0), loading_(false)
{
    ui->setupUi(this);
    // setStyleSheet(QString::fromUtf8("border-image: url(:/Shadowsun.jpg);"));
    // setStyleSheet(QString::fromUtf8("QMainWindow {background-image: url(:/Shadowsun.jpg); background-clip: content; background-repeat: no-repeat;}"));

    // Note that if we do not specify QMainWindow the stleSheet will be propagated and every child widget (that is all widgets in this case)
    // will paint the image again as its own background.
    // setStyleSheet(QString::fromUtf8("QMainWindow {background-image: url(:/Shadowsun.jpg); background-clip: content;}"));

    // Read the file with the name of the last open file if any.
    QString lastOpenFileType;
    QString lastOpenFile;
    QFile lastOpenInputFile("LastOpenFile.txt");
    lastOpenInputFile.open(QIODevice::ReadOnly);
    if(lastOpenInputFile.isOpen()) {
        QTextStream in(&lastOpenInputFile);
        if(!in.atEnd()) {
            lastOpenFileType = in.readLine();
            lastOpenFile = in.readLine();
        }
    }

    // Read the list of input files
    QFile inputFile("FileList.txt");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    int type = -1;
    QString defaultBackgroundImage("");
    QString defaultAlpha("");
    while( !in.atEnd() ) {
        QString line(in.readLine());
        if( !line.startsWith("#") && !line.isEmpty() ) {
            if( !line.startsWith("-") ) {
                if( line.startsWith("DefaultBackgroundImage") ) {
                    QStringList defaultImageList((line.split(":"))[1].split(";"));
                    unsigned int defaultImageListSize = defaultImageList.size();
                    if( defaultImageListSize > 0 ) {
                        defaultBackgroundImage = defaultImageList[0].trimmed();
                        if( defaultImageListSize > 1 ) {
                            defaultAlpha = defaultImageList[1].trimmed();
                        }
                    }
                }
                else {
                    fileTypeList_.push_back(line);
                    ++type;
                }
            }
            else {
                // Save the listFile name.
                // Save also the image and alpha for this listFile if any
                // otherwise save the default ones.
                QStringList splittedLine(line.split(";"));
                unsigned int splittedLineSize = splittedLine.size();
                if( splittedLineSize > 0 ) {
                    fileList_[type].push_back(splittedLine[0].remove(0,1).trimmed());
                    if( splittedLineSize > 1 ) {
                        backgroundImageList_[type].push_back(splittedLine[1].trimmed());
                    }
                    else backgroundImageList_[type].push_back(defaultBackgroundImage);
                    if( splittedLineSize > 2 ) {
                        alphaList_[type].push_back(splittedLine[2].trimmed());
                    }
                    else alphaList_[type].push_back(defaultAlpha);
                }
            }
        }
    }
    int lastOpenFileTypeNum = -1;
    int fileTypeNum = 0;

    QList<QString>::const_iterator nameIt = fileTypeList_.begin();
    for( ; nameIt != fileTypeList_.end(); ++nameIt, ++fileTypeNum ) {
        if( *nameIt == lastOpenFileType ) lastOpenFileTypeNum = fileTypeNum;
        ui->listTypeSelectionComboBox->insertItem(fileTypeNum, *nameIt);
    }

    connect(ui->listSelectionComboBox, SIGNAL(activated(int)), this, SLOT(loadFile(int)));
    connect(ui->listTypeSelectionComboBox, SIGNAL(activated(int)), this, SLOT(selectType(int)));
    connect(this, SIGNAL(displayTotal(int)), ui->lcdTotal, SLOT(display(int)));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSaveToPdf, SIGNAL(triggered()), this, SLOT(saveToPdf()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
    connect(ui->mainList, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedMainList(QModelIndex)));
    connect(ui->fullList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedFullList(QModelIndex)));
    connect(ui->optionsTable, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedOptionsTable(QModelIndex)));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(upButtonSlot()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(downButtonSlot()));

    if ( lastOpenFileTypeNum != -1 ) {
        selectType( lastOpenFileTypeNum );
        ui->listTypeSelectionComboBox->setCurrentIndex(lastOpenFileTypeNum);
        int fileNum = 0;
        int lastOpenFileNum = -1;
        for( QList<QString>::const_iterator fileIt = fileList_[lastOpenFileTypeNum].begin();
        fileIt != fileList_[lastOpenFileTypeNum].end(); ++fileIt, ++fileNum ) {
            if( *fileIt == lastOpenFile ) lastOpenFileNum = fileNum;
        }
        if( lastOpenFileNum != -1 ) {
            loadFile(lastOpenFileNum);
            ui->listSelectionComboBox->setCurrentIndex(lastOpenFileNum);
        }
    }
    ui->optionsTabWidget->clear();

    // Read from file and set labels and the specialString written for the fifth field in unit lines.
    QFile localizeFile("Localize.txt");
    localizeFile.open(QIODevice::ReadOnly);
    QTextStream localize(&localizeFile);
    int localizeCount = 0;
    while( !localize.atEnd() ) {
        QString line(localize.readLine());
        if( !line.startsWith("#") ) {
            switch(localizeCount) {
            case 0:
                ui->label_2->setText(line);
                break;
            case 1:
                ui->label_3->setText(line);
                break;
            case 2:
                ui->label->setText(line);
                break;
            case 3:
                specialString_ = line;
                break;
            default:
                break;
            }
            ++localizeCount;
        }
    }
}

MainWindow::~MainWindow()
{
    // Save the name of the last open file.
    if( activeTypeIndex_ != -1 ) {
        QFile data("LastOpenFile.txt");
        if( data.open(QFile::WriteOnly | QFile::Truncate) ) {
            QTextStream out(&data);
            out << fileTypeList_.at(activeTypeIndex_) << endl;
            if( activeFileIndex_ != -1 ) {
                out << fileList_[activeTypeIndex_].at(activeFileIndex_);
            }
        }
    }
    delete ui;
}

void MainWindow::selectType(const int index)
{
    ui->listSelectionComboBox->clear();
    int fileNum = 0;
    QList<QString>::const_iterator nameIt = fileList_[index].begin();
    for( ; nameIt != fileList_[index].end(); ++nameIt, ++fileNum ) {
//        if( *nameIt == lastOpenFile ) lastOpenFileNum = fileNum;
        QString name(nameIt->split(".").at(0));
        if( name.contains("/") ) ui->listSelectionComboBox->insertItem(fileNum, name.split("/").last());
        else ui->listSelectionComboBox->insertItem(fileNum, name);
    }
    activeTypeIndex_ = index;
}

void MainWindow::drawBackgroundImage(const QString & imageName)
{
    QString backgroundImage = QString::fromUtf8("QMainWindow {background-image: url(");
    if( !imageName.isEmpty() ) {
        backgroundImage += imageName;
    }
    backgroundImage += QString::fromUtf8("); background-clip: content;}");
    setStyleSheet(backgroundImage);
}

void MainWindow::setStyle(const QString & alpha)
{
    // Set transparent background to all the other windows
    QString style("");
    // style += QString::fromUtf8("QTabBar::tab, ");
    style += QString::fromUtf8("QTabWidget, ");
    style += QString::fromUtf8("QTableView, ");
    style += QString::fromUtf8("QListView, ");
    style += QString::fromUtf8("QToolButton, ");
    style += QString::fromUtf8("QLabel, ");
    style += QString::fromUtf8("QLCDNumber ");
    style += QString::fromUtf8("{");
    style += QString::fromUtf8("background-color: rgba(255, 255, 255,");
    style += alpha;
    style += QString::fromUtf8(");} ");

    ui->optionsTabWidget->setStyleSheet(style);
    ui->mainList->setStyleSheet(style);
    ui->fullList->setStyleSheet(style);
    ui->optionsTable->setStyleSheet(style);
    ui->upButton->setStyleSheet(style);
    ui->downButton->setStyleSheet(style);
    ui->label->setStyleSheet(style);
    ui->label_2->setStyleSheet(style);
    ui->label_3->setStyleSheet(style);
    ui->lcdTotal->setStyleSheet(style);
}

int MainWindow::findIndex(const QList<QString> & list, const QString & name)
{
    int index = 0;
    QList<QString>::const_iterator it = list.begin();
    for( ; it != list.end(); ++it, ++index ) {
        QString file = *it;
        if( *it == name ) return index;
    }
    return -1;
}

int MainWindow::findRow(const DataModel * model, const QString & name)
{
    int foundRow = -1;
    int modelRows = model->modelRowCount();
    for( int row = 0; row < modelRows; ++row ) {
        if( model->getData(row).string_ == name ) {
            foundRow = row;
        }
    }
    return foundRow;
}

void MainWindow::setOption(const DataModel * model, const QString & name,
                           const int selectedModelRow, const bool subOption)
{
    // First reset all the values
    if( !subOption ) {
        modelRowAndTab_.model = 0;
        modelRowAndTab_.row = -1;
        modelRowAndTab_.tabName = "";
    }

    const QMap<QString, OptionsDataModel *> * tabsMap = model->children(selectedModelRow);
    QMap<QString, OptionsDataModel *>::const_iterator tabsIt = tabsMap->begin();
    int row = -1;
    QString tabName("");
    bool state = false;
    // int size = tabsMap->size();
    for( ; tabsIt != tabsMap->end(); ++tabsIt ) {
        row = findRow(*tabsIt, name);
        if( row != -1 ) {
            tabName = tabsIt.key();
            state = (*tabsIt)->checked(row);
            if( state == false ) {
                if( subOption ) {
                    optionsCheck(true, row, tabName);
                }
                else {
                    check(true, row, tabName);
                }
            }
            if( !subOption ) {
                modelRowAndTab_.model = *tabsIt;
                modelRowAndTab_.row = row;
                modelRowAndTab_.tabName = tabName;
            }
            else {
//                const OptionsDataModel * newModel = dynamic_cast<const OptionsDataModel*>(model);
//                newModel = newModel->singleItemChildren(selectedModelRow)->value(tabName);
//                if( newModel != 0 ) {
//                    optionModelRowAndTab_.model = newModel;
//                }
//                else {
                optionModelRowAndTab_.model = *tabsIt;
//                }
                optionModelRowAndTab_.row = row;
                optionModelRowAndTab_.tabName = tabName;
            }
        }
    }
}

void MainWindow::load()
{
    loading_ = true;
    QString fileName(QFileDialog::getOpenFileName(this));

    if( fileName == "" ) return;

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);

    // Read the dataFile type
    QString dataFileType(in.readLine().split("=").at(1));
    int typeIndex = findIndex(fileTypeList_, dataFileType);
    if( typeIndex == -1 ) {
        // Show an error window and exit from this method
        return;
    }
    // Read the dataFile name
    QString dataFileName(in.readLine().split("=").at(1));
    int fileIndex = findIndex(fileList_[typeIndex], dataFileName);
    if( fileIndex == -1 ) {
        // Show an error window and exit from this method
        return;
    }
    // Load the file
    selectType(typeIndex);
    loadFile(fileIndex);

    int selectedModelRow = -1;
    // std::pair<const DataModel *, int> optionsModelAndRow = std::pair<const DataModel *, int>(0, -1);

    QString optionName("");
    int multiOptionCounter = 0;

    while( !in.atEnd() ) {
        QString line(in.readLine());
        if( line.startsWith("Total") || line.isEmpty() ) continue;

        // It is an item
        if( !line.startsWith("-") ) {
            optionName = "";
            int count = -1;
            QStringList splittedLine(line.split(" - "));
            QString name = splittedLine.at(0);
            if( name.split(" x ").size() > 1 ) {
                count = name.split(" x ").at(0).toInt();
                name = name.split(" x ").at(1);
            }
            int row = findRow(model_, name);
            if( row == -1 ) {
                // This is an error, it should never happen.
                continue;
            }
            doubleClickedFullList(model_->index(row, 0));

            selectedModelRow = selectedModel_->rowCount()-1;
            if( count != -1 ) {
                // Set the attributes
                int startingCount = selectedModel_->count(selectedModelRow);
                if( count != startingCount ) {
                    SpinBox * spinBox = (SpinBox*)ui->mainList->indexWidget( selectedModel_->index(selectedModelRow,1) );
                    if( spinBox != 0 ) spinBox->setValue(count);
                }
            }
            continue;
        }

        // This is an option or a suboption

        if( selectedModelRow == -1 ) {
            // This is an error, it should never happen.
            continue;
        }

        // Find the option among all the tabs
        QStringList splittedLine(line.split("- "));
        QString name = splittedLine.at(1).trimmed();

        if( line.startsWith("- ") ) {
            int count = 1;
            if( name.split(" x ").size() > 1 ) {
                count = name.split(" x ").at(0).toInt();
                name = name.split(" x ").at(1);
            }
            // optionsModelAndRow = setOption(selectedModel_, name, selectedModelRow);
            if( optionName != name ) {
                setOption(selectedModel_, name, selectedModelRow);
                // for( int i=0; i<count; ++i ) {
                //     setOption(selectedModel_, name, selectedModelRow);
                // }
                multiOptionCounter = 0;
                optionName = name;
            }
            else {
                spinCheck(modelRowAndTab_.row, modelRowAndTab_.tabName, 1);
                ++multiOptionCounter;
            }
            if(count > 1) {
                QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
                int row = selected.row();
                OptionsDataModel * model = selectedModel_->children(row)->value(modelRowAndTab_.tabName);
                spinCheck(modelRowAndTab_.row, modelRowAndTab_.tabName, count - model->count(modelRowAndTab_.row));
//                model->countValueChanged(modelRowAndTab_.row, count);
            }
            // Now set the number in the spinbox
            SpinBox * spinBox = subOptionSpinBoxes_.key(modelRowAndTab_);
            if( spinBox != 0 ) {
                spinBox->setValue(count+multiOptionCounter);
            }
        }
        else if( line.startsWith("-- ") ) {
            if( modelRowAndTab_.model == 0 ) {
                // This is an error, it should never happen.
                continue;
            }
            int count = 1;
            if( name.split(" x ").size() > 1 ) {
                count = name.split(" x ").at(0).toInt();
                name = name.split(" x ").at(1);
            }
            ui->optionsTable->selectRow(multiOptionCounter);

            setOption(modelRowAndTab_.model, name, modelRowAndTab_.row, true);

            OptionsDataModel * model = selectedModel_->children(selectedModelRow)->value(currentOptionType_)->singleItemChildren(multiOptionCounter)->value(optionModelRowAndTab_.tabName);

            // int countChange = count - optionModelRowAndTab_.model->count(optionModelRowAndTab_.row);
            int countChange = count - model->count(optionModelRowAndTab_.row);
            optionsSpinCheck(optionModelRowAndTab_.row, optionModelRowAndTab_.tabName, countChange);

            model->countValueChanged(optionModelRowAndTab_.row, count);

            // (const_cast<DataModel *>(optionModelRowAndTab_.model))->countValueChanged(optionModelRowAndTab_.row, count);

            SpinBox * spinBox = subOptionSpinBoxes_.key(optionModelRowAndTab_);
            if( spinBox != 0 ) {
                spinBox->setValue(count);
            }
        }
    }
    inputFile.close();
    subOptionSpinBoxes_.clear();
    loading_ = false;
}

void MainWindow::loadFile(const int index)
{
    total_ = 0;
    emit(displayTotal(total_));
    if( model_ != 0 ) {
        clearOptions();
        ui->optionsTable->setModel(0);
        delete model_;
        if( selectedModel_ != 0 ) delete selectedModel_;
    }
    // QDir::current();
    // model_ = new DataModel(":/" + fileList_.at(index));
    model_ = new DataModel(fileList_[activeTypeIndex_].at(index));
    drawBackgroundImage(backgroundImageList_[activeTypeIndex_].at(index));
    setStyle(alphaList_[activeTypeIndex_].at(index));
    selectedModel_ = 0;
    ui->fullList->setModel(model_);
    activeFileIndex_ = index;
}

void MainWindow::check(const int state, const int checkBoxIndex, const QString & tabName)
{
    // Single selection mode. Only one item in the list.
    QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
    int row = selected.row();
    OptionsDataModel * model = selectedModel_->children(row)->value(tabName);
    // The int checkBoxIndex has beed filled as the index of the row in the child model.
    int value = model->value(checkBoxIndex);
    QString optionSpecialType = model->specialType(checkBoxIndex);
    // Save the state into the model
    model->setChecked(checkBoxIndex, bool(state));
    // If it has been deselected
    if( state == 0 ) value*=-1;
    if( optionSpecialType.contains("xModel") ) {
        int changedValue = selectedModel_->addToNum(row, value);
        addToTotal(changedValue);
    }
    else {
        selectedModel_->addToValue(row, value);
        addToTotal(value);
    }
    // If the selected option has suboptions
    if( !model->children(checkBoxIndex)->isEmpty() ) {
        // Save the name of the tab.
        currentOptionType_ = tabName;
        // This method creates the QList inside the QMap needed to show the items in the optionsTable.
        int optionsValue = model->setupOptions(checkBoxIndex, state);
        selectedModel_->addToValue(row, -optionsValue);
        addToTotal( -optionsValue );
        if( state == 0 ) ui->optionsTable->setModel(0);
        else {
            selectedModel_->setWithSubOptionsModel(row, model);
            ui->optionsTable->setModel(0);
            ui->optionsTable->setModel(model);
        }
    }
}

void MainWindow::optionsCheck(const int state, const int checkBoxIndex, const QString & tabName)
{
    // Single selection mode. Only one item in the list.
    QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
    int row = selected.row();
    QModelIndex selectedOption = ui->optionsTable->selectionModel()->selectedIndexes().at(0);
    int rowOption = selectedOption.row();
    OptionsDataModel * model = selectedModel_->children(row)->value(currentOptionType_)->singleItemChildren(rowOption)->value(tabName);
    // The int checkBoxIndex has beed filled as the index of the row in the child model.
    int value = model->value(checkBoxIndex);
    QString optionType = model->type(checkBoxIndex);
    // Save the state into the model
    model->setChecked(checkBoxIndex, bool(state));
    // If it has been deselected
    if( state == 0 ) value*=-1;
    // model->addToSingleItemValue(row, value);
    selectedModel_->children(row)->value(currentOptionType_)->addToSingleItemValue(rowOption, value);
    selectedModel_->addToValue(row, value);
    addToTotal(value);
}

/// Slot used by the spinBoxes in the options tabs.
void MainWindow::spinCheck(const int spinBoxIndex, const QString & tabName, const int countChange)
{
    int optionsValue = 0;
    // Single selection mode. Only one item in the list.
    QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
    int row = selected.row();
    OptionsDataModel * model = (OptionsDataModel*)(selectedModel_->children(row)->value(tabName));
    // if( selectedModel_->withSubOptionsModel(row) != 0 ) {
    if( !model->children(spinBoxIndex)->isEmpty() ) {
        // Keep min and max == count so that the spinBox will never be created.
        selectedModel_->addToCountMin(row, countChange);
        selectedModel_->addToCountMax(row, countChange);
        if( model->checked(spinBoxIndex) ) {
            selectedModel_->addToCount(row, countChange);
            bool shownOptions = false;
            const QMap<int, QList<Data> > * subOptions = model->selectedOptions();
            if( subOptions->contains(spinBoxIndex) ) {
                if( model->selectedOptionRow() == spinBoxIndex ) shownOptions = true;
                if( countChange > 0 ) {
                    for( int i=0; i<countChange; ++i ) {
                        // selectedModel_->withSubOptionsModel(row)->addItem(spinBoxIndex);
                        model->addItem(spinBoxIndex, shownOptions);
                    }
                }
                else {
                    for( int i=0; i>countChange; --i ) {
                        optionsValue += model->removeItem(spinBoxIndex, shownOptions);
                    }
                }
            }
        }
    }
    if( model->checked(spinBoxIndex) ) {
        double num = model->num(spinBoxIndex);
        selectedModel_->addToValue(row, int(num*countChange - optionsValue));
        addToTotal(int(num*countChange - optionsValue));
    }
}

void MainWindow::optionsSpinCheck(const int spinBoxIndex, const QString & tabName, const int countChange)
{
    // Single selection mode. Only one item in the list.
    QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
    int row = selected.row();
    QModelIndex selectedOption = ui->optionsTable->selectionModel()->selectedIndexes().at(0);
    int rowOption = selectedOption.row();
    OptionsDataModel * model = selectedModel_->children(row)->value(currentOptionType_)->singleItemChildren(rowOption)->value(tabName);
    if( model->checked(spinBoxIndex) ) {
        double num = model->num(spinBoxIndex);
        selectedModel_->children(row)->value(currentOptionType_)->addToSingleItemValue(rowOption, int(num*countChange));
        selectedModel_->addToValue(row, int(num*countChange));
        addToTotal(int(num*countChange));
    }
}

void MainWindow::addToTotal(const int value)
{
    total_+=value;
    emit(displayTotal(total_));
}

void MainWindow::clickedMainList(const QModelIndex & index)
{
    clickedList(index.row(), selectedModel_->children(index.row()), true);
}

void MainWindow::clickedMainList(const int row)
{
    clickedList(row, selectedModel_->children(row), true);
}

void MainWindow::clickedOptionsTable(const QModelIndex & index)
{
    QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
    int row = selected.row();
    // QString type(currentOptionType_);
//    bool test = selectedModel_->children(row)->contains(currentOptionType_);
//    int size = selectedModel_->children(row)->size();
    OptionsDataModel * model = (OptionsDataModel*)(selectedModel_->children(row)->value(currentOptionType_));
//    for( QMap<QString, OptionsDataModel*>::const_iterator it = selectedModel_->children(row)->begin();
//    it!= selectedModel_->children(row)->end(); ++it) {
//        QString key(it.key());
//        int i=0;
//    }
    clickedList(index.row(), model->singleItemChildren(index.row()), false);
}

void MainWindow::clickedList(const int row, const QMap<QString, OptionsDataModel*> * children, const bool main)
{
    // First clean all contents of the groupBox
    clearOptions();

    // This can be done because it returns a pointer to the QList.
    // int tab = 0;
    QMap<QString, OptionsDataModel *>::const_iterator child = children->constBegin();
    for( ; child != children->constEnd(); ++child ) {
        if( child.value() != 0 ) {
            // ui->optionsTable->setModel(*childModel);
            OptionsDataModel * childModel = child.value();
            QGridLayout * gridBox = new QGridLayout;
            int rows = childModel->modelRowCount();
            for( int i=0; i<rows; ++i ) {
                QString name(childModel->string(i) + " - ");
                name += QString::number(childModel->num(i));
                CheckBox * checkBox = new CheckBox(name, i, child.key());
                checkBox->setChecked(childModel->checked(i));
                if( main ) connect(checkBox, SIGNAL(stateChanged(int, int, QString)), this, SLOT(check(int, int, QString)));
                else connect(checkBox, SIGNAL(stateChanged(int, int, QString)), this, SLOT(optionsCheck(int, int, QString)));
                // When adding a widget to a layout, this takes ownership of the pointer.
                gridBox->addWidget(checkBox, i, 0);
                SpinBox * spinBox = createSpinBox(childModel, i, child.key());
                if( spinBox != 0 ) {
                    if( main ) connect(spinBox, SIGNAL(valueChanged(int, QString, int)), this, SLOT(spinCheck(int, QString, int)));
                    else connect(spinBox, SIGNAL(valueChanged(int, QString, int)), this, SLOT(optionsSpinCheck(int, QString, int)));
                    gridBox->addWidget(spinBox, i, 1);
                    QSpacerItem * horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
                    gridBox->addItem(horizontalSpacer, i, 2);
                }
                if( !childModel->children(i)->isEmpty() ) {
                    // QMap<QString, OptionsDataModel *>::const_iterator childOption = childModel->children(i)->begin();
                    Button * button = new Button(childModel, i);
                    button->setText("Show");
                    //CheckBox * tempCheckBox = dynamic_cast<CheckBox*>(button->parent());
                    //int index = tempCheckBox->index();
                    //connect(button, SIGNAL(clicked()), button, SLOT(clickedSlot()));
                    connect(button, SIGNAL(clicked(OptionsDataModel *, const int)), this, SLOT(showOptionsSlot(OptionsDataModel *, const int)));
                    gridBox->addWidget(button, i, 2);
                }
            }
            ui->optionsTabWidget->addTab( createTab(child.key(), gridBox), child.key() );
        }
    }
    // If the clicked list has a selected option with subOptions
    if( main ) {
        if( selectedModel_->withSubOptionsModel(row) != 0 ) {
            int optionRow = selectedModel_->withSubOptionsModel(row)->selectedOptionRow();
            currentOptionType_ = selectedModel_->withSubOptionsModel(row)->type(optionRow);
            // QString check = currentOptionType_;
            if( selectedModel_->withSubOptionsModel(row)->rowCount() != 0 ) {
                ui->optionsTable->setModel(selectedModel_->withSubOptionsModel(row));
            }
            else ui->optionsTable->setModel(0);
        }
        else ui->optionsTable->setModel(0);
    }
}

void MainWindow::clickedSpinBox(const int row)
{
    QItemSelectionModel * selectionModel = ui->mainList->selectionModel();
    QModelIndex topLeft(selectedModel_->index(row,2));
    QModelIndex bottomRight(selectedModel_->index(row,2));
    QItemSelection selection(topLeft, bottomRight);
    // Using ClearAndSelect to impose single selection.
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    // Call this to force the drawing of the options box.
    clickedMainList(row);
}

void MainWindow::doubleClickedFullList(const QModelIndex & index)
{
    if( selectedModel_ == 0 ) {
        selectedModel_ = new DataModel(index);
        ui->mainList->setModel(selectedModel_);
        // Not using the SpinBoxDelegate, using a SpinBox object directly.
        // The delegate gets visible only after double clicking on the cell, while
        // the SpinBox widget is always visible.
        // delegate_ = new SpinBoxDelegate();
        // ui->mainList->setItemDelegate(delegate_);
        connect(selectedModel_, SIGNAL(addToTotal(int)), this, SLOT(addToTotalSlot(int)));
        connect(selectedModel_, SIGNAL(showOptions(OptionsDataModel*)), this, SLOT(showOptionsSlot(OptionsDataModel*)));
        connect(selectedModel_, SIGNAL(selectRow(int)), this, SLOT(selectRowSlot(int)));
        connect(selectedModel_, SIGNAL(clickMainList(const int, const QMap<QString, OptionsDataModel*>*, const bool)),
                this, SLOT(clickedList(const int, const QMap<QString, OptionsDataModel*>*, const bool)));
        connect(selectedModel_, SIGNAL(createSpinBox(const DataModel*, int)), this, SLOT(createSpinBoxSlot(const DataModel*, int)));
        connect(selectedModel_, SIGNAL(removedSpinBox(const DataModel*,int)), this, SLOT(removedSpinBoxSlot(const DataModel*,int)));
        connect(selectedModel_, SIGNAL(addedSpinBox(const DataModel*,int)), this, SLOT(addedSpinBoxSlot(const DataModel*,int)));
        connect(selectedModel_, SIGNAL(connectOptions(const DataModel*)), this, SLOT(connectOptionsSlot(const DataModel*)));
    }
    int rowCount = selectedModel_->modelRowCount();
    selectedModel_->insertRows(rowCount,1,index);
    // Before the row insertion rowCount = size. Now, after the insertion, it is equal to size - 1.
    SpinBox * spinBox = createSpinBox(selectedModel_, rowCount);
    connectSpinBox(spinBox, rowCount);
    clickedSpinBox(rowCount);
}

//int MainWindow::findUnfilledSpinBox(const DataModel * model, const int row, const QString & tabName)
//{
//    QList<std::pair<OptionsModelRowAndTab, int> >::const_iterator it = unfilledSpinBoxes_.begin();
//    int counter = 0;
//    for( ; it != unfilledSpinBoxes_.end(); ++it, ++counter ) {
//        const DataModel * checkModel = it->first.model;
//        // const DataModel * checkParentModel = it->first.model->parent(it->first.model);
//        QString parentModelName = model->string(row);
//        QString modelName = it->first.model->string(it->first.row);
//        // this will not compile
//        int checkRow = it->first.row;
//        QString checkTabName = it->first.tabName;
//        if( (it->first.model == model) && (it->first.row == row) && (it->first.tabName == tabName) ) {
//            int count = it->second;
//            unfilledSpinBoxes_.removeAt(counter);
//            return count;
//        }
//    }
//    return -1;
//}

SpinBox * MainWindow::createSpinBox(const DataModel * model, const int row, const QString & tabName)
{
    int countMin = model->countMin(row);
    int countMax = model->countMax(row);
    int count = model->count(row);
    // if( !unfilledSpinBoxes_.isEmpty() ) {
    //     int unfilledCount = findUnfilledSpinBox(model, row, tabName);
    //     if( unfilledCount != -1 ) count = unfilledCount;
    // }
    // Do not create the SpinBox if min==max. In that case the value is fixed.
    if( countMin != countMax ) {
        SpinBox * spinBox = new SpinBox(row, tabName);
        spinBox->setMinimum(countMin);
        spinBox->setMaximum(countMax);
        spinBox->setValue(count);
        // bool load = loading_;
        connect(spinBox, SIGNAL(valueChanged(int,int)), model, SLOT(countValueChanged(int,int)));
        if( loading_ ) {
            OptionsModelRowAndTab omrt(model, row, tabName);
            // Remove old pointers. SpinBoxes get created and destroyed all the time. We
            // only need to get the last pointers just before we change the values when loading a list.
            // int removed = subOptionSpinBoxes_.remove(subOptionSpinBoxes_.key(omrt));
            subOptionSpinBoxes_.remove(subOptionSpinBoxes_.key(omrt));
            subOptionSpinBoxes_.insert(spinBox, omrt);
        }
        return spinBox;
    }
    return 0;
}

void MainWindow::connectSpinBox(SpinBox * spinBox, const int row)
{
    if( spinBox != 0 ) {
        // Used so that when the SpinBox receives focus the options checkboxes are updated.
        connect(spinBox, SIGNAL(spinBoxFocus(int)), this, SLOT(clickedSpinBox(int)));
        // Inside setIndexWidget dataChanged is called, forcing an update of the spinBox and
        // resetting the value to the minValue. We save here the value in the spinBox and reset
        // it after the call.
        int value = spinBox->value();
        // The viewport takes ownership of the widget
        ui->mainList->setIndexWidget(selectedModel_->index(row,1), spinBox);
        spinBox->setValue(value);
    }
}

void MainWindow::createSpinBoxSlot(const DataModel* model, const int row)
{
    connectSpinBox(createSpinBox(model, row), row);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if( event->type() == QEvent::KeyPress ) {
        QKeyEvent * ke = static_cast<QKeyEvent *>(event);
        if( ke->key() == Qt::Key_Delete ) {
            // Single selection mode, only one item in the list.
            QItemSelectionModel * selectionModel = ui->mainList->selectionModel();
            if( !selectionModel->selectedIndexes().isEmpty() ) {
                QModelIndex selected = selectionModel->selectedIndexes().at(0);
                // Remove the row (the first SpinBox will also be deleted).
                selectedModel_->removeRows(selected.row(),1,selected);
                // If there is a new item selected, draw its options.
                if( !selectionModel->selectedIndexes().isEmpty() ) {
                    selected = selectionModel->selectedIndexes().at(0);                    
                    clickedMainList(selected);
                }
                else {
                    clearOptions();
                    ui->optionsTable->setModel(0);
                }
            }
        }
    }
}

void MainWindow::addedSpinBoxSlot(const DataModel* model, const int addedRow)
{
    syncSpinBoxes(model, addedRow, 1);
}

void MainWindow::removedSpinBoxSlot(const DataModel* model, const int removedRow)
{
    syncSpinBoxes(model, removedRow, -1);
}

void MainWindow::syncSpinBoxes(const DataModel * model, const int fromRow, const int shift)
{
    // The spinBox is created when insertRows is called. We only need to resync the indeces in all the
    // following spinBoxes.
    for( int row = fromRow; row < model->rowCount(); ++row ) {
        SpinBox * spinBox = (SpinBox*)ui->mainList->indexWidget( model->index(row,1) );
        if( spinBox != 0 ) spinBox->setIndex(spinBox->index() + shift);
    }
}

void MainWindow::clearOptions()
{
    ui->optionsTabWidget->clear();
    QList<QObject*>::const_iterator tab = tabs_.begin();
    for( ; tab != tabs_.end(); ++tab ) {
        delete *tab;
    }
    tabs_.clear();
}

// /// This can be used to intercept events
//bool MainWindow::eventFilter(QObject * object, QEvent * event)
//{
//    if( object == target && event->type() == QEvent::KeyPress ) {
//        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);
//        if( keyEvent->key() == Qt::Key_Delete ) {
//            return true;
//        }
//        else return false;
//    }
//    return false;
//}

bool MainWindow::getOutFileName(QString & fileName, const QString & extension)
{
    // Open a QFileDialog to get the file name.
    fileName = QFileDialog::getSaveFileName(this);
    if( !fileName.isEmpty() ) {
        if( !fileName.contains(".") )fileName += extension;
        return true;
    }
    return false;
}

void MainWindow::fillOutTextStream(QTextStream & out)
{
    selectedModel_->write(out, specialString_);
    out << endl;
    out << "Total = " << total_ << endl;
}

void MainWindow::save()
{
    // Open a QFileDialog to get the file name.
    QString fileName;
    if( getOutFileName(fileName, ".txt") ) {
        QFile data(fileName);
        if( data.open(QFile::WriteOnly | QFile::Truncate) ) {
            QTextStream out(&data);
            // Save also the DataFile name to allow list loading
            out << "Data file type =" << fileTypeList_.at(activeTypeIndex_) << endl;
            out << "Data file name =" << fileList_[activeTypeIndex_].at(activeFileIndex_) << endl << endl;
            fillOutTextStream(out);
        }
    }
}

void MainWindow::saveToPdf()
{
    QString fileName;
    if( getOutFileName(fileName, ".pdf") ) {
        QString outString;
        QTextStream out(&outString);
        fillOutTextStream(out);
        QPrinter printer;
        printer.setOutputFileName( fileName );
        QTextDocument textDocument;
        textDocument.setPlainText(outString);
        textDocument.print(&printer);
    }
}

void MainWindow::print()
{
    QString outString;
    QTextStream out(&outString);
    fillOutTextStream(out);
    QPrinter printer;
    // outString.replace("\n", "<br>");

    QPrintDialog printDialog(&printer, this);
    if( printDialog.exec() ) {
        QTextDocument textDocument;
        // textDocument.setHtml(outString);
        textDocument.setPlainText(outString);
        textDocument.print(&printer);
    }
}

void MainWindow::showOptionsSlot(OptionsDataModel * options)
{
    ui->optionsTable->setModel(options);
}

void MainWindow::showOptionsSlot(OptionsDataModel * options, const int optionRow)
{
    const QMap<int, QList<Data> > * subOptions = options->selectedOptions();
    if( subOptions->contains(optionRow) ) {
        currentOptionType_ = options->type(optionRow);
        selectedModel_->setWithSubOptionsModel(optionRow, options);
        // Set the new index so that the chosen model is shown
        options->setSelectedOptionRow(optionRow);
        ui->optionsTable->setModel(0);
        ui->optionsTable->setModel(options);
    }
    else ui->optionsTable->setModel(0);
}

void MainWindow::selectRowSlot(const int row)
{
    ui->mainList->selectRow(row);
}

void MainWindow::upButtonSlot()
{
    if( ui->mainList->selectionModel() != 0 ) {
        if( !ui->mainList->selectionModel()->selectedIndexes().isEmpty() ) {
            QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
            selectedModel_->switchRows(selected, true);
        }
    }
}

void MainWindow::downButtonSlot()
{
    if( ui->mainList->selectionModel() != 0 ) {
        if( !ui->mainList->selectionModel()->selectedIndexes().isEmpty() ) {
            QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
            selectedModel_->switchRows(selected, false);
        }
    }
}

QWidget * MainWindow::createTab(const QString & tabName, QGridLayout * gridBox)
{
    QWidget * tab;
    QVBoxLayout * verticalLayout;
    QScrollArea * optionsScrollArea;
    QWidget * scrollAreaWidgetContents;
    QVBoxLayout * verticalLayout_1;
    QGroupBox * optionsGroup;

    tab = new QWidget();
    tab->setObjectName(tabName);
    tab->setAutoFillBackground(false);
    verticalLayout = new QVBoxLayout(tab);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(9);
    verticalLayout->setObjectName(tabName+"verticalLayout");
    optionsScrollArea = new QScrollArea(tab);
    optionsScrollArea->setObjectName(tabName+"optionsScrollArea");
    optionsScrollArea->setAutoFillBackground(false);
    optionsScrollArea->setFrameShape(QFrame::StyledPanel);
    optionsScrollArea->setLineWidth(0);
    optionsScrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget(optionsScrollArea);
    scrollAreaWidgetContents->setObjectName(tabName+"scrollAreaWidgetContents");
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 380, 167));
    scrollAreaWidgetContents->setAutoFillBackground(true);
    verticalLayout_1 = new QVBoxLayout(scrollAreaWidgetContents);
    verticalLayout_1->setSpacing(0);
    verticalLayout_1->setMargin(0);
    verticalLayout_1->setObjectName(tabName+"verticalLayout_1");
    optionsGroup = new QGroupBox(scrollAreaWidgetContents);
    optionsGroup->setObjectName(tabName+"group");
    optionsGroup->setAutoFillBackground(false);
    optionsGroup->setFlat(false);
    optionsGroup->setCheckable(false);

    verticalLayout_1->addWidget(optionsGroup);

    optionsScrollArea->setWidget(scrollAreaWidgetContents);

    verticalLayout->addWidget(optionsScrollArea);

    optionsScrollArea->setBackgroundRole(QPalette::Base);
    optionsScrollArea->setFrameStyle(0);

    optionsGroup->setLayout(gridBox);

    // Set transparent background
    tab->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0); background: "));
    // optionsScrollArea->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0); background: "));
    // scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0); background: "));
    // optionsGroup->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0); background: "));


    // Save the pointer to be able to delete it in clearOptions.
    tabs_.push_back(tab);

    return tab;
}

void MainWindow::connectOptionsSlot(const DataModel * options)
{
    connect(options, SIGNAL(updateCount()), this, SLOT(updateCountSlot()));
}

void MainWindow::updateCountSlot()
{
    if( ui->mainList->selectionModel() != 0 && !ui->mainList->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->mainList->selectionModel()->selectedIndexes().at(0);
        QModelIndex topLeft = model_->index(selected.row(), 1);
        int row = selected.row();
        QModelIndex bottomRight = model_->index(selected.row()+1, 1);
        selectedModel_->updated(topLeft, bottomRight);
    }
}
