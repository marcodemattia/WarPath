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

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QVBoxLayout>
//#include "data.h"
//#include "optionsdatamodel.h"
// #include <QSharedPointer>

class Data;
class DataModel;
class OptionsDataModel;

class Data
{
public:
    /// Default construction with initialization.
    Data();
    /// Copy constructor creating a new child if any in the original data.
    Data(const Data & data);
    /// Construction from single values.
    Data(const QString & string, const int countMin, const int countMax, const double num,
            const QString & type, const QString & specialType, const bool check);
    /// Destructor deletes the allocated children.
    ~Data();
    /// Returns a modifiable reference to the value corresponding to column
    void set(const int column, const QVariant & value);
    /// Returns the sum of the values of all the selected options
    int totalSelectedOptionsValue() const;
    int computeTotalSelectedOptionsValue( const QMap<QString, OptionsDataModel*> * childrenMap ) const;

    // data members
    QString string_;
    int count_;
    int countMin_;
    int countMax_;
    double num_;
    int value_;
    QString type_;
    QString specialType_;
    QMap<QString, OptionsDataModel*> children_;
    // Used only by the options
    bool check_;
};

class DataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /// Default constructor reading a text file
    DataModel(const QString & fileName, QObject *parent = 0);
    /// Constructor used for childs
    DataModel(QObject *parent = 0);
    /**
    * The index is not used as of now. The insertRows method needs to be called after
    * the connection of the addToTotal signal, otherwise the first row created inside
    * the constructor would not be taken into account.
    */
    DataModel(const QModelIndex & index, QObject *parent = 0);

    virtual ~DataModel() {}

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /** The rowCount method is used by the view. We define a different method because we
    * want to use rowCount in a different way in the optionsModel and still be able to
    * get the rowCount of the model.
    */
    int modelRowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    /// To make the data editable
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

    /// To add and remove rows
    virtual bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void switchRows(const QModelIndex &index, const bool up);

    /// Return the data structure. All the values can be accessed directly from it.
    Data getData(const int row) const { return dataList_.at(row); }

    /// To return child models
    const QMap<QString, OptionsDataModel*> * children(const int row) const;
    QString string(const int row) const { return dataList_.at(row).string_;}
    int count(const int row) const { return dataList_.at(row).count_; }
    int countMin(const int row) const { return dataList_.at(row).countMin_; }
    int countMax(const int row) const { return dataList_.at(row).countMax_; }
    double num(const int row) const { return dataList_.at(row).num_; }
    int value(const int row) const { return dataList_.at(row).value_; }
    QString type(const int row) const { return dataList_.at(row).type_; }
    QString specialType(const int row) const { return dataList_.at(row).specialType_; }
    bool checked(const int row) const { return dataList_.at(row).check_; }
    void setChecked(const int row, const bool checked) { dataList_[row].check_ = checked; }
    void addToValue(const int row, const int value);
    int addToNum(const int row, const double numIncrement);
    void addToCount(const int row, const int countIncrement);
    void addToCountMin(const int row, const int countIncrement);
    void addToCountMax(const int row, const int countIncrement);
    void fillOptions(QString & line, const bool previousWasMain);
    virtual void write(QTextStream & out, const QString & specialString = "Special:") const;
    OptionsDataModel * withSubOptionsModel(const int row) {
        return withSubOptionsPointers_.value(row);
    }
    void setWithSubOptionsModel(const int row, OptionsDataModel * subOptionsModel);
    // QString backgroundImageName() const {return imageName_;}
    void updated(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
        emit dataChanged(topLeft, bottomRight);
    }

public slots:
    /// Receives the new count value and the row in which it must be inserted.
    void countValueChanged(const int row, const int count);

signals:
    void addToTotal(const int value);
    void showOptions(OptionsDataModel * options);
    void selectRow(const int row);
    void clickMainList(const int row, const QMap<QString, OptionsDataModel *> * children, const bool main);
    void createSpinBox(const DataModel* model, const int row);
    void addedSpinBox(const DataModel * model, const int row);
    void removedSpinBox(const DataModel * model, const int row);
    void connectOptions(const DataModel *);
    void updateCount();

protected:
    void fillLine(QString & line);
    /// Modify this if the structure of the data file changes
    void setColumns() { columnsInFile_ = 4; }
    //bool hasChildren() const { return !(childs_.isEmpty()); }
    void addSharedOptions(const QList<QString> & sharedOptions);
    void write(const QMap<QString, OptionsDataModel *> * children, QTextStream & out, const QString & prefix = "") const;
    void reorderWithSubOptionsMap(const int position, const int shift);
    virtual bool insertRows(int position, int rows, const QModelIndex &index, const bool notSwitchRows);
    void insertOptions( const int position, const int withSubOptionsIndex, OptionsDataModel * withSubOptionsOption,
                        const bool notSwitchRows, const int checkedWithSubOptionsTotValue );

    QList<Data> dataList_;
    int columnsInFile_;
    bool previousWasMain_;
    bool previousWasOption_;
    QString previousName_;
    QMap<int, OptionsDataModel *> withSubOptionsPointers_;
    // QString imageName_;
};

#endif // DATAMODEL_H
