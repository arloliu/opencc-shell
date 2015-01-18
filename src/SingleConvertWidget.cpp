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

#include "PersonalSettings.hpp"
#include "ConvertSettingWidget.hpp"
#include "SingleConvertWidget.hpp"
#include "TextLoader.hpp"
#include "TextConverter.hpp"
#include <QMap>
#include <QElapsedTimer>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QFileInfo>
#include <QFileDialog>

SingleConvertWidget::SingleConvertWidget(QStatusBar* statusBar, QWidget* parent = 0):
    QWidget(parent), mStatusBar(statusBar)
{
    QVBoxLayout* outlineLayout = new QVBoxLayout();
    QHBoxLayout* srcFileLayout = new QHBoxLayout();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QHBoxLayout* actionLayout = new QHBoxLayout();


    // create source file components
    mLoadFileButton = new QPushButton(QIcon(":/res/open.png"), tr("&Open"));
    mSrcFileLineEdit = new QLineEdit();
    mSrcFileLineEdit->setMaxLength(255);
    mSrcFileLineEdit->setMaximumWidth(600);
    mSrcFileLineEdit->setReadOnly(true);

    // add widgets to source file layout
    srcFileLayout->addWidget(mLoadFileButton, 0);
    srcFileLayout->addWidget(mSrcFileLineEdit, 20);
    srcFileLayout->addStretch(1);

    // create convert setting widget
    mConvertSettings = new ConvertSettingWidget();

    // create src. widgets
    QLabel* srcContentLabel = new QLabel(tr("Original Content:"));
    mPreviewCheckBox = new QCheckBox(tr("Preview Converted Result"));
    bool previewState = mSettings.value("SingleMode/PreviewState", true).toBool();
    mPreviewCheckBox->setChecked(previewState);

    mSrcTextEdit = new QPlainTextEdit();
    mSrcScrollBar = mSrcTextEdit->verticalScrollBar();

    QHBoxLayout* srcOptionLayout = new QHBoxLayout();
    srcOptionLayout->addWidget(srcContentLabel);
    srcOptionLayout->addWidget(mPreviewCheckBox);
    srcOptionLayout->addStretch(1);

    // create src. layout and add widgets to it
    mSrcContentLayout = new QVBoxLayout();
    mSrcContentLayout->addLayout(srcOptionLayout);
    mSrcContentLayout->addWidget(mSrcTextEdit);

    // create dest. widgets
    mDstTextEdit = new QPlainTextEdit();
    mDstScrollBar = mDstTextEdit->verticalScrollBar();
    QLabel* dstContentLabel = new QLabel(tr("Converted Content:"));

    // create dest. layout and add widgets to it
    mDstContentLayout = new QVBoxLayout();
    mDstContentLayout->addWidget(dstContentLabel);
    mDstContentLayout->addWidget(mDstTextEdit);

    // add src & dst layout to main layout
    mainLayout->addLayout(mSrcContentLayout);
    mainLayout->addLayout(mDstContentLayout);


    // create action components
    mConvertFileButton = new QPushButton(QIcon(":/res/convert.png"), QString(" ") + tr("&Convert && Save"));
    mConvertFileButton->setIconSize(QSize(32, 32));
    mConvertFileButton->setStyleSheet("QPushButton {font-size: 16px; font-weight: bold;}");
    mOverwriteCheckBox = new QCheckBox(tr("Overwrite Original File"));
    mOverwriteCheckBox->setChecked(mSettings.value("SingleMode/OverwriteMode", false).toBool());

    // add action components to action layout
    actionLayout->addStretch(1);
    actionLayout->addWidget(mConvertFileButton);
    actionLayout->addWidget(mOverwriteCheckBox);
    actionLayout->addStretch(1);

    // add tool/main/action layouts to outline layout
    outlineLayout->addLayout(srcFileLayout);
    outlineLayout->addWidget(mConvertSettings);
    outlineLayout->addLayout(mainLayout);
    outlineLayout->addLayout(actionLayout);
    this->setLayout(outlineLayout);

    QObject::connect(mLoadFileButton, SIGNAL(clicked(bool)), this, SLOT(loadFile()));
    QObject::connect(mConvertFileButton, SIGNAL(clicked(bool)), this, SLOT(convertFile()));
    QObject::connect(mConvertSettings, SIGNAL(settingChanged()), this, SLOT(convertSettingChanged()));
    QObject::connect(mPreviewCheckBox, SIGNAL(toggled(bool)), this, SLOT(previewStateChanged(bool)));
    QObject::connect(mOverwriteCheckBox, SIGNAL(toggled(bool)), this, SLOT(overwriteModeChanged(bool)));


    // post processing
    previewStateChanged(mPreviewCheckBox->isChecked());

    // sync src & dst scrollbar
    QObject::connect(mSrcScrollBar, SIGNAL(valueChanged(int)), this, SLOT(syncTextEditScrollBar(int)));
    QObject::connect(mDstScrollBar, SIGNAL(valueChanged(int)), this, SLOT(syncTextEditScrollBar(int)));
    QObject::connect(mSrcTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(syncTextEditCursor()));
    QObject::connect(mDstTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(syncTextEditCursor()));

}

SingleConvertWidget::~SingleConvertWidget()
{
}

void SingleConvertWidget::setDestContentVisible(bool visible)
{
    for (int i = 0; i < mDstContentLayout->count(); ++i)
    {
        QLayoutItem* item = mDstContentLayout->itemAt(i);
        item->widget()->setVisible(visible);
    }
}

void SingleConvertWidget::convertSrcContent()
{
    QString config = mConvertSettings->getOption();
    QElapsedTimer timer;
    timer.start();
    TextConverter* converter = new TextConverter(config);
    mDstContent = converter->convertToUtf8(mSrcContent);
    delete converter;
    mConvertElapsedMsec = timer.elapsed();
}

void SingleConvertWidget::previewConvertResult()
{
    this->convertSrcContent();
    mDstTextEdit->setPlainText(mDstContent);
    mStatusBar->showMessage(tr("%1 converted, took %2 secs").arg(mSrcFile).arg((float)(mConvertElapsedMsec/1000.0)));
}

// slots
void SingleConvertWidget::loadFile()
{
    QFileDialog dialog(this, tr("Open Text File"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFileMode(QFileDialog::ExistingFile);

    QString previousDirectory = mSettings.value("SingleMode/LoadDirectory").toString();
    if (!previousDirectory.isEmpty())
        dialog.setDirectory(previousDirectory);

    if (!dialog.exec())
        return;

    mSrcFile = dialog.selectedFiles().at(0);
    mSrcFileLineEdit->setText(mSrcFile);
    mStatusBar->showMessage(tr("%1 loaded").arg(mSrcFile));

    QFileInfo srcFileInfo(mSrcFile);
    mSettings.setValue("SingleMode/LoadDirectory", srcFileInfo.canonicalPath());

    TextLoader loader(mSrcFile);
    mSrcContent = loader.content();
    mSrcCharset = loader.charset();
    mSrcTextEdit->setPlainText(mSrcContent);

    QLocale::Country langType = TextConverter::getLocaleCountry(mSrcContent, mSrcCharset);
    QString previousMethod = mConvertSettings->getMethod();
    QString currentMethod;

    if (langType == QLocale::Taiwan)
        currentMethod = "t2s";
    else
        currentMethod = "s2t";

    mConvertSettings->setMethod(currentMethod);

    if (currentMethod == previousMethod)
    {
        this->convertSettingChanged();
    }
}

void SingleConvertWidget::convertFile()
{
    if (mSrcContent.isEmpty())
        return;

    bool overwrite = mOverwriteCheckBox->isChecked();
    mSettings.setValue("SingleMode/OverwriteMode", overwrite);

    if (!overwrite)
    {
        QFileDialog dialog(this, tr("Save File"));
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setViewMode(QFileDialog::Detail);
        if (!dialog.exec())
            return;
        mDstFile = dialog.selectedFiles().at(0);
    }
    else
    {
        mDstFile = mSrcFile;
    }

    QElapsedTimer timer;
    timer.start();

    // convert source content to mDstContent
    this->convertSrcContent();

    // convert mDstContent to final content
    QString dstCharset = mConvertSettings->getCharset();
    QByteArray finalContent;
    if (dstCharset != "UTF-8")
        finalContent = TextConverter::convertFromUtf8(dstCharset, mDstContent);
    else
        finalContent = mDstContent.toUtf8();

    // save file
    QFile dstFile(mDstFile);
    if (!dstFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // TODO: show error dialog
        mStatusBar->showMessage(tr("Can't open file: %1").arg(mDstFile));
    }

    if (dstFile.write(finalContent) == -1)
    {
        // TODO: show error dialog
        mStatusBar->showMessage(tr("Can't save file: %1").arg(mDstFile));
    }
    dstFile.close();
    mDstContent = QString();

    qint64 totalElapssed = timer.elapsed();
    mStatusBar->showMessage(tr("%1 saved, took %2 secs").arg(mDstFile).arg((float)(totalElapssed/1000.0)));

}

void SingleConvertWidget::convertSettingChanged()
{
    if (!mSrcContent.isEmpty())
    {
        if (mPreviewCheckBox->isChecked())
        {
            previewConvertResult();
        }
        else
        {
            mDstContent = QString();
            mDstTextEdit->setPlainText(mDstContent);
        }
    }
}

void SingleConvertWidget::previewStateChanged(bool checked)
{
    if (checked)
    {
        if (!mSrcContent.isEmpty() && mDstContent.isEmpty())
            previewConvertResult();
    }
    mSettings.setValue("SingleMode/PreviewState", checked);

    this->setDestContentVisible(checked);
}

void SingleConvertWidget::overwriteModeChanged(bool checked)
{
    mSettings.setValue("SingleMode/OverwriteMode", checked);
}

void SingleConvertWidget::syncTextEditCursor()
{
    if ((void*)(this->sender()) == (void*)mSrcTextEdit)
        mDstScrollBar->setValue(mSrcScrollBar->value());
    else
        mSrcScrollBar->setValue(mDstScrollBar->value());
}

void SingleConvertWidget::syncTextEditScrollBar(int value)
{
    QObject* sender = this->sender();
    if ((void*)sender == (void*)mSrcScrollBar)
        mDstScrollBar->setValue(value);
    else
        mSrcScrollBar->setValue(value);
}
