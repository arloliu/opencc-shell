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
#include "TextConverter.hpp"
#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

// opencc header files
#include "Config.hpp"
#include "Converter.hpp"

QStringList TextConverter::mTwCharsetList = QStringList();
QStringList TextConverter::mCnCharsetList = QStringList();
QTextCodec* TextConverter::mUtf8Codec = QTextCodec::codecForName("UTF-8");
QTextCodec* TextConverter::mBig5Codec = QTextCodec::codecForName("Big5");
QTextCodec* TextConverter::mGbkCodec = QTextCodec::codecForName("GB2312");

TextConverter::TextConverter(const QString& configName, const QString& configDir):
    mInitialized(false), mConverter(0)
{
    mConfigPath = configName;

    // open & read config file content
    QFile configFile(mConfigPath);
    if (!configFile.exists() || !configFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray configContent = configFile.readAll();

    // fix config directory
    if (configDir.isEmpty())
    {
        QFileInfo configDirInfo = QFileInfo(mConfigPath);
        mConfigDir = configDirInfo.canonicalPath();
    }
    else
    {
        mConfigDir = configDir;
    }

    // try to create opencc conveter instance
    opencc::Config config;
    try
    {
        mConverter = config.NewFromString(QString(configContent).toStdString(), mConfigDir.toStdString());
    }
    catch(opencc::Exception& ex)
    {
        mErrorMessage = ex.what();
        return;
    }
    mInitialized = true;
}

TextConverter::~TextConverter()
{
}


QString TextConverter::convertToUtf8(QString& text)
{
    if (!mInitialized)
        return QString();

    QByteArray utf8_text = text.toUtf8();
    std::string converted;
    try
    {
        converted = mConverter->Convert(std::string(utf8_text));
    }
    catch (opencc::Exception& ex)
    {
        mErrorMessage = ex.what();
        return QString();
    }

    return QString::fromUtf8(converted.c_str());
}

// public static methods
QByteArray TextConverter::convertFromUtf8(const QString& charset, QString& text)
{
    if (charset.toUpper() == "UTF-8")
        return text.toUtf8();

    QTextCodec* codec = QTextCodec::codecForName(charset.toUtf8());

    return codec->fromUnicode(text);
}

QStringList TextConverter::getCharsetList(QLocale::Country langType)
{
    if (mTwCharsetList.isEmpty())
    {
        mTwCharsetList.append("UTF-8");
        mTwCharsetList.append("Big5");
        mTwCharsetList.append("Big5-HKSCS");
        mTwCharsetList.append("UTF-16");
        mTwCharsetList.append("UTF-16BE");
        mTwCharsetList.append("UTF-16LE");
    }
    if (mCnCharsetList.isEmpty())
    {
        mCnCharsetList.append("UTF-8");
        mCnCharsetList.append("GB18030");
        mCnCharsetList.append("GBK");
        mCnCharsetList.append("UTF-16");
        mCnCharsetList.append("UTF-16BE");
        mCnCharsetList.append("UTF-16LE");
    }
    if (langType == QLocale::Taiwan)
        return mTwCharsetList;

    return mCnCharsetList;
}

QLocale::Country TextConverter::getLocaleCountry(const QString& data, const QString& charset)
{
    QString charsetUpper = charset.toUpper();
    if (charsetUpper == "BIG5" || charsetUpper == "BIG5-HKSCS")
        return QLocale::Taiwan;
    else if (charsetUpper == "UTF-8")
        return getUtf8LocaleCountry(data);
    return QLocale::China;
}

// private static methods
QLocale::Country TextConverter::getUtf8LocaleCountry(const QString& data)
{
    // it's a dirty way to detect if UTF-8 text is a trad. or simp. chinese
    int length = data.size();
    QLocale::Country language = QLocale::Taiwan;
    for (int i = 0 ; i < length; ++i)
    {
        if (data.at(i) == '?')
            continue;
        QByteArray big5 = mBig5Codec->fromUnicode(data.at(i));
        QByteArray gbk = mGbkCodec->fromUnicode(data.at(i));
        QString big5_utf8 = mUtf8Codec->toUnicode(big5);
        QString gbk_utf8 = mUtf8Codec->toUnicode(gbk);
        if (gbk_utf8 == "?")
        {
            language = QLocale::Taiwan;
            break;
        }
        if ( big5_utf8 == "?")
        {
            language = QLocale::China;
            break;
        }
    }
    return language;
}

QString TextConverter::getConfigPath(const QString& config)
{
    QString configPath;

    // try personal directory first
    PersonalSettings settings;
    configPath = settings.getStorageDirectory() + "/" + config;
    if (QFile::exists(configPath))
        return configPath;

    // try program working directory
    configPath = QCoreApplication::applicationDirPath() + "/" + config;
    if (QFile::exists(configPath))
        return configPath;

    // try pre-defined default directory
    configPath = QString("%1/%2").arg(OPENCC_OCD_DIR).arg(config);
    if (QFile::exists(configPath))
        return configPath;

    // return relative path
    return config;
}
