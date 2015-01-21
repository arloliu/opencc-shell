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

#include "TextLoader.hpp"
#include "CharsetDetector.hpp"
#include <QFile>
#include <QTextCodec>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QMimeDatabase>
#include <QMimeType>
#endif

TextLoader::TextLoader(const QString& fileName, const QString& charset):
    mValid(false), mContent(), mCharset()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QMimeDatabase db;
    QString type = db.mimeTypeForFile(fileName).name();
    if (type.contains("executable") || type.contains("octet-stream") || type.contains("compressed"))
        return;
    if (!type.contains("text") && !type.contains("html") && !type.contains("xml"))
        return;
#endif

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QByteArray buffer = file.readAll();
    file.close();

    if (charset.toUpper() == "AUTO")
    {
        CharsetDetector detector;
        mCharset = detector.detect(buffer);
    }
    else
    {
        mCharset = charset;
    }

    QTextCodec* codec = QTextCodec::codecForName(mCharset.toUtf8());
    mCharset = codec->name();
    mContent = codec->toUnicode(buffer);
    mValid = true;
}

TextLoader::TextLoader(const QByteArray& text, const QString& charset):
    mValid(false), mContent(), mCharset()
{
    if (charset.toUpper() == "AUTO")
    {
        CharsetDetector detector;
        mCharset = detector.detect(text);
    }
    else
    {
        mCharset = charset;
    }

    QTextCodec* codec = QTextCodec::codecForName(mCharset.toUtf8());
    mCharset = codec->name();
    mContent = codec->toUnicode(text);
    mValid = true;
}

TextLoader::~TextLoader()
{
}

QString TextLoader::content()
{
    return mContent;
}

QString TextLoader::charset()
{
    return mCharset;
}

bool TextLoader::valid()
{
    return mValid;
}
