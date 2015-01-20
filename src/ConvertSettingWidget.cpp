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

#include "TextConverter.hpp"
#include "ConvertSettingWidget.hpp"
#include <QString>
#include <QList>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

ConvertSettingWidget::ConvertSettingWidget(QWidget* parent):
    QWidget(parent)
{
    QLabel* convertMethodLabel = new QLabel(tr("Conversion:"));
    mConvertMethod = new QComboBox();
    mConvertMethod->addItem(tr("Simplified to Traditional"), "s2t");
    mConvertMethod->addItem(tr("Traditional to Simplified"), "t2s");
    mConvertMethod->setToolTip(tr("Choose conversion direction"));

    QLabel* convertOptionLabel = new QLabel(tr("Option:"));
    mConvertOption = new QComboBox();
    mConvertOption->setToolTip(tr("Choose conversion option"));

    QLabel* convertEncodingLabel = new QLabel(tr("Encoding:"));
    mConvertCharset = new QComboBox();
    mConvertCharset->setToolTip(tr("Choose conversion encoding"));

    this->updateOptions();

    QHBoxLayout* layout = new QHBoxLayout();

    layout->addWidget(convertMethodLabel);
    layout->addWidget(mConvertMethod);
    layout->addWidget(convertOptionLabel);
    layout->addWidget(mConvertOption);
    layout->addWidget(convertEncodingLabel);
    layout->addWidget(mConvertCharset);
    layout->addStretch(1);

    this->setLayout(layout);

    QObject::connect(mConvertMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(methodChanged(int)));
    QObject::connect(mConvertOption, SIGNAL(currentIndexChanged(int)), this, SLOT(optionChanged(int)));
}

ConvertSettingWidget::~ConvertSettingWidget()
{

}
QString ConvertSettingWidget::getMethod()
{
    return mConvertMethod->itemData(mConvertMethod->currentIndex()).toString();
}

QString ConvertSettingWidget::getOption()
{
    return mConvertOption->itemData(mConvertOption->currentIndex()).toString();
}

QString ConvertSettingWidget::getCharset()
{
    return mConvertCharset->itemData(mConvertCharset->currentIndex()).toString();
}

void ConvertSettingWidget::setMethod(const QString& method)
{
    mConvertMethod->setCurrentIndex(mConvertMethod->findData(method));
}

// private methods
void ConvertSettingWidget::updateOptions()
{
    QString method = this->getMethod();

    mConvertOption->clear();
    mConvertCharset->clear();
    if (method == "s2t")
    {
        mConvertOption->addItem(tr("Standard"), ":/config/s2t.json");
        mConvertOption->setItemData(0, tr("Simplified Chinese to Traditional Chinese"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Taiwan Standard"), ":/config/s2tw.json");
        mConvertOption->setItemData(1, tr("Simplified Chinese to Traditional Chinese (Taiwan Standard)"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Taiwan Standard with Taiwanese Idiom"), ":/config/s2twp.json");
        mConvertOption->setItemData(2, tr("Simplified Chinese to Traditional Chinese (Taiwan Standard) with Taiwanese idiom"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Hong Kong Standard"), ":/config/s2hk.json");
        mConvertOption->setItemData(3, tr("Simplified Chinese to Traditional Chinese (Hong Kong Standard)"), Qt::ToolTipRole);

        QStringList charsetList = TextConverter::getCharsetList(QLocale::Taiwan);
        Q_FOREACH(const QString& charset, charsetList)
        {
            mConvertCharset->addItem(charset, charset);
        }
    }
    else
    {
        mConvertOption->addItem(tr("Standard"), ":/config/t2s.json");
        mConvertOption->setItemData(0, tr("Traditional Chinese to Simplified Chinese"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Taiwan Standard to Simplified Chinese"), ":/config/tw2s.json");
        mConvertOption->setItemData(1, tr("Traditional Chinese (Taiwan Standard) to Simplified Chinese"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Taiwan Standard to Mainland Chinese Idiom"), ":/config/tw2sp.json");
        mConvertOption->setItemData(2, tr("Traditional Chinese (Taiwan Standard) to Simplified Chinese with Mainland Chinese idiom"), Qt::ToolTipRole);
        mConvertOption->addItem(tr("Hong Kong Standard to Simplified Chinese"), ":/config/hk2s.json");
        mConvertOption->setItemData(3, tr("Traditional Chinese (Hong Kong Standard) to Simplified Chinese"), Qt::ToolTipRole);

        QStringList charsetList = TextConverter::getCharsetList(QLocale::China);
        Q_FOREACH(const QString& charset, charsetList)
        {
            mConvertCharset->addItem(charset, charset);
        }
    }
}

// private slots
void ConvertSettingWidget::methodChanged(int index)
{
    this->updateOptions();
}

void ConvertSettingWidget::optionChanged(int index)
{
    if (index == -1)
        return;
    emit settingChanged();
}

