/*
 * CANAL view demo software for RUSOKU technologies for TouCAN, TouCAN Marine, TouCAN Duo USB<=>CAN bus converter
 *
 * Copyright (C) 2018 Gediminas Simanskis (gediminas@rusoku.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3.0 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 */


#include "mainwindow.h"
#include "canal.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{       
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qRegisterMetaType<canalMsg>("canalMsg");

//    QApplication::addLibraryPath(a.applicationDirPath() + "dll");
//    QStringList paths = QCoreApplication::libraryPaths();
//    paths.append(".");
//    paths.append("imageformats");
//    paths.append("platforms");
//    paths.append("dll");
//    QCoreApplication::setLibraryPaths(paths);



    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/img/Rusoku_Icon.png"));

    //QApplication::addLibraryPath(a.applicationDirPath() + "/dll/");
    //qDebug() << a.applicationDirPath() + "/dll/";

    QApplication::setOrganizationDomain("RUSOKU Technologies");
    QApplication::setApplicationName("CANAL View");

    MainWindow w(0);
    w.show();

    return a.exec();
}
