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
#include "ConvertSettingWidget.hpp"
#include "BatchConvertWidget.hpp"
#include "TextLoader.hpp"
#include "TextConverter.hpp"
#include <QString>
#include <QFileInfo>
#include <QFileDialog>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCompleter>
#include <QDirModel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtConcurrentMap>
#include <QFutureWatcher>

// private static instance
TextConverter* BatchConvertWidget::mConverter = 0;
QString BatchConvertWidget::mSrcCharset = "auto";
QString BatchConvertWidget::mDstCharset = QString();

BatchConvertWidget::BatchConvertWidget(QStatusBar* statusBar, QWidget* parent):
    QWidget(parent), mStatusBar(statusBar)
{
    // conncurrent task watcher
    mTaskWatcher = new QFutureWatcher<bool>(this);
    QObject::connect(mTaskWatcher, SIGNAL(finished()), this, SLOT(batchConvertFinished()));

    mOutlineLayout = new QVBoxLayout();
    mOutlineLayout->setAlignment(Qt::AlignTop);

    this->setLayout(mOutlineLayout);

    createFolderComponents();
    createConvertMethodComponents();
    createAssignComponents();
    createPreviewComponents();
    createActionComponents();

    updateSrcDir();
    updateDstDir();
}

BatchConvertWidget::~BatchConvertWidget()
{
    mTaskWatcher->cancel();
    mTaskWatcher->waitForFinished();
}

bool BatchConvertWidget::batchConvert(const ConvertTask& task)
{
    // load file content;
    TextLoader loader(task.first, mSrcCharset);
    QString srcContent = loader.content();

    // convert source content to mDstContent
    QString dstContent = mConverter->convertToUtf8(srcContent);
    if (dstContent.isEmpty())
        return false;

    // convert to final content
    QByteArray finalContent;
    if (mDstCharset != "UTF-8")
        finalContent = TextConverter::convertFromUtf8(mDstCharset, dstContent);
    else
        finalContent = dstContent.toUtf8();

    // save file
    QFile dstFile(task.second);
    if (!dstFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    if (dstFile.write(finalContent) == -1)
    {
        return false;
    }
    dstFile.close();

    return true;
}

void BatchConvertWidget::createFolderComponents()
{
    QHBoxLayout* srcDirLayout = new QHBoxLayout();
    QHBoxLayout* dstDirLayout = new QHBoxLayout();

    QCompleter* pathCompleter = new QCompleter(this);
    pathCompleter->setModel(new QDirModel(pathCompleter));

    QLabel* srcDirLabel = new QLabel(tr("Source Folder:"));

    srcDirLabel->setMinimumWidth(100);
    srcDirLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    mSrcDirOpenButton = new QPushButton(QIcon(":/res/open.png"), QString());
    mSrcDirOpenButton->setToolTip(tr("Open Source Folder"));

    mSrcDirInfo = new QLineEdit();
    mSrcDirInfo->setMaxLength(255);
    mSrcDirInfo->setReadOnly(false);
    mSrcDirInfo->setCompleter(pathCompleter);
    mSrcDirInfo->setText(mSettings.value("BatchMode/SourceDirectory").toString());

    QLabel* srcDirFilterLabel = new QLabel(tr("Filter:"));
    mSrcDirFilter = new QComboBox();
    mSrcDirFilter->addItem(tr("Any Files"), "*");
    mSrcDirFilter->addItem(tr("Text files (*.txt)"), "*.txt");
    mSrcDirFilter->addItem(tr("HTML files (*.html *.htm *.xhtml)"), "*.html *.htm *.xhtml");
    mSrcDirFilter->setMaximumWidth(120);

    QLabel* srcCharsetLabel = new QLabel(tr("Encoding:"));
    mSrcCharsetSelector = new QComboBox();
    mSrcCharsetSelector->addItem(tr("Auto Detect"), "auto");
    mSrcCharsetSelector->addItem("UTF-8", "UTF-8");
    mSrcCharsetSelector->addItem("Big5", "Big5");
    mSrcCharsetSelector->addItem("GB18030", "GB18030");
    mSrcCharsetSelector->addItem("GBK", "GBK");
    mSrcCharsetSelector->addItem("GB2312", "GB2312");

    mSrcDir.setFilter(QDir::Files);
    mSrcDir.setSorting(QDir::Name);

    srcDirLayout->addWidget(srcDirLabel);
    srcDirLayout->addWidget(mSrcDirOpenButton);
    srcDirLayout->addWidget(mSrcDirInfo, 20);
    srcDirLayout->addWidget(srcCharsetLabel);
    srcDirLayout->addWidget(mSrcCharsetSelector);
    srcDirLayout->addWidget(srcDirFilterLabel);
    srcDirLayout->addWidget(mSrcDirFilter);
    srcDirLayout->addStretch(1);

    // create components for dest. layout
    QLabel* dstDirLabel = new QLabel(tr("Dest. Folder:"));
    dstDirLabel->setMinimumWidth(100);
    dstDirLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    mSetDstDirButton = new QPushButton(QIcon(":/res/open.png"), QString());
    mSetDstDirButton->setToolTip(tr("Set Destination Folder"));

    mDstDirInfo = new QLineEdit();
    mDstDirInfo->setMaxLength(255);
    mDstDirInfo->setReadOnly(false);
    mDstDirInfo->setCompleter(pathCompleter);
    mDstDirInfo->setText(mSettings.value("BatchMode/DestinationDirectory").toString());

    mSetDstAsSrcButton = new QPushButton(QIcon(":/res/update.png"), QString());

    dstDirLayout->addWidget(dstDirLabel);
    dstDirLayout->addWidget(mSetDstDirButton);
    dstDirLayout->addWidget(mDstDirInfo, 20);
    dstDirLayout->addWidget(mSetDstAsSrcButton);
    dstDirLayout->addStretch(1);

    mOutlineLayout->addLayout(srcDirLayout);
    mOutlineLayout->addLayout(dstDirLayout);

    QObject::connect(mSrcDirOpenButton, SIGNAL(clicked(bool)), this, SLOT(openSrcDir()));
    QObject::connect(mSrcCharsetSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(reloadPreviewFile()));
    QObject::connect(mSrcDirFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSrcFileList()));
    QObject::connect(mSrcDirInfo, SIGNAL(returnPressed()), this, SLOT(updateSrcDir()));

    QObject::connect(mSetDstDirButton, SIGNAL(clicked(bool)), this, SLOT(openDstDir()));
    QObject::connect(mDstDirInfo, SIGNAL(returnPressed()), this, SLOT(updateDstDir()));
    QObject::connect(mSetDstAsSrcButton, SIGNAL(clicked(bool)), this, SLOT(setDstAsSrcDir()));
}

void BatchConvertWidget::createConvertMethodComponents()
{
    mConvertSettings = new ConvertSettingWidget();
    mOutlineLayout->addWidget(mConvertSettings);
}

void BatchConvertWidget::createAssignComponents()
{
    QGroupBox* groupBox = new QGroupBox(tr("Assign Files for Conversion"));
    groupBox->setFlat(false);

    QHBoxLayout* assignLayout = new QHBoxLayout();

    mSrcDirFileList = new FileListWidget(false);
    mDstFileList = new FileListWidget(true);

    QVBoxLayout* assignButtonLayout = new QVBoxLayout();
    mAddSelected = new QPushButton(QIcon(":/res/add-one.png"), tr("Add"));
    mAddAll = new QPushButton(QIcon(":/res/add-all.png"), tr("Add All"));
    mRemoveSelected = new QPushButton(QIcon(":/res/remove-one.png"), tr("Remove"));
    mRemoveAll = new QPushButton(QIcon(":/res/remove-all.png"), tr("Remove All"));

    QString assignButtonStyle = "QPushButton {text-align: left; padding: 4px 8px; outline: none;} ";
    mAddSelected->setStyleSheet(assignButtonStyle);
    mAddAll->setStyleSheet(assignButtonStyle);
    mRemoveSelected->setStyleSheet(assignButtonStyle);
    mRemoveAll->setStyleSheet(assignButtonStyle);

    assignButtonLayout->addSpacing(24);
    assignButtonLayout->addWidget(mAddSelected);
    //assignButtonLayout->addWidget(mAddAll);
    assignButtonLayout->addWidget(mRemoveSelected);
    //assignButtonLayout->addWidget(mRemoveAll);
    assignButtonLayout->addStretch(1);

    assignLayout->addWidget(mSrcDirFileList);
    assignLayout->addLayout(assignButtonLayout);
    assignLayout->addWidget(mDstFileList);

    groupBox->setLayout(assignLayout);
    mOutlineLayout->addWidget(groupBox);


    QObject::connect(mAddSelected, SIGNAL(clicked(bool)), this, SLOT(addSelectedFiles()));
    QObject::connect(mRemoveSelected, SIGNAL(clicked(bool)), mDstFileList, SLOT(removeSelectedFiles()));
}

void BatchConvertWidget::createPreviewComponents()
{
    mPreviewContent = new QGroupBox(tr("Content Preview"));
    mPreviewContent->setMinimumHeight(160);
    mPreviewContent->setCheckable(true);
    mPreviewContent->setChecked(mSettings.value("BatchMode/PreviewState", true).toBool());
    mPreviewContent->setFlat(false);

    QHBoxLayout* previewLayout = new QHBoxLayout();

    mSrcPreviewTextEdit = new QPlainTextEdit();
    mDstPreviewTextEdit = new QPlainTextEdit();
    mSrcPreviewTextEdit->setAcceptDrops(false);
    mDstPreviewTextEdit->setAcceptDrops(false);

    mSrcPreviewScrollBar = mSrcPreviewTextEdit->verticalScrollBar();
    mDstPreviewScrollBar = mDstPreviewTextEdit->verticalScrollBar();

    previewLayout->addWidget(mSrcPreviewTextEdit);
    previewLayout->addWidget(mDstPreviewTextEdit);

    mPreviewContent->setLayout(previewLayout);

    mOutlineLayout->addWidget(mPreviewContent, 1);


    QObject::connect(mSrcDirFileList, SIGNAL(cellClicked(int, int)), this, SLOT(previewFile()));
    QObject::connect(mPreviewContent, SIGNAL(toggled(bool)), this, SLOT(previewStateChanged(bool)));
    QObject::connect(mConvertSettings, SIGNAL(settingChanged()), this, SLOT(convertSettingChanged()));

    // sync src & dst scrollbar
    QObject::connect(mSrcPreviewScrollBar, SIGNAL(valueChanged(int)), this, SLOT(syncPreviewScrollBar(int)));
    QObject::connect(mDstPreviewScrollBar, SIGNAL(valueChanged(int)), this, SLOT(syncPreviewScrollBar(int)));
    QObject::connect(mSrcPreviewTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(syncPreviewCursor()));
    QObject::connect(mDstPreviewTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(syncPreviewCursor()));

}

void BatchConvertWidget::createActionComponents()
{
    QHBoxLayout* actionLayout = new QHBoxLayout();

    mConvertFileButton = new QPushButton(QIcon(":/res/convert.png"), QString(" ") + tr("Start Batch &Convert"));
    mConvertFileButton->setIconSize(QSize(32, 32));
    mConvertFileButton->setStyleSheet("QPushButton {font-size: 16px; font-weight: bold;}");

    actionLayout->addStretch(1);
    actionLayout->addWidget(mConvertFileButton);
    actionLayout->addStretch(1);

    mOutlineLayout->addLayout(actionLayout);

    mProgressDialog = new QProgressDialog(this);
    mProgressDialog->setWindowTitle(tr("Converting Files..."));
    mProgressDialog->setWindowModality(Qt::WindowModal);

    QObject::connect(mProgressDialog, SIGNAL(canceled()), mTaskWatcher, SLOT(cancel()));
    QObject::connect(mTaskWatcher, SIGNAL(progressRangeChanged(int, int)), mProgressDialog, SLOT(setRange(int, int)));
    QObject::connect(mTaskWatcher, SIGNAL(progressRangeChanged(int, int)), mProgressDialog, SLOT(setRange(int, int)));
    QObject::connect(mTaskWatcher, SIGNAL(progressValueChanged(int)), mProgressDialog, SLOT(setValue(int)));
    QObject::connect(mTaskWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(setProgressText(int)));


    QObject::connect(mConvertFileButton, SIGNAL(clicked()), this, SLOT(convertFiles()));
}

QStringList BatchConvertWidget::getSrcNameFilters()
{
    return mSrcDirFilter->itemData(mSrcDirFilter->currentIndex()).toString().split(" ");
}

bool BatchConvertWidget::fixDirPath(QDir& dir, QLineEdit* info, bool forceMode)
{
    QString path = info->text().trimmed();

    if (path.isEmpty())
        return false;

    if (path.startsWith("~/"))
    {
        path = path.replace(0, 1, QDir::homePath());
    }

    int retval = true;
    QFileInfo dirInfo(path);
    if (!dirInfo.isDir())
    {
        if (!forceMode)
            return false;
        retval = false;
    }

    if (forceMode)
    {
        dir.setPath(dirInfo.absoluteFilePath());
        info->setText(dirInfo.absoluteFilePath());
    }
    else
    {
        dir.setPath(dirInfo.canonicalFilePath());
        info->setText(dirInfo.canonicalFilePath());
    }
    return retval;
}

bool BatchConvertWidget::openDirDialog(const QString& dialogName, const QString& dir, QString& selectedDir)
{
    QFileDialog dialog(this, dialogName);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFileMode(QFileDialog::DirectoryOnly);

    if (!dir.isEmpty())
        dialog.setDirectory(dir);

    bool retval = dialog.exec();
    if (retval)
        selectedDir = dialog.selectedFiles().at(0);

    return retval;
}

void BatchConvertWidget::convertPreviewContent()
{
    QString config = mConvertSettings->getOption();
    TextConverter converter(config);
    mDstPreviewTextEdit->setPlainText(converter.convertToUtf8(mSrcPreviewContent));
}

bool BatchConvertWidget::confirmOverwrite()
{
    // respect dest. dir which user inputed
    mDstDir.setPath(mDstDirInfo->text().trimmed());

    QFileInfo dstDirInfo(mDstDir.canonicalPath());
    if (!dstDirInfo.isDir())
    {
        QMessageBox::warning(this, tr("Conversion Warning"), tr("Dest. folder doesn't exist"));
        return false;
    }
    if (!dstDirInfo.isWritable())
    {
        QMessageBox::warning(this, tr("Conversion Warning"), tr("Doesn't have permission to access dest. folder"));
        return false;
    }

    bool exists = false;
    QFileInfoList dstFileList = mDstFileList->getDstFileList();
    Q_FOREACH(const QFileInfo& srcInfo, dstFileList)
    {
        QString srcPath = srcInfo.filePath();
        QFileInfo dstInfo(mDstDir.absoluteFilePath(srcInfo.fileName()));
        if (dstInfo.exists())
        {
            exists = true;
            break;
        }
    }

    if (exists)
    {
        int ret = QMessageBox::information(this,
            tr("Overwrite Confirm"),
            tr("Some files already existed in dest. folder, Do you want to overwrite these files?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );
        if (ret == QMessageBox::No)
            return false;
    }

    return true;
}

// private slots
void BatchConvertWidget::openSrcDir()
{
    this->fixDirPath(mSrcDir, mSrcDirInfo);

    QString selectedDir;
    if (!this->openDirDialog(tr("Open Source Folder"), mSrcDirInfo->text(), selectedDir))
    {
        return;
    }

    mSrcDir.setPath(selectedDir);
    mSrcDirInfo->setText(mSrcDir.canonicalPath());

    mSettings.setValue("BatchMode/SourceDirectory", mSrcDir.canonicalPath());

    QStringList nameFilters = getSrcNameFilters();
    mSrcDir.setNameFilters(nameFilters);

    updateSrcFileList();
}

void BatchConvertWidget::openDstDir()
{
    this->fixDirPath(mDstDir, mDstDirInfo);

    QString selectedDir;
    if (!this->openDirDialog(tr("Set Destination Folder"), mDstDirInfo->text(), selectedDir))
    {
        return;
    }

    mDstDir.setPath(selectedDir);
    mDstDirInfo->setText(mSrcDir.canonicalPath());

    mSettings.setValue("BatchMode/DestinationDirectory", mDstDir.canonicalPath());
}

void BatchConvertWidget::updateSrcDir()
{
    if (!this->fixDirPath(mSrcDir, mSrcDirInfo))
        return;
    mSettings.setValue("BatchMode/SourceDirectory", mSrcDir.canonicalPath());
    this->updateSrcFileList();
}

void BatchConvertWidget::updateDstDir()
{
    this->fixDirPath(mDstDir, mDstDirInfo);
    mSettings.setValue("BatchMode/DestinationDirectory", mDstDir.canonicalPath());
}

void BatchConvertWidget::updateSrcFileList()
{
    mSrcDir.setNameFilters(getSrcNameFilters());
    QFileInfoList files = mSrcDir.entryInfoList();

    // clear all rows first
    while (mSrcDirFileList->rowCount() > 0)
    {
        mSrcDirFileList->removeRow(0);
    }

    mSrcDirFileList->setRowCount(files.size());
    int row = 0;
    Q_FOREACH(const QFileInfo& file, files)
    {
        QTableWidgetItem* item = new QTableWidgetItem(
            QIcon(":/res/text-file.png"),
            file.fileName(),
            QTableWidgetItem::UserType
        );
        item->setData(Qt::UserRole, file.canonicalFilePath());
        mSrcDirFileList->setItem(row, 0, item);

        ++row;
    }
}

void BatchConvertWidget::setDstAsSrcDir()
{
    if (!this->fixDirPath(mSrcDir, mSrcDirInfo))
        return;

    mDstDir.setPath(mSrcDirInfo->text());
    mDstDirInfo->setText(mSrcDirInfo->text());
}

void BatchConvertWidget::addSelectedFiles()
{
    QList<QTableWidgetItem*> selectedItems = mSrcDirFileList->selectedItems();
    Q_FOREACH(QTableWidgetItem* item, selectedItems)
    {
        QString filePath = item->data(Qt::UserRole).toString();
        mDstFileList->addFile(filePath);
    }
}

void BatchConvertWidget::previewFile()
{
    bool previewState = mPreviewContent->isChecked();
    if (!previewState)
    {
        mSrcPreviewTextEdit->setPlainText(QString());
        mDstPreviewTextEdit->setPlainText(QString());
        mSrcPreviewFile = QString();
        mSrcPreviewContent = QString();
        return;
    }

    QTableWidgetItem* srcItem = mSrcDirFileList->selectedItems().at(0);
    if (!srcItem)
    {
        mSrcPreviewTextEdit->setPlainText(QString());
        mDstPreviewTextEdit->setPlainText(QString());
        mSrcPreviewFile = QString();
        mSrcPreviewContent = QString();
        return;
    }

    QString srcFile = srcItem->data(Qt::UserRole).toString();

    if (mSrcPreviewFile == srcFile)
        return;
    mSrcPreviewFile = srcFile;

    QString srcCharset = mSrcCharsetSelector->itemData(mSrcCharsetSelector->currentIndex()).toString();
    TextLoader loader(mSrcPreviewFile, srcCharset);
    mSrcPreviewContent = loader.content();
    mSrcPreviewTextEdit->setPlainText(mSrcPreviewContent);

    this->convertPreviewContent();
}

void BatchConvertWidget::previewStateChanged(bool checked)
{
    mSettings.setValue("BatchMode/PreviewState", checked);
    this->previewFile();
}

void BatchConvertWidget::reloadPreviewFile()
{
    if (mSrcPreviewFile.isEmpty())
        return;
    QString srcCharset = mSrcCharsetSelector->itemData(mSrcCharsetSelector->currentIndex()).toString();
    TextLoader loader(mSrcPreviewFile, srcCharset);
    mSrcPreviewContent = loader.content();
    mSrcPreviewTextEdit->setPlainText(mSrcPreviewContent);

    this->convertPreviewContent();
}

void BatchConvertWidget::syncPreviewCursor()
{
    if ((void*)(this->sender()) == (void*)mSrcPreviewTextEdit)
        mDstPreviewScrollBar->setValue(mSrcPreviewScrollBar->value());
    else
        mSrcPreviewScrollBar->setValue(mDstPreviewScrollBar->value());
}

void BatchConvertWidget::syncPreviewScrollBar(int value)
{
    QObject* sender = this->sender();
    if ((void*)sender == (void*)mSrcPreviewScrollBar)
        mDstPreviewScrollBar->setValue(value);
    else
        mSrcPreviewScrollBar->setValue(value);
}

void BatchConvertWidget::convertSettingChanged()
{
    if (mSrcPreviewContent.isEmpty() || !mPreviewContent->isChecked())
        return;

    this->convertPreviewContent();
}

void BatchConvertWidget::convertFiles()
{
    if (mDstFileList->getDstFileCount() == 0)
        return;

    this->fixDirPath(mDstDir, mDstDirInfo, true);

    if (!this->confirmOverwrite())
    {
        return;
    }

    // setup static text converter
    if (mConverter)
        delete mConverter;
    mConverter = new TextConverter(mConvertSettings->getOption());

    // save src & dest. charset
    mSrcCharset = mSrcCharsetSelector->itemData(mSrcCharsetSelector->currentIndex()).toString();
    mDstCharset = mConvertSettings->getCharset();

    // fill convert task list for batch conversion
    QFileInfoList dstFileList = mDstFileList->getDstFileList();
    mBatchConvertTaskList.clear();
    Q_FOREACH(const QFileInfo& info, dstFileList)
    {
        QString srcPath = info.filePath();
        QString dstPath = mDstDir.absoluteFilePath(info.fileName());
        mBatchConvertTaskList << ConvertTask(srcPath, dstPath);
    }
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    mElapsedTimer.start();
    // start conversion
    QFuture<bool> future = QtConcurrent::mapped(mBatchConvertTaskList, batchConvert);
    mTaskWatcher->setFuture(future);

    mProgressDialog->exec();
}

void BatchConvertWidget::setProgressText(int index)
{
    QString progressText = QString(tr("Converting %2").arg(mBatchConvertTaskList.at(index).second));
    mProgressDialog->setLabelText(progressText);
}

void BatchConvertWidget::batchConvertFinished()
{
    qint64 elapsedMsecs = mElapsedTimer.elapsed();
    float elapsedSecs = elapsedMsecs / 1000.0;

    QFuture<bool> future = mTaskWatcher->future();
    QList<bool> results = future.results();
    int resultCount = future.resultCount();
    bool canceled = mTaskWatcher->isCanceled();

    if (canceled)
    {
        mStatusBar->showMessage(
            tr("Batch conversion canceled, %1 files converted, %2 left, took %3 secs")
                .arg(future.resultCount())
                .arg(mBatchConvertTaskList.size() - resultCount)
                .arg(elapsedSecs)
        );
    }
    else
    {
        int successCount = 0;
        Q_FOREACH(bool result, results)
        {
            if (result)
                successCount++;
        }
        if (successCount == resultCount)
        {
            mStatusBar->showMessage(
                tr("Batch conversion finished, %1 files converted, took %2 secs")
                .arg(successCount).arg(elapsedSecs)
            );
        }
        else
        {
            mStatusBar->showMessage(
                tr("Batch conversion finished, %1 files converted, %2 files failed, took %3 secs")
                    .arg(successCount)
                    .arg(resultCount - successCount)
                    .arg(elapsedSecs)
            );
        }
    }
}
