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

#ifndef OPTIONSDATAMODEL_H
#define OPTIONSDATAMODEL_H

#include "datamodel.h"
#include <QObject>
#include <QMap>

class DataModel;

class OptionsDataModel : public DataModel
{
    Q_OBJECT

public:
    /// Default constructor reading a text file
    //    OptionsDataModel(const QString & fileName, QObject *parent = 0);
    /// Constructor used for childs
    OptionsDataModel(QObject *parent = 0);
    /**
    * The index is not used as of now. The insertRows method needs to be called after
    * the connection of the addToTotal signal, otherwise the first row created inside
    * the constructor would not be taken into account.
    */
    OptionsDataModel(const QModelIndex & index, QObject *parent = 0);
    virtual ~OptionsDataModel() {}

    virtual bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) {
        return DataModel::insertRows( position, rows, index );
    }
    virtual bool insertRows(int position, int rows, const QModelIndex &index, const bool notSwitchRows);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    int setupOptions(const int row, const bool cheked = true);
//    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    /** This method must be called before and after the copy constructor in Data, so that
    * rowCount will return the correct amount.
    */
    void insideCopyConstructor(const bool inside) { insideCopyConstructor_ = inside; }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    const QMap<QString, OptionsDataModel*> * singleItemChildren(const int row) const;
    void addToSingleItemValue(const int row, const int value);
    int addToSingleItemNum(const int row, const int numIncrement);
    virtual void write(QTextStream & out, const QString & prefix = "") const;
    void addItem(const int row, const bool isMultiItems = true);
    int removeItem(const int row, const bool spinBox = true);
    const QMap<int, QList<Data> > * selectedOptions() const { return &selectedOptions_; }
    int selectedOptionRow() const { return selectedOptionRow_; }
    void setSelectedOptionRow(const int optionRow) { selectedOptionRow_ = optionRow; }

protected:
    QMap<int, QList<Data> > selectedOptions_;
    int selectedOptionRow_;
//    /// Redifined here so that it uses the modelRowCount method instead of the rowCount one.
//    bool hasIndex(int row, int column, const QModelIndex &parent) const;
    bool insideCopyConstructor_;
};

#endif // OPTIONSDATAMODEL_H
