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

#include <QtGui/QApplication>
#include <QPlastiqueStyle>
// #include <QtGui/QStyleFactory>
//#include "qtc-gdbmacros/gdbmacros.cpp"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Set the style to plastique. With the default windows style
    // the QTabWidget will not have a transparent background.
    // QStyleFactory styleFactory;
    // a.setStyle(styleFactory.create("plastique"));
    QApplication::setStyle(new QPlastiqueStyle); // or QCleanlooksStyle, or...

    return a.exec();
}
