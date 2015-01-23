# OpenCC Shell 開放中文轉換介面

OpenCC Shell is a GUI application based on [OpenCC](https://github.com/BYVoid/OpenCC) project, provides single file and multiple files batch conversion between traditional and simplified chinese.

OpenCC Shell是一個基於[OpenCC](https://github.com/BYVoid/OpenCC) 程式庫的圖型化介面，提供單一以及多檔批次中文簡繁轉換功能。

###  Build Requirements 編譯需求
* cmake (>=2.8.9)
* Qt4 (>=4.8) or Qt5 (>= 5.0) (*recommended*)

Ubuntu packages (QT5):
```
sudo apt-get install cmake qtbase5-dev qttools5-dev qttools5-dev-tools
```
Ubuntu packages (QT4):
```
sudo apt-get install cmake libqt4-dev
```

### Build with CMake 使用CMake編譯
Linux (Qt4 version)
```
cmake -DWITH_QT5=OFF -DCMAKE_BUILD_TYPE=Release
```

Linux (Qt5 version)
```
cmake -DWITH_QT5=ON -DCMAKE_BUILD_TYPE=Release
```

Windows MSYS/Qt Creator (Qt4 version)
```
cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="" -DCMAKE_BUILD_TYPE=Release
```

Windows MSYS/Qt Creator (Qt5 version)
```
cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="" -DCMAKE_BUILD_TYPE=Release
```

## Downloads 下載
[OpenCC Shell Win32 Installer] (https://github.com/arloliu/opencc-shell/releases)

## Links 相關連結
[OpenCC Project Site] (https://github.com/BYVoid/OpenCC)

## License  許可協議
Apache License 2.0

