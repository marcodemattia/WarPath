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

#include "spinbox.h"

SpinBox::SpinBox(const int index, const QString & tabName) : index_(index), tabName_(tabName)
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));
}

void SpinBox::setValue( const int value )
{
    oldValue_ = value;
    QSpinBox::setValue(value);
}

void SpinBox::valueChangedSlot(const int value)
{
    emit(valueChanged(index_, value));
    int valueChange = value - oldValue_;
    emit(valueChanged(index_, tabName_, valueChange));
    oldValue_ = value;
}

void SpinBox::focusInEvent(QFocusEvent * event)
{
    QSpinBox::focusInEvent(event);
    emit(spinBoxFocus(index_));
}
