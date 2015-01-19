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

#ifndef _TEXT_LOADER_HPP
#define _TEXT_LOADER_HPP
#include <QString>
#include <QList>

class TextLoader
{
public:
    explicit TextLoader(const QString& fileName, const QString& charset = "auto");
    ~TextLoader();
    QString convert(const QString& config, const QString& text);
    QString content();
    QString charset();

private:
    QString mContent;
    QString mCharset;

    static QStringList mTwCharsetList;
    static QStringList mCnCharsetList;
};
#endif // _TEXT_LOADER_HPP
