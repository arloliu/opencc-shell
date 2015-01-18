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

#ifndef _CONVERT_SETTING_WIDGET_HPP
#define _CONVERT_SETTING_WIDGET_HPP
#include <QString>
#include <QWidget>

class QComboBox;

class ConvertSettingWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ConvertSettingWidget(QWidget* parent = 0);
    ~ConvertSettingWidget();
    QString getMethod();
    QString getOption();
    QString getCharset();
    void setMethod(const QString& method);
private:
    void updateOptions();

    QComboBox* mConvertMethod;
    QComboBox* mConvertOption;
    QComboBox* mConvertCharset;

signals:
    void settingChanged();

private slots:
    void methodChanged(int index);
    void optionChanged(int index);
};
#endif // _CONVERT_SETTING_WIDGET_HPP
