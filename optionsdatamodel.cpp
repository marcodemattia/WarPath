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

//OptionsDataModel::OptionsDataModel(const QString & fileName, QObject *parent) : DataModel( fileName, parent ),
//    selectedOptionRow_(-1), insideCopyConstructor_(false) {}
OptionsDataModel::OptionsDataModel(QObject *parent) : DataModel(parent), selectedOptionRow_(-1), insideCopyConstructor_(false) {}
OptionsDataModel::OptionsDataModel(const QModelIndex & index, QObject *parent) : DataModel( index, parent ),
    selectedOptionRow_(-1), insideCopyConstructor_(false) {}

int OptionsDataModel::rowCount(const QModelIndex & parent) const
{
    // The first time it must return the rowCount of the model, because the construction of the index
    // inside Data copy constructor requires it.
    // After setupOptions is called it will return the size of the QList in the map.
    if( insideCopyConstructor_ ) return modelRowCount();
    else {
        return selectedOptions_.value(selectedOptionRow_).size();
    }
}

QVariant OptionsDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= selectedOptions_.value(selectedOptionRow_).size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch( index.column() ) {
        case (0):
            return selectedOptions_.value(selectedOptionRow_).at(index.row()).string_;
            break;
        case (1):
            return selectedOptions_.value(selectedOptionRow_).at(index.row()).count_;
            break;
        case (2):
            return selectedOptions_.value(selectedOptionRow_).at(index.row()).value_;
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

bool OptionsDataModel::insertRows(int position, int rows, const QModelIndex &index, const bool notSwitchRows)
{
    OptionsDataModel * model = (OptionsDataModel*)index.model();
    QString name = model->string(position);
    selectedOptions_ = *(model->selectedOptions());
    return DataModel::insertRows( position, rows, index, notSwitchRows );
}

void OptionsDataModel::addItem(const int row, const bool isMultiItems)
{
    if( isMultiItems ) beginInsertRows(QModelIndex(), rowCount(), rowCount());
    selectedOptions_[row].push_back(Data(dataList_.at(row)));
    // All this items are single items.
    selectedOptions_[row].back().countMin_ = 1;
    selectedOptions_[row].back().countMax_ = 1;
    selectedOptions_[row].back().count_ = 1;
    selectedOptions_[row].back().value_ = int(num(row));
    emit updateCount();
    if( isMultiItems ) endInsertRows();
}

int OptionsDataModel::removeItem(const int row, const bool spinBox)
{
    // Take -1 to make less differences in the loop.
    int totRows = rowCount() - 1;
    int optionsValue = selectedOptions_.value(row).at(totRows).totalSelectedOptionsValue();
//    const QMap<QString, OptionsDataModel*> * subOptionsMap = &(selectedOptions_.value(row).at(totRows).children_);
//    QMap<QString, OptionsDataModel*>::const_iterator subOptionTab = subOptionsMap->begin();
//    for( ; subOptionTab != subOptionsMap->end(); ++subOptionTab ) {
//        OptionsDataModel * subOptionsTabModel = subOptionTab.value();
//        for( int subOptionRow = 0; subOptionRow < subOptionsTabModel->modelRowCount(); ++subOptionRow ) {
//            if( subOptionsTabModel->checked(subOptionRow) ) {
//                optionsValue += subOptionsTabModel->value(subOptionRow);
//            }
//        }
//    }
    if( spinBox ) beginRemoveRows(QModelIndex(), 0, 0);
    selectedOptions_[row].pop_back();
    if( selectedOptions_[row].size() == 0 ) selectedOptions_.remove(row);
    if( spinBox ) endRemoveRows();
    emit updateCount();
    return optionsValue;
}
int OptionsDataModel::setupOptions(const int row, const bool checked)
{
    // This is used by addItem, so it must be set before its call.
    selectedOptionRow_ = row;
    int optionsValue = 0;
    if( !selectedOptions_.contains(row) && checked ) {
        // Accessing with the operator[] silently creates an entry if there is none.
        if( countMax(row) > 1 ) {
            for( int i=0; i<count(row); ++i ) {
                addItem(row, false);
            }
        }
        else addItem(row, false);
    }
    else if ( !checked ) {
        // It was unchecked, remove all the items and return the value of all their options.
        for( int i=0; i < count(row); ++i ) {
            optionsValue += removeItem(row, false);
        }
    }
    return optionsValue;
}

QVariant OptionsDataModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return selectedOptions_.value(selectedOptionRow_).at(section).type_;
    else
        return selectedOptions_.value(selectedOptionRow_).at(section).type_;
        // return QString("Row %1").arg(section);
}

const QMap<QString, OptionsDataModel*> * OptionsDataModel::singleItemChildren(const int row) const
{
    if( row <= selectedOptions_.value(selectedOptionRow_).size() ) return &(selectedOptions_[selectedOptionRow_].at(row).children_);
    return 0;
}

void OptionsDataModel::addToSingleItemValue(const int row, const int valueIncrement)
{
    int valueChange = selectedOptions_.value(selectedOptionRow_).at(row).value_ + valueIncrement;
    selectedOptions_[selectedOptionRow_][row].set(3, valueChange);
    emit dataChanged(index(row,2), index(row,2));
}

int OptionsDataModel::addToSingleItemNum(const int row, const int numIncrement)
{
    Data * data = &(selectedOptions_[selectedOptionRow_][row]);
    double numChange = data->num_ + numIncrement;
    // The new value is the old + the increment multiplied by the count of the data.
    int valueChange = int(numIncrement*data->count_);
    data->set(2, numChange);
    data->set(3, data->value_ + valueChange);
    emit dataChanged(index(row,2), index(row,2));
    return valueChange;
}

void OptionsDataModel::write(QTextStream & out, const QString & prefix) const
{
    // Loop on all the entries in the selectedModel and write them.
    int rowCount = modelRowCount();
    for( int row = 0; row < rowCount; ++row ) {
        if( !checked(row) ) continue;
        // Check if this option has subOptions.
        if( selectedOptions_.contains(row) ) {
            // Only options can have subOptions (tab == 0), not sharedOptions (tab == 1).
            for( QList<Data>::const_iterator data = selectedOptions_.value(row).begin();
            data != selectedOptions_.value(row).end(); ++data ) {
                out << prefix << " " << data->string_ << " - " << data->value_ << endl;
                DataModel::write(&(data->children_), out, "--");
            }
        }
        else {
            out << prefix << " ";
            // If it is not a fixed 1 entry write also the number of entries.
            if( !(countMax(row) == countMin(row) && countMax(row) == 1) ) {
                out << count(row) << " x ";
            }
            out << string(row) << " - " << value(row) << endl;
            DataModel::write(children(row), out, "--");
        }
    }
}
