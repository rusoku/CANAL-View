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
#include "ui_mainwindow.h"
#include <QDebug>


void MainWindow::saveSettings()
{
  //settings->beginGroup("forms");
  //settings->setValue("title", windowTitle());
  //settings->beginGroup(objectName());
  //settings->setValue("geometry", geometry());
  //settings->endGroup();
  //settings->endGroup();

  settings->beginGroup("OpenFlags");
  settings->setValue("Silent", ui->cb_SilentMode->isChecked());
  settings->setValue("Loopback", ui->cb_LoopbackMode->isChecked());
  settings->setValue("Retransmition", ui->cb_DisRetransmition->isChecked());
  settings->setValue("BusOff", ui->cb_AutoBusOff->isChecked());
  settings->setValue("StatusMessage", ui->cb_EnStatusMessage->isChecked());
  settings->setValue("TimMode", ui->cb_TimMode->isChecked());
  settings->setValue("RxFifoLocked", ui->cb_RxFifoLocked->isChecked());
  settings->setValue("TxFifoMode", ui->cb_TxFifoPriority->isChecked());
  settings->setValue("WakeUp", ui->cb_WakeUpMode->isChecked());
  settings->setValue("TimeDelay", ui->cb_TimestampDelay->isChecked());
  settings->setValue("OpenString",ui->le_OpenParameters->text());
  settings->endGroup();
}

void MainWindow::loadSettings()
{
  //setWindowTitle(settings->value("title","CANAL View").toString());
  //settings->beginGroup("forms");
  //settings->beginGroup(objectName());
  //setGeometry(settings->value("geometry", QRect(200, 200, 300, 300)).toRect());
  //settings->endGroup();
  //settings->endGroup();


  settings = new QSettings("RUSOKU Technologies", "CANAL View", this);
  //settings = new QSettings("settings.ini", QSettings::IniFormat, this);
  //setObjectName(name);
  setWindowTitle("CANAL view");

  settings->beginGroup("OpenFlags");
  ui->cb_SilentMode->setChecked(settings->value("Silent",false).toBool());
  ui->cb_LoopbackMode->setChecked(settings->value("Loopback",false).toBool());
  ui->cb_DisRetransmition->setChecked(settings->value("Retransmition",false).toBool());
  ui->cb_AutoBusOff->setChecked(settings->value("BusOff",false).toBool());
  ui->cb_EnStatusMessage->setChecked(settings->value("StatusMessage",false).toBool());
  ui->cb_TimMode->setChecked(settings->value("TimMode",false).toBool());
  ui->cb_RxFifoLocked->setChecked(settings->value("RxFifoLocked",false).toBool());
  ui->cb_TxFifoPriority->setChecked(settings->value("TxFifoMode",false).toBool());
  ui->cb_WakeUpMode->setChecked(settings->value("WakeUp",false).toBool());
  ui->cb_TimestampDelay->setChecked(settings->value("TimeDelay",false).toBool());
  ui->le_OpenParameters->setText(settings->value("OpenString","0;12345678;125").toString());
  settings->endGroup();
}
