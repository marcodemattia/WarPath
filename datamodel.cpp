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

#include "optionsdatamodel.h"

DataModel::DataModel(const QString & fileName, QObject *parent) : QAbstractTableModel(parent), previousWasMain_(false), previousWasOption_(false)
{
    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);

    setColumns();

    QList<QString> sharedOptions;

    // imageName_ = "";
    while( !in.atEnd() ) {
        QString line(in.readLine());
        if( line.startsWith("#") || line.isEmpty() ) continue;
//        if( line.startsWith("Image") ) {
//            // Reading the image name for this file
//            imageName_ = (line.split(":"))[1];
//            continue;
//        }
        if( line.startsWith("-") ) {
            fillOptions(line, previousWasMain_);
            previousWasMain_ = false;
            continue;
        }
        if( line.startsWith("@") ) {
            line.remove(0,1);
            // Add this to the shared options list. It will be added at the end.
            sharedOptions.push_back(line);
            continue;
        }
        // Two consecutive main lines, no childs for the previous one.
        previousWasMain_ = true;
        fillLine(line);
    }
    addSharedOptions(sharedOptions);

    inputFile.close();
}

void DataModel::addSharedOptions(const QList<QString> & sharedOptions)
{
    // Add the shared options to the corresponding items:
    // Loop on all the items
    QList<Data>::iterator item = dataList_.begin();
    for( ; item != dataList_.end(); ++item ) {
        // Loop on all the sharedOptions
        for( QList<QString>::const_iterator sharedOptionLine = sharedOptions.begin();
        sharedOptionLine != sharedOptions.end(); ++sharedOptionLine ) {
            // Separate the names from the options line. The first part contains the names, the
            // second part is the normal options line.
            QList<QString> splittedSharedOptionLine = sharedOptionLine->split(":");
            // Check if the name of the item is in the list for this shared option.
            QList<QString> namesList = splittedSharedOptionLine.at(0).split("@");
            for( QList<QString>::const_iterator name = namesList.begin(); name != namesList.end(); ++name ) {
                if( item->string_ == name->trimmed() ) {
                    // Add the shared option to the item options.
                    QString optionLine(splittedSharedOptionLine.at(1));
                    // This is for items without normal options, but with special options.
                    QString type(optionLine);
                    QList<QString> typeCheck = type.split(";");
                    type = "Special Options";
                    if( typeCheck.size() > 3 ) type = typeCheck.at(3).trimmed();
                    if( !item->children_.contains(type) ) item->children_[type] = new OptionsDataModel(this);
                    item->children_[type]->fillLine(optionLine);
                }
            }
        }
        if( !item->children_.isEmpty() ) {
            for( QMap<QString, OptionsDataModel*>::const_iterator child = item->children_.begin();
            child != item->children_.end(); ++child ) {
                child.value()->addSharedOptions(sharedOptions);
            }
        }
    }
}


/// Used for childs, which do not have the type (one less column)
DataModel::DataModel(QObject *parent) : QAbstractTableModel(parent)
{
    setColumns();
//    fillLine(line);
}

DataModel::DataModel(const QModelIndex & index, QObject *parent) : QAbstractTableModel(parent)
{
    setColumns();
}

void DataModel::fillOptions(QString & line, const bool previousWasMain)
{
    // Remove the first char
    line.remove(0,1);
    // Check the type of the option.
    QString checkType(line);
    QList<QString> checkTypeSplit( checkType.split(";") );
    checkType = "Options";
    if( checkTypeSplit.size() > 3 ) {
        if( !checkTypeSplit.at(3).trimmed().isEmpty() ) {
            checkType = checkTypeSplit.at(3).trimmed();
        }
    }
    // If it is a suboption redirect to the child.
    if( line.startsWith("-") ) {
        line.remove(0,1);
        //line.trimmed();
        dataList_.back().children_[previousName_]->fillOptions(line, previousWasOption_);
        previousWasOption_ = false;
    }
    else {
        // trimmed: removes whitespaces at both ends
        line = line.trimmed();
        // No const reference, do not pass a temporary (check).
        if( !dataList_.back().children_.contains(checkType) ) dataList_.back().children_[checkType] = new OptionsDataModel(this);
        dataList_.back().children_[checkType]->fillLine(line);
        previousName_ = checkType;
        previousWasOption_ = true;
    }
}

void DataModel::fillLine(QString & line)
{
    QStringList splittedLine = line.split(";");
    if( splittedLine.size() < columnsInFile_-1 ) {
        QString errorMessage("The number of items is ");
        QMessageBox::information(0, tr("InputFile error"),
            "Wrong number of items in file rows.");
        exit(1);
    }
    QString countMinMax = splittedLine.at(1);
    // First case: count has a min and a max value.
    if( countMinMax.contains("-") ) {
        QStringList countMinMaxList = countMinMax.split("-");
        dataList_.push_back( Data( splittedLine.at(0).trimmed(), countMinMaxList.at(0).toInt(), countMinMaxList.at(1).toInt(),
                                   splittedLine.at(2).toDouble(), QString(""), QString(""), false) );
    }
    // Second case: count has min and no max value.
    else if( countMinMax.contains("+") ) {
        countMinMax.remove("+");
        dataList_.push_back( Data( splittedLine.at(0).trimmed(), countMinMax.toInt(), 9999,
                                   splittedLine.at(2).toDouble(), QString(""), QString(""), false) );
    }
    // Third case: count has a fixed value.
    else {
        int count = countMinMax.toInt();
        dataList_.push_back( Data( splittedLine.at(0).trimmed(), count, count, splittedLine.at(2).toDouble(),
                                    QString(""), QString(""), false) );
    }

    QString checkString;
    if( (splittedLine.size() > 3) && (!splittedLine.at(3).trimmed().isEmpty()) ) dataList_.back().type_ = splittedLine.at(3).trimmed();
    else dataList_.back().type_ = "Options";
    if( splittedLine.size() > 4 ) dataList_.back().specialType_ = splittedLine.at(4).trimmed();
}

bool DataModel::insertRows(int position, int rows, const QModelIndex &index)
{
    return insertRows(position, rows, index, true);
}

bool DataModel::insertRows(int position, int rows, const QModelIndex &index, const bool notSwitchRows)
{
    DataModel * model = (DataModel*)(index.model());
    int row = index.row();
    int value = model->value(row);
    // We use the copy constructor which internally handles the copy of the child
    // pointer through the creation of a new child.
    // dataList_.push_back(Data( model->data(row) ));
    beginInsertRows(QModelIndex(), position, position+rows-1);
    dataList_.insert(position, Data( model->getData(row) ));
    endInsertRows();

    //int currentRow = dataList_.size()-1;
    int currentRow = position;

    // Check if there is only one option with subOptions and in that case
    // set it to checked and show it in the optionsTable.
    if( !dataList_.at(currentRow).children_.isEmpty() ) {
        // QString withSubOptionsString("");
        // QString checkedWithSubOptionsString("");
        int withSubOptionsIndex = -1;
        int checkedWithSubOptionsIndex = -1;
        int withSubOptions = 0;
        int checkedWithSubOptions = 0;
        int checkedWithSubOptionsTotValue = 0;
        OptionsDataModel * withSubOptionsOption = 0;
        OptionsDataModel * checkedWithSubOptionsOption = 0;
        QMap<QString, OptionsDataModel*>::const_iterator child = dataList_.at(currentRow).children_.begin();
        for( ; child != dataList_.at(currentRow).children_.end(); ++child ) {
            OptionsDataModel * options = child.value();
            if( options != 0 ) {
                for( int optionsRow = 0; optionsRow < options->modelRowCount(); ++optionsRow ) {
                    if( options->children(optionsRow)->size() > 0 ) {
                        ++withSubOptions;
                        withSubOptionsIndex = optionsRow;
                        // withSubOptionsString = child.key();
                        withSubOptionsOption = options;
                        if( options->checked(optionsRow) ) {
                            ++checkedWithSubOptions;
                            checkedWithSubOptionsTotValue += options->value(optionsRow);
                            checkedWithSubOptionsIndex = optionsRow;
                            // checkedWithSubOptionsString = child.key();
                            checkedWithSubOptionsOption = options;
                        }
                    }
                }
            }
        }
        if( withSubOptions == 1 ) {
            // If this option with subOptions is not a single option (that is with countMin == countMax == 1) or if it is already
            // checked (in the case of a line switch).
            if( !(withSubOptionsOption->countMin(withSubOptionsIndex) == withSubOptionsOption->countMax(withSubOptionsIndex) &&
                withSubOptionsOption->countMax(withSubOptionsIndex) == 1) || withSubOptionsOption->checked(withSubOptionsIndex) ) {
                insertOptions( position, withSubOptionsIndex, withSubOptionsOption, notSwitchRows, checkedWithSubOptionsTotValue );
            }
            else reorderWithSubOptionsMap(position, 1);
        }
        // If there is at least one option with subOptions selected.
        else if( checkedWithSubOptions >= 1 ) {
            insertOptions( position, checkedWithSubOptionsIndex, checkedWithSubOptionsOption, notSwitchRows, checkedWithSubOptionsTotValue );
        }
        else {
            // If no new value was inserted, shift from this position.
            reorderWithSubOptionsMap(position, 1);
        }
    }
    else {
        // If no new value was inserted, shift from this position.
        reorderWithSubOptionsMap(position, 1);
    }

    emit(selectRow(currentRow));
    emit(clickMainList(currentRow, children(currentRow), true));
    emit(addedSpinBox(this, position + 1));
    emit(addToTotal(value));
    // emit(connectOptions(model));

    return true;
}

void DataModel::insertOptions( const int position, const int withSubOptionsIndex, OptionsDataModel * withSubOptionsOption,
                               const bool notSwitchRows, const int checkedWithSubOptionsTotValue )
{
    withSubOptionsOption->setupOptions(withSubOptionsIndex);
    // When switching lines we must preserve the case in which the option is not checked.
    int optionValue = 0;
    if( withSubOptionsOption->checked(withSubOptionsIndex) || notSwitchRows ) {
        // This is needed only when notSwitchRows == true.
        withSubOptionsOption->setChecked(withSubOptionsIndex, true);

        // When notSwitchRows == true there will be only one option checked,
        // but when switching lines there could be more, thus we ask data for the total.
        if( checkedWithSubOptionsTotValue == 0 ) optionValue = withSubOptionsOption->value(withSubOptionsIndex);
        else optionValue = checkedWithSubOptionsTotValue;
        addToValue(position, optionValue);
    }
    // else withSubOptionsOption->setChecked(withSubOptionsIndex, false);
    // Before overwriting it, save the pointer in the current position.
    OptionsDataModel * oldWithSubOptionsOption = withSubOptionsPointers_[position];
    withSubOptionsPointers_[position] = withSubOptionsOption;
    // If a new value was inserted, shift from the next position.
    reorderWithSubOptionsMap(position+1, 1);
    // Now put back the old pointer (if any) in position+1.
    if( oldWithSubOptionsOption != 0 ) withSubOptionsPointers_[position+1] = oldWithSubOptionsOption;
    if( notSwitchRows ) emit(addToTotal(optionValue));
    // Show it only if it is checked.
    if( withSubOptionsOption->checked(withSubOptionsIndex) ) emit(showOptions(withSubOptionsOption));
    else emit(showOptions(0));
    emit(connectOptions(withSubOptionsOption));
}

void DataModel::switchRows(const QModelIndex &index, const bool up)
{
//    int shift = 0;
//    if( up ) shift = -1;
    int row = index.row();
    if( dataList_.size() > 1 ) {
        if( (up && row > 0) ) {
            insertRows(row - 1, 1, index, false);
            emit(createSpinBox(this, row - 1));
            removeRows(row + 1, 1, index);
            // emit(selectRow(row));
            emit(clickMainList(row - 1, children(row - 1), true));
            // Change also the spinBox indexes
        }
        if( (!up && row < dataList_.size()-1) ) {
            insertRows(row + 2, 1, index, false);
            emit(createSpinBox(this, row + 2));
            removeRows(row, 1, index);
            // emit(selectRow(row));
            emit(clickMainList(row + 1, children(row + 1), true));
            // Change also the spinBox indexes
        }
    }
}

bool DataModel::removeRows(int position, int rows, const QModelIndex &index)
{
    int value = dataList_.at(position).value_;

    beginRemoveRows(QModelIndex(), position, position+rows-1);
    dataList_.removeAt(position);
    endRemoveRows();

    reorderWithSubOptionsMap(position, -1);

    emit(removedSpinBox(this, position));
    emit(addToTotal(-1*value));
    return true;
}

void DataModel::setWithSubOptionsModel(const int row, OptionsDataModel * subOptionsModel) {
    if( !withSubOptionsPointers_.contains(row) ) {
        withSubOptionsPointers_[row] = subOptionsModel;
    }
}

void DataModel::reorderWithSubOptionsMap(const int position, const int shift)
{
    if( !withSubOptionsPointers_.isEmpty() ) {
        // Reorder the QMap with pointer to options with subOptions.
        // If we removed a line, delete also the corresponding pointer.
        // if( shift == -1 ) delete withSubOptionsPointers_.take(position);
        if( shift == -1 ) withSubOptionsPointers_.take(position);

        // QList<int> toChange;

        QMap<int, OptionsDataModel *> tempWithSubOptionsPointers;
        for( QMap<int, OptionsDataModel *>::iterator option = withSubOptionsPointers_.begin();
        option != withSubOptionsPointers_.end(); ++option ) {
            // When inserting a new line we need also to shift the element corresponding to position.
            // When removing a line we deleted the element at position, thus the = in >= will never happen.
            if( option.key() >= position ) {
                // toChange.push_back( option.key() );
                tempWithSubOptionsPointers.insert(option.key()+shift, option.value());
            }
            else {
                tempWithSubOptionsPointers.insert(option.key(), option.value());
            }
        }
        withSubOptionsPointers_ = tempWithSubOptionsPointers;

        // // Sort in ascending order.
        // qSort(toChange);
        // for( QList<int>::const_iterator changeIndex = toChange.begin(); changeIndex != toChange.end(); ++changeIndex ) {
        //     OptionsDataModel * model = withSubOptionsPointers_.take(*changeIndex);
        //     withSubOptionsPointers_[*changeIndex + shift] = model;
        // }

    }
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    return dataList_.count();
}

int DataModel::modelRowCount(const QModelIndex &parent) const
{
    return dataList_.count();
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    // One of the entries is used as headerData
    return columnsInFile_-1;
}

const QMap<QString, OptionsDataModel *> * DataModel::children(const int row) const {
    if( row <= modelRowCount() ) return &(dataList_.at(row).children_);
    return 0;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= dataList_.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch( index.column() ) {
        case (0):
            return dataList_.at(index.row()).string_;
            break;
        case (1):
            return dataList_.at(index.row()).count_;
            break;
        case (2):
            return dataList_.at(index.row()).value_;
            break;
        default:
        QMessageBox::information(0, tr("Wrong column"),
            "Wrong column index.");
            exit(1);
        }
    }
    else
        return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        switch(section) {
        case(0):
            return QString("Name");
            break;
        case(1):
            return QString("Number");
            break;
        case(2):
            return QString("Value");
            break;
        default:
            return QString("Column %1").arg(section);
            break;
        }
    if (orientation == Qt::Vertical)
        return dataList_.at(section).type_;
    else
        return dataList_.at(section).type_;
        // return QString("Row %1").arg(section);
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index);
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        dataList_[index.row()].set(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void DataModel::countValueChanged(const int row, const int count)
{
    int valueChange = int((count - dataList_[row].count_)*dataList_[row].num_);
    emit(addToTotal( valueChange ));
    int value = dataList_[row].value_ + valueChange;
    dataList_[row].set(1, QVariant(count));
    dataList_[row].set(3, QVariant(value));
    // Set also min and max == count so that the spinBox wont be created.
    emit dataChanged(index(row,2), index(row,2));
}

void DataModel::addToCount(const int row, const int countIncrement)
{
//    int valueChange = int(countIncrement*dataList_[row].num_);
//    emit(addToTotal( valueChange ));
//    int value = dataList_[row].value_ + valueChange;
//    int value = dataList_[row].value_;
    dataList_[row].set(1, QVariant(dataList_[row].count_ + countIncrement));
//    dataList_[row].set(3, QVariant(value));
//    emit dataChanged(index(row,1), index(row,2));
}
void DataModel::addToCountMin(const int row, const int countIncrement)
{
    dataList_[row].set(7, QVariant(dataList_[row].count_ + countIncrement));
}

void DataModel::addToCountMax(const int row, const int countIncrement)
{
    dataList_[row].set(8, QVariant(dataList_[row].count_ + countIncrement));
}

void DataModel::addToValue(const int row, const int valueIncrement)
{
    int valueChange = dataList_[row].value_ + valueIncrement;
    dataList_[row].set(3, valueChange);
    emit dataChanged(index(row,2), index(row,2));
}

int DataModel::addToNum(const int row, const double numIncrement)
{
    Data * data = &(dataList_[row]);
    double numChange = data->num_ + numIncrement;
    // The new value is the old + the increment multiplied by the count of the data.
    int valueChange = int(numIncrement*data->count_);
    data->set(2, numChange);
    data->set(3, data->value_ + valueChange);
    emit dataChanged(index(row,2), index(row,2));
    return valueChange;
}

void DataModel::write(QTextStream & out, const QString & specialString) const
{
    // Loop on all the entries in the selectedModel and write them.
    int rowCount = modelRowCount();
    for( int row = 0; row < rowCount; ++row ) {
        // If it is not a fixed 1 entry write also the number of entries.
        if( !(countMax(row) == countMin(row) && countMax(row) == 1) ) {
            // Check if this option has subOptions with more then one possible items.
            bool singleItems = true;
            QMap<QString, OptionsDataModel*>::const_iterator childPair = children(row)->begin();
            for( ; childPair != children(row)->end(); ++childPair ) {
                OptionsDataModel * optionsModel = childPair.value();
                int optionRowCount = optionsModel->modelRowCount();
                for( int optionRow = 0; optionRow < optionRowCount; ++optionRow ) {
                    if( !optionsModel->children(optionRow)->isEmpty() ) {
                        if( optionsModel->countMax(optionRow) > 1 ) singleItems = false;
                    }
                }
            }
            if( singleItems ) out << count(row) << " x ";
        }
        out << string(row) << " - " << value(row) << endl;
        write(children(row), out, "-");
        if( !specialType(row).isEmpty() ) out << specialString << " " << specialType(row) << endl;
        out << endl;
    }
}

void DataModel::write(const QMap<QString, OptionsDataModel *> * children, QTextStream & out, const QString & prefix) const
{
    // Loop on all the selected options and write them.
    QMap<QString, OptionsDataModel *>::const_iterator child = children->begin();
    for( ; child != children->end(); ++child ) {
        child.value()->write(out, prefix);
    }
}
