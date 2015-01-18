/*
 * OpenCC Shell
 *
 * Copyright 2014-2015 Arlo Liu <arlo.liu@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MainWindow.hpp"
#include "SingleConvertWidget.hpp"
#include "BatchConvertWidget.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent), mTabWidget(0), mStatusBar(0),
    mSingleConvertWidget(0), mBatchConvertWidget(0)
{
    mStatusBar = new QStatusBar(this);
    this->setStatusBar(mStatusBar);
    this->setWindowTitle(tr("OpenCC Shell"));
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->setWindowIcon(QIcon(":/res/logo.png"));

    mSingleConvertWidget = new SingleConvertWidget(mStatusBar, this);
    mBatchConvertWidget = new BatchConvertWidget(mStatusBar, this);

    mTabWidget = new QTabWidget();
    mTabWidget->addTab(mSingleConvertWidget, QIcon(":/res/single-file.png"), tr("Single File Conversion"));
    mTabWidget->addTab(mBatchConvertWidget, QIcon(":/res/multiple-files.png"), tr("Batch Conversion"));
    mTabWidget->setCurrentIndex(mSettings.value("CurrentTabIndex", 0).toInt());

    this->setCentralWidget(mTabWidget);

    QObject::connect(mTabWidget, SIGNAL(currentChanged(int)), this, SLOT(saveTabIndex(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::saveTabIndex(int index)
{
    mSettings.setValue("CurrentTabIndex", index);
}
