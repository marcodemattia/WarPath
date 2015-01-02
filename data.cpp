#include "data.h"
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

Data::Data() : string_(""), count_(0), countMin_(0), countMax_(0),
    num_(0), value_(0), type_(""), specialType_(""), check_(false)
{
}

Data::Data(const Data & data) :
        string_(data.string_), count_(data.count_), countMin_(data.countMin_), countMax_(data.countMax_),
        num_(data.num_), value_(int(data.num_*data.count_)), type_(data.type_), specialType_(data.specialType_), check_(data.check_)
{
    if( !data.children_.isEmpty() ) {
        QMap<QString, OptionsDataModel*>::const_iterator child = data.children_.begin();
        for( ; child != data.children_.end(); ++child ) {
            // This covers the case when an item has only shared options.
            if( child.value() != 0 ) {
                OptionsDataModel * originalChildModel = child.value();
                OptionsDataModel * childDataModel = new OptionsDataModel(QModelIndex());
                int childRows = originalChildModel->modelRowCount();
                QString originalChildName = originalChildModel->string(0);
                originalChildModel->insideCopyConstructor(true);
                childDataModel->insideCopyConstructor(true);
                for( int i=0; i<childRows; ++i ) {
                    childDataModel->insertRows(childDataModel->modelRowCount(), 1, originalChildModel->index(i,0));
                }
                originalChildModel->insideCopyConstructor(false);
                childDataModel->insideCopyConstructor(false);
                children_[child.key()] = childDataModel;
            }
            else children_[child.key()] = 0;
        }
    }
    value_ += totalSelectedOptionsValue();
}

Data::Data(const QString & string, const int countMin, const int countMax,
           const double num, const QString & type, const QString & specialType, const bool check) :
        string_(string), count_(countMin), countMin_(countMin), countMax_(countMax),
        num_(num), value_(int(num*countMin)), type_(type), specialType_(specialType), check_(check)
{
}

Data::~Data()
{
    QMap<QString, OptionsDataModel*>::iterator it = children_.begin();
    for( ; it != children_.end(); ++it ) {
        delete it.value();
    }
}

void Data::set(const int column, const QVariant & value)
{
    switch( column ) {
    case(0):
        string_ = value.toString();
        break;
    case(1):
        count_ = value.toInt();
        break;
    case(2):
        num_ = value.toInt();
        break;
    case(3):
        value_ = value.toInt();
        break;
    case(4):
        type_ = value.toString();
        break;
    case(5):
        // CHECK if passing this type of value it should be assigned with setValue.
        //        child_ = value.value<DataModel*>();
        break;
    case(6):
        check_ = value.toBool();
        break;
    case(7):
        countMin_ = value.toInt();
        break;
    case(8):
        countMax_ = value.toInt();
        break;
    default:
//        QMessageBox::information(0, tr("Data: Wrong column"),
//        "Data: Wrong column index.");
        exit(1);
    }
}

int Data::totalSelectedOptionsValue() const
{
//    int optionsValue = 0;
//    QMap<QString, OptionsDataModel*>::const_iterator subOptionTab = children_.begin();
//    for( ; subOptionTab != children_.end(); ++subOptionTab ) {
//        OptionsDataModel * subOptionsTabModel = subOptionTab.value();
//        for( int subOptionRow = 0; subOptionRow < subOptionsTabModel->modelRowCount(); ++subOptionRow ) {
//            // Do not count options with subOptions, they are added elsewhere.
//            if( subOptionsTabModel->checked(subOptionRow) && subOptionsTabModel->children(subOptionRow)->isEmpty() ) {
//                optionsValue += subOptionsTabModel->value(subOptionRow);
//            }
//            if( !subOptionsTabModel->children(subOptionRow).isEmpty() ) {
//            }
//        }
//    }
    return computeTotalSelectedOptionsValue( &children_ );
}

int Data::computeTotalSelectedOptionsValue( const QMap<QString, OptionsDataModel*> * childrenMap ) const
{
    int optionsValue = 0;
    QMap<QString, OptionsDataModel*>::const_iterator subOptionTab = childrenMap->begin();
    for( ; subOptionTab != childrenMap->end(); ++subOptionTab ) {
        OptionsDataModel * subOptionsTabModel = subOptionTab.value();
        for( int subOptionRow = 0; subOptionRow < subOptionsTabModel->modelRowCount(); ++subOptionRow ) {
            // Do not count options with subOptions, they are added elsewhere.
            // Do not count also options with the specialType "xModel", they are treated in a different way.
            if( subOptionsTabModel->checked(subOptionRow) && subOptionsTabModel->children(subOptionRow)->isEmpty() &&
                !subOptionsTabModel->specialType(subOptionRow).contains("xModel") ) {
                optionsValue += subOptionsTabModel->value(subOptionRow);
            }
            const QList<Data> & subOptionsDataList = subOptionsTabModel->selectedOptions()->value(subOptionRow);
            QList<Data>::const_iterator subOptionsData = subOptionsDataList.begin();
            for( ; subOptionsData != subOptionsDataList.end(); ++subOptionsData ) {
                optionsValue += subOptionsData->totalSelectedOptionsValue();
            }
            if( !subOptionsTabModel->children(subOptionRow)->isEmpty() ) {
                optionsValue += computeTotalSelectedOptionsValue( subOptionsTabModel->children(subOptionRow) );
            }
        }
    }
    return optionsValue;
}
