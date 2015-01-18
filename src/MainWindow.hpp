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

#ifndef _MAINWINDOW_HPP
#define _MAINWINDOW_HPP
#include <QMainWindow>
#include "PersonalSettings.hpp"

class QTabWidget;
class QStatusBar;
class SingleConvertWidget;
class BatchConvertWidget;

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    PersonalSettings mSettings;
    QTabWidget* mTabWidget;
    QStatusBar* mStatusBar;

    SingleConvertWidget* mSingleConvertWidget;
    BatchConvertWidget* mBatchConvertWidget;

private slots:
    void saveTabIndex(int index);
};
#endif // _MAINWINDOW_HPP
