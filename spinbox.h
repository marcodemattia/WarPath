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

#ifndef SPINBOX_H
#define SPINBOX_H

/**
  * This class inherits from QSpinBox box and stores an additional int which is needed to distinguish the
  * different rows to which the spinboxes will be attached.
  * It adds a signal which returns the new value and this int whenever the spinbox value is changed.
  */

#include <QSpinBox>
#include <QModelIndex>
#include <QAbstractTableModel>

class SpinBox : public QSpinBox
{
    Q_OBJECT
public:
    SpinBox(const int index, const QString & tabName = "Options");
    void setIndex(const int index) { index_ = index; }
    void setTabName(const QString & tabName) { tabName_ = tabName; }
    void setValue(const int value);
    int index() { return index_; }

protected slots:
    void valueChangedSlot(const int value);

signals:
    void valueChanged(const int index, const int value);
    void valueChanged(const int index, const QString & tabName, const int value);
    void spinBoxFocus(const int index);

protected:
    void focusInEvent(QFocusEvent * event);

    int index_;
    QString tabName_;
    int oldValue_;
};

#endif // SPINBOX_H
