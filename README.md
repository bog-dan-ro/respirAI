## respirAI dependencies
* qt 5.11 or newer
* qt quick controls 2
* qt serial port
* qt charts
* qt virtual keyboard

On Raspbian you need to install the following packages:

```bash
$ sudo apt install qt5-default qtdeclarative5-dev qtquickcontrols2-5-dev libqt5serialport5-dev \
                     qml-module-qtcharts libqt5charts5-dev qml-module-qtquick-controls2 \
                     qml-module-qtquick-virtualkeyboard qtvirtualkeyboard-plugin \
                     qml-module-qt-labs-folderlistmodel qml-module-qt-labs-settings \
                     qml-module-qtquick2 qml-module-qtquick-window2 qml-module-qtquick-layouts
```

## How to build it with DUMMY_DATA
```bash
$ qmake CONFIG+=release CONFIG+=DUMMY_DATA
$ make -j6
$ ./respirAI
```
