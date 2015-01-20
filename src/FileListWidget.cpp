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

#include "FileListWidget.hpp"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFileInfo>

FileListWidget::FileListWidget(bool removeAction, QWidget* parent):
    QTableWidget(parent), mRemoveAction(removeAction)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->verticalHeader()->hide();
    this->setShowGrid(false);

    if (!mRemoveAction)
    {
        this->setColumnCount(1);
        this->setHorizontalHeaderLabels(QStringList(tr("Name")));
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        this->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
#else
        this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
#endif
    }
    else
    {
        this->setColumnCount(2);
        QStringList headerLabels;
        headerLabels << tr("Name") << "";
        this->setHorizontalHeaderLabels(headerLabels);
        this->setColumnWidth(1, 32);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        this->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
        this->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
#else
        this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
#endif
    }

    QObject::connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(removeSingleFile(int, int)));
}

FileListWidget::~FileListWidget()
{
}

void FileListWidget::addFile(const QString& path)
{
    QFileInfo file(path);

    // check if file existed
    Q_FOREACH(const QFileInfo& info, mFileList)
    {
        if (info.filePath() == path)
        {
            return;
        }
    }

    if (!file.isReadable())
        return;

    mFileList.append(file);

    int row = this->rowCount();
    this->setRowCount(row + 1);

    // append to table view
    QTableWidgetItem* fileItem = new QTableWidgetItem(
        QIcon(":/res/text-file.png"),
        file.fileName(),
        QTableWidgetItem::UserType
    );
    fileItem->setData(Qt::UserRole, file.filePath());
    this->setItem(row, 0, fileItem);

    if (mRemoveAction)
    {
        QTableWidgetItem* actionItem = new QTableWidgetItem(
            QIcon(":/res/remove.png"),
            QString(),
            QTableWidgetItem::UserType
        );
        actionItem->setData(Qt::UserRole, file.filePath());
        actionItem->setToolTip(tr("Remove %1").arg(file.fileName()));
        this->setItem(row, 1, actionItem);
    }
}

// public slots
void FileListWidget::removeSelectedFiles()
{
    QList<QTableWidgetItem*> selectedItems = this->selectedItems();
    QList<int> selectedRows;

    Q_FOREACH(QTableWidgetItem* item, selectedItems)
    {
        // skip duplicate row
        if (!item || item->column() != 0)
            continue;

        int row = this->row(item);
        selectedRows.append(row);
        QString filePath = item->data(Qt::UserRole).toString();
        mFileList.removeAll(QFileInfo(filePath));
    }

    // sort selected rows to descending order
    qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>());

    // remove row by descending order
    Q_FOREACH(int row, selectedRows)
    {
        this->model()->removeRow(row);
    }
}

// private slots
void FileListWidget::removeSingleFile(int row, int column)
{
    if (!mRemoveAction || column != 1)
        return;

    // remove row from table widget
    QTableWidgetItem* fileItem = this->item(row, 0);
    if (!fileItem)
        return;

    QString filePath = fileItem->data(Qt::UserRole).toString();
    this->model()->removeRow(row);

    mFileList.removeAll(QFileInfo(filePath));
}

