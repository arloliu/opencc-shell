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

#ifndef _BATCH_CONVERT_WIDGET_HPP
#define _BATCH_CONVERT_WIDGET_HPP
#include <QString>
#include <QList>
#include <QPair>
#include <QFileInfo>
#include <QDir>
#include <QWidget>
#include <QElapsedTimer>
#include <QFutureWatcher>
#include "PersonalSettings.hpp"

// Forward declarations
class QStatusBar;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QVBoxLayout;
class QPlainTextEdit;
class QScrollBar;
class QGroupBox;
class QProgressDialog;

class ConvertSettingWidget;
class FileListWidget;
class TextConverter;

class BatchConvertWidget: public QWidget
{
    Q_OBJECT
public:
    typedef QPair<QString, QString> ConvertTask;
    typedef QList<ConvertTask> ConvertTaskList;

    explicit BatchConvertWidget(QStatusBar* statusBar, QWidget* parent = 0);
    ~BatchConvertWidget();

    static bool batchConvert(const ConvertTask& task);

private:
    void createFolderComponents();
    void createConvertMethodComponents();
    void createAssignComponents();
    void createPreviewComponents();
    void createActionComponents();

    QStringList getSrcNameFilters();
    bool fixDirPath(QDir& dir, QLineEdit* info, bool forceMode = false);
    bool openDirDialog(const QString& dialogName, const QString& dir, QString& selectedDir);
    void convertPreviewContent();
    bool confirmOverwrite();

    PersonalSettings mSettings;

    QStatusBar* mStatusBar;

    QVBoxLayout* mOutlineLayout;

    QPushButton* mSrcDirOpenButton;
    QLineEdit* mSrcDirInfo;
    QComboBox* mSrcDirFilter;
    QComboBox* mSrcCharsetSelector;
    QDir mSrcDir;

    QPushButton* mSetDstDirButton;
    QPushButton* mSetDstAsSrcButton;
    QLineEdit* mDstDirInfo;
    QDir mDstDir;

    ConvertSettingWidget* mConvertSettings;

    FileListWidget* mSrcDirFileList;
    FileListWidget* mDstFileList;

    QPushButton* mAddSelected;
    QPushButton* mAddAll;
    QPushButton* mRemoveSelected;
    QPushButton* mRemoveAll;

    QGroupBox* mPreviewContent;
    QPlainTextEdit* mSrcPreviewTextEdit;
    QPlainTextEdit* mDstPreviewTextEdit;
    QScrollBar* mSrcPreviewScrollBar;
    QScrollBar* mDstPreviewScrollBar;
    QString mSrcPreviewFile;
    QString mSrcPreviewContent;

    QPushButton* mConvertFileButton;
    QProgressDialog* mProgressDialog;

    QFutureWatcher<bool>* mTaskWatcher;
    ConvertTaskList mBatchConvertTaskList;
    QElapsedTimer mElapsedTimer;

    static TextConverter* mConverter;
    static QString mSrcCharset;
    static QString mDstCharset;

private slots:
    void openSrcDir();
    void openDstDir();
    void updateSrcDir();
    void updateDstDir();
    void updateSrcFileList();
    void setDstAsSrcDir();
    void addSelectedFiles();
    void previewFile();
    void previewStateChanged(bool checked);
    void reloadPreviewFile();
    void syncPreviewCursor();
    void syncPreviewScrollBar(int value);
    void convertSettingChanged();
    void convertFiles();
    void setProgressText(int index);
    void batchConvertFinished();
};
#endif // _BATCH_CONVERT_WIDGET_HPP
