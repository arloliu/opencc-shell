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

#ifndef _PERSONAL_SETTINGS_HPP
#define _PERSONAL_SETTINGS_HPP
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QFileInfo>
#include <QTextCodec>

class PersonalSettings
{
    public:
        explicit PersonalSettings():
            mSettings(QSettings::IniFormat, QSettings::UserScope, "dev-null", "ocs")
        {
            mSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
        }

        ~PersonalSettings()
        {
            mSettings.sync();
        }

        QString getStorageDirectory()
        {
            return QFileInfo(mSettings.fileName()).canonicalPath();
        }

        void setValue(const QString& key, const QVariant& value)
        {
            mSettings.setValue(key, value);
        }

        QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const
        {
            return mSettings.value(key, defaultValue);
        }

    private:
        QSettings mSettings;
};
#endif // _PERSONAL_SETTINGS_HPP
