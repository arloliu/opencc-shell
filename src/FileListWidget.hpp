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

#ifndef _FILE_LIST_WIDGET_HPP
#define _FILE_LIST_WIDGET_HPP
#include <QString>
#include <QTableWidget>
#include <QFileInfo>

class FileListWidget: public QTableWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(bool removeAction, QWidget* parent = 0);
    ~FileListWidget();
    QFileInfoList getDstFileList() { return mFileList; }
    int getDstFileCount() { return mFileList.size(); }
    void addFile(const QString& path);

private:
    int mRemoveAction;
    QFileInfoList mFileList;

public slots:
    void removeSelectedFiles();

private slots:
    void removeSingleFile(int row, int column);
};
#endif // _FILE_LIST_WIDGET_HPP
