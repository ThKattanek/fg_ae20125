#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    setup_dialog(NULL)
{
    // Configurationsdatei öffnen
    config = new QSettings(QDir::homePath() + "/.fg_ae20125.conf",QSettings::IniFormat,this);

    config->beginGroup("MainWindow");
    QString SystemLocale = config->value("language","nope").toString();
    config->endGroup();

    ui->setupUi(this);

    ui->tabWidget->setEnabled(false);

    /// CurrentPath holen und abspeichern ///
    appPath = QDir::currentPath();

    /// Translator installieren ///
    langPath = appPath+"/lang";

    qApp->installTranslator(&qtTranslator);
    qApp->installTranslator(&appTranslator);

    if(SystemLocale == "nope")
    {
        QString SystemLocale = QLocale::system().name();       // "de_DE"
        SystemLocale.truncate(SystemLocale.lastIndexOf('_'));  // "de"

        config->beginGroup("MainWindow");
        config->setValue("language",SystemLocale);
        config->endGroup();
    }

    CreateLanguageMenu(SystemLocale);

    config->beginGroup("Schnittstelle");
    Port = config->value("Port","").toString();
    BaudRate = config->value("BaudRate",9600).toInt();
    DataBits = config->value("DataBits",QSerialPort::Data8).toInt();
    Parity = config->value("Parity",QSerialPort::NoParity).toInt();
    StopBits = config->value("StopBits",QSerialPort::OneStop).toInt();
    FlowControl = config->value("FlowControl",QSerialPort::NoFlowControl).toInt();
    config->endGroup();

    // Serielle Port Klasse erstellen //
    serial = new QSerialPort(this);
    serial_info = new QSerialPortInfo();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,SLOT(serial_error(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(serial_incomming_data()));

    isConnected = false;
    ui->actionTrennen->setEnabled(false);

    // Im System vorhandene Schnittstellen ermitteln und die Namen speichern //
    for(int i=0; i<serial_info->availablePorts().length(); i++)
    {
        AvailablePorts.append(serial_info->availablePorts()[i].portName());
    }
}

MainWindow::~MainWindow()
{   
    delete ui;

    // Configurationsdatei schließen
    if(config != NULL) delete config;
}

void MainWindow::CreateLanguageMenu(QString defaultLocale)
{
   QActionGroup* langGroup = new QActionGroup(ui->menu_Sprache);
   langGroup->setExclusive(true);

   connect(langGroup, SIGNAL(triggered(QAction *)),this, SLOT(slotLanguageChanged(QAction *)));

    QDir dir(langPath);
    QStringList fileNames;

    fileNames = dir.entryList(QStringList("lang_*.qm"));

   for (int i = 0; i < fileNames.size(); ++i)
   {
      // get locale extracted by filename
      QString locale;
      locale = fileNames[i];                      // "lang_de.qm"
      locale.truncate(locale.lastIndexOf('.'));   // "lang_de"
      locale.remove(0, locale.indexOf('_') + 1);  // "de"

      QString lang = QLocale::languageToString(QLocale(locale).language());

      if(lang == "German") lang = "Deutsch";

      QAction *action = new QAction(lang, this);
      action->setCheckable(true);
      action->setData(locale);

      QString iconfile = fileNames[i];
      iconfile.truncate(iconfile.lastIndexOf('.'));
      iconfile = dir.filePath(iconfile + ".png");
      action->setIcon(QIcon(iconfile));
      action->setIconVisibleInMenu(true);
      action->setStatusTip(tr("Wechseln zur Sprache:") + " " + lang);

      ui->menu_Sprache->addAction(action);
      langGroup->addAction(action);

      // set default translators and language checked
      if (defaultLocale == locale)
      {
         appTranslator.load("lang_" + action->data().toString(), langPath);
         qtTranslator.load("qt_" + action->data().toString(), langPath);
         action->setChecked(true);
         ui->retranslateUi(this);
      }
    }
}

void MainWindow::RetranslateUi()
{
    ui->retranslateUi(this);

    // Und weitere Unterfenster hier eintragen ...
}

void MainWindow::slotLanguageChanged(QAction* action)
{
    appTranslator.load("lang_" + action->data().toString(), langPath);
    qtTranslator.load("qt_" + action->data().toString(), langPath);
    config->beginGroup("MainWindow");
    config->setValue("language",action->data().toString());
    config->endGroup();

    ui->menu_Sprache->clear();
    CreateLanguageMenu(action->data().toString());
    RetranslateUi();
}

void MainWindow::on_action_Beeden_triggered()
{
    close();
}

void MainWindow::on_actionEinstellungen_triggered()
{
    // Setup Fenster Öffnen //
    setup_dialog = new SetupDialog(this);
    if(setup_dialog == NULL)
    {
        //qDebug() << "Fehler: SetupDialog kann nicht erstellt werden.";
        return;
    }

    // Alle Verfügbaren Ports ans Setup schicken
    setup_dialog->SetAvailablePorts(AvailablePorts);

    // Werte Setzen //
    setup_dialog->SetPort(Port);
    setup_dialog->SetBaudRate(BaudRate);
    setup_dialog->SetDataBits(DataBits);
    setup_dialog->SetParity(Parity);
    setup_dialog->SetStopBits(StopBits);
    setup_dialog->SetFlowControl(FlowControl);

    // Dialog Modal starten //
    if(QDialog::Accepted == setup_dialog->exec())
    {
        // Werte aus Dialog holen und in INI speichern //
        Port = setup_dialog->GetPort();
        BaudRate = setup_dialog->GetBaudRate();
        DataBits = setup_dialog->GetDataBits();
        Parity = setup_dialog->GetParity();
        StopBits = setup_dialog->GetStopBits();
        FlowControl = setup_dialog->GetFlowControl();

        // In Cofigdatei speichern //
        config->beginGroup("Schnittstelle");
        config->setValue("Port",Port);
        config->setValue("BaudRate",BaudRate);
        config->setValue("DataBits",DataBits);
        config->setValue("Parity",Parity);
        config->setValue("StopBits",StopBits);
        config->setValue("FlowControl",FlowControl);
        config->endGroup();
    }
}

void MainWindow::serial_error(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        on_actionTrennen_triggered();
        QMessageBox::critical(this, tr("Fehler"), serial->errorString());
    }
}

void MainWindow::serial_incomming_data()
{
    #define PUFFER_SIZE 1024
    static char puffer[PUFFER_SIZE];
    static unsigned short puffer_pos;

    QByteArray data = serial->readAll();

    for(int i=0;i<data.length();i++)
    {
        if(puffer_pos == PUFFER_SIZE) puffer_pos = 0;

        puffer[puffer_pos++] = data.at(i);
        if(puffer[puffer_pos-1] == ';')
        {
            // String ist fertig //
            puffer[puffer_pos-1] = 0;

            ui->label->setText(QString(puffer));

            puffer_pos = 0;
        }
    }
}

void MainWindow::on_actionVerbinden_triggered()
{
    if(isConnected) return;

    serial->setPortName(Port);

    if(serial->open(QIODevice::ReadWrite))
    {
        if(serial->setBaudRate(BaudRate)
                && serial->setDataBits((QSerialPort::DataBits)DataBits)
                && serial->setParity((QSerialPort::Parity)Parity)
                && serial->setStopBits((QSerialPort::StopBits)StopBits)
                && serial->setFlowControl((QSerialPort::FlowControl)FlowControl)
                )
        {
            isConnected = true;
            ui->actionVerbinden->setEnabled(false);
            ui->actionTrennen->setEnabled(true);
            ui->tabWidget->setEnabled(true);
        }
        else
        {
            serial->close();
            isConnected = false;
            QMessageBox::critical(this,"Fehler...","Fehler beim setzen der Schnittstellen Parameter.\nBitte ueberpruefen Sie ihre Einstellungen.");
        }
    }
    else
    {
        serial->close();
        isConnected = false;
        QMessageBox::critical(this,"Fehler...","Die Serielle Schnittstelle konnte nicht geoeffnet werden\nBitte ueberpruefen Sie ihre Einstellungen.");
    }
}

void MainWindow::on_actionTrennen_triggered()
{
    if(!isConnected) return;

    serial->close();
    ui->actionVerbinden->setEnabled(true);
    ui->actionTrennen->setEnabled(false);
    ui->tabWidget->setEnabled(false);

    isConnected = false;
}
