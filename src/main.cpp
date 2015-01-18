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

#include "MainWindow.hpp"
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QStyle>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QString defaultLocaleFile = ":/translations/" + QLocale::system().name() + ".qm";
    QTranslator translator;

    translator.load(defaultLocaleFile);
    app.installTranslator(&translator);

    MainWindow window;
    window.setMinimumSize(800, 600);
    window.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            window.size(),
            qApp->desktop()->availableGeometry()
        )
    );

    window.show();
    return app.exec();
}

