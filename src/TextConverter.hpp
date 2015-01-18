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

#ifndef _TEXT_CONVERTER_HPP
#define _TEXT_CONVERTER_HPP
#include <QString>
#include <QList>
#include <QByteArray>
#include <QLocale>

// opencc header files
#include "Common.hpp"

class TextConverter
{
public:
    TextConverter(const QString& configName, const QString& configDir = QString());
    ~TextConverter();
    QString convertToUtf8(QString& text);
    QString getErrorMessage() { return mErrorMessage; }

    static QByteArray convertFromUtf8(const QString& charset, QString& text);
    static QLocale::Country getLocaleCountry(const QString& data, const QString& charset);
    static QStringList getCharsetList(QLocale::Country langType);

private:
    static QLocale::Country getUtf8LocaleCountry(const QString& data);
    static QString getConfigPath(const QString& config);

    static QStringList mTwCharsetList;
    static QStringList mCnCharsetList;
    static QTextCodec* mUtf8Codec;
    static QTextCodec* mBig5Codec;
    static QTextCodec* mGbkCodec;

    bool mInitialized;
    QString mConfigPath;
    QString mConfigDir;
    QString mErrorMessage;

    opencc::ConverterPtr mConverter;
};
#endif
