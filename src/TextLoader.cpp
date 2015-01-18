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
#include <QTextStream>
#include <QTextCodec>

TextLoader::TextLoader(const QString& fileName):
    mContent(), mCharset("UTF-8")
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QByteArray buffer = file.readAll();
    file.close();

    CharsetDetector detector;
    mCharset = detector.detect(buffer);


    QTextStream stream(buffer);
    QTextCodec* codec = QTextCodec::codecForName(mCharset.toUtf8());
    stream.setCodec(codec);
    mCharset = codec->name();
    mContent = stream.readAll();
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
