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

#ifndef _SINGLE_CONVERT_WIDGET_HPP
#define _SINGLE_CONVERT_WIDGET_HPP
#include <QString>
#include <QMap>
#include <QWidget>
#include "PersonalSettings.hpp"

// Forward declarations
class QVBoxLayout;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QPlainTextEdit;
class QScrollBar;
class QStatusBar;
class ConvertSettingWidget;

class SingleConvertWidget: public QWidget
{
    Q_OBJECT
public:
    explicit SingleConvertWidget(QStatusBar* statusBar, QWidget* parent);
    ~SingleConvertWidget();

private:
    void setDestContentVisible(bool visible);
    void convertSrcContent();
    void previewConvertResult();

    PersonalSettings mSettings;

    QStatusBar* mStatusBar;

    QPushButton* mLoadFileButton;
    QLineEdit* mSrcFileLineEdit;
    QComboBox* mSrcCharsetSelector;

    ConvertSettingWidget* mConvertSettings;

    QVBoxLayout* mSrcContentLayout;
    QCheckBox* mPreviewCheckBox;
    QPushButton* mRefreshPreviewButton;
    QPlainTextEdit* mSrcTextEdit;
    QScrollBar* mSrcScrollBar;

    QVBoxLayout* mDstContentLayout;
    QPlainTextEdit* mDstTextEdit;
    QScrollBar* mDstScrollBar;

    QPushButton* mConvertFileButton;
    QCheckBox* mOverwriteCheckBox;

    QString mSrcFile;
    QString mSrcContent;

    QString mDstFile;
    QString mDstContent;

    qint64 mConvertElapsedMsec;

private slots:
    void loadFile();
    void loadSrcContent();
    void updateSrcContent();
    void refreshPreview();
    void convertFile();
    void convertSettingChanged();
    void previewStateChanged(bool checked);
    void overwriteModeChanged(bool checked);
    void syncTextEditCursor();
    void syncTextEditScrollBar(int value);
};
#endif // _SINGLE_CONVERT_WIDGET_HPP
