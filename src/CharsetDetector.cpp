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

#include "CharsetDetector.hpp"

CharsetDetector::CharsetDetector(): mHandle(0)
{
    mHandle = uchardet_new();
}

CharsetDetector::~CharsetDetector()
{
    if (mHandle)
        uchardet_delete(mHandle);
}

QString CharsetDetector::detect(QByteArray& data)
{
    uchardet_reset(mHandle);
    uchardet_handle_data(mHandle, data.data(), data.length());
    uchardet_data_end(mHandle);
    QString charset = QString(uchardet_get_charset(mHandle));

    if (charset.isEmpty())
        charset = "UTF-8";

    return charset;
}
