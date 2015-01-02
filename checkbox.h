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

#ifndef CHECKBOX_H
#define CHECKBOX_H

/**
  * This class inherits from QCheck box and stores an additional int which is needed to distinguish the
  * different check boxes.
  * It adds a signal which returns the state and this int.
  */

#include <QCheckBox>

class CheckBox : public QCheckBox
{
    Q_OBJECT

public:
    CheckBox(const QString & text, const int checkBoxIndex, const QString & tabName, QWidget * parent = 0);
    int index() { return checkBoxIndex_; }
protected slots:
    /// Used to emit the stateChanged(int,int) signal when the stateChanged(int) signal is emitted.
    void stateChangedSlot(const int state);

signals:
    void stateChanged(const int state, const int checkboxIndex, const QString & tabName);

protected:
    int checkBoxIndex_;
    QString tabName_;
};

#endif // CHECKBOX_H
