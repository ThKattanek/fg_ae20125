# Steuerungssoftware für ASCEL Funktionsgenerator AE20125 #


### Einleitung ###

Diese Software erlaubt es den Funktionsgenerator von ASCEL AE20125 auch unter Linux zu benutzen. Dank des offenen
Datenprotokolls von ASCEL konnte ich diese Anwendung programmieren. Für Verbesserungen oder Änderungen sendet einfach ein Ticket !

### Version: 0.1 Beta ###

### Screenshot ###


### Voraussetzung ###

* installiertes git
* installiertes qt-sdk
* QtSerialPort (<url>https://qt-project.org/wiki/QtSerialPort</url>)

### Bauen der Anwendung unter Linux ###

#### Quellcode Clonen: ####
git clone https://bitbucket.org/tkattanek/fg_ae20125.git
#### Anwendung bauen: ####
* qmake
* make
* make install (mit Root Rechte)

Danach sollte die Anwendung im Menü oder Desktop zu sehen sein. Getestet unter Debian 7.4 (x32 und x64)

#### Die Anwendung installiert sich in folgende Verzeichnisse: ####

* /usr/bin
* /usr/share/fg_ae20125
* /usr/share/applications
* /usr/share/pixmaps

### Bauen der Anwendung unter Windows (MinGW32) ###

* qmake
* make
* make release
* make release-install

#### !! Anstatt make könnte auch mingw32-make benutzt werden. ####
