#ifndef BUTTON_H
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

#define BUTTON_H

//#include "QPushButton.h"
#include "qtoolbutton.h"
#include "optionsdatamodel.h"

//class Button : public QPushButton
class Button : public QToolButton
{
    Q_OBJECT
    
    public:
        Button(OptionsDataModel * model, const int buttonIndex) :
                model_(model), buttonIndex_(buttonIndex)
        {
            connect(this, SIGNAL(released()), this, SLOT(clickedSlot()));
        }

    signals:
        void clicked(OptionsDataModel *, const int);
        
    protected slots:
        void clickedSlot()
        {
            emit(clicked(model_, buttonIndex_));
        }

    protected:
        OptionsDataModel * model_;
        const int buttonIndex_;
};

#endif // BUTTON_H
