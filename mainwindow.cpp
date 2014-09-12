#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    setup_dialog(NULL),protokoll(NULL)
{
    // Configurationsdatei öffnen
    config = new QSettings(QDir::homePath() + "/.fg_ae20125.conf",QSettings::IniFormat,this);

    config->beginGroup("MainWindow");
    QString SystemLocale = config->value("language","nope").toString();
    config->endGroup();

    ui->setupUi(this);

    connect(ui->fr_0,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_1,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_2,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_3,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_4,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_5,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_6,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_7,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));
    connect(ui->fr_8,SIGNAL(valueChanged(int)),this,SLOT(OnFrequencyChanged(int)));

    // Alle Gruppen deaktivieren //
    ui->FrequenzGroup->setEnabled(false);
    ui->WaveGroup->setEnabled(false);
    ui->ModusGroup->setEnabled(false);
    ui->PLLGroup->setEnabled(false);
    ui->PresetGroup->setEnabled(false);
    ui->SweepGroup->setEnabled(false);
    ui->ModGroup->setEnabled(false);
    ui->InfoGroup->setEnabled(false);

    // PLL Einstellungen deaktivieren //
    SetPLLEnable(0);

    /// Icons Laden ///

    led_on = new QIcon(":/icons/icons/green-on-16.png");
    led_off = new QIcon(":/icons/icons/green-off-16.png");
    wave0_on = new QIcon(":/icons/icons/wave0.png");
    wave0_off = new QIcon(":/icons/icons/wave0_grey.png");
    wave1_on = new QIcon(":/icons/icons/wave1.png");
    wave1_off = new QIcon(":/icons/icons/wave1_grey.png");
    wave2_on = new QIcon(":/icons/icons/wave2.png");
    wave2_off = new QIcon(":/icons/icons/wave2_grey.png");

    SetWaveForm(3); // Alle ausschalten
    SetModus(3);    // Alle ausschalten

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

    protokoll = new AscelProtokoll();
}

MainWindow::~MainWindow()
{   
    delete ui;

    // Configurationsdatei schließen
    if(config != NULL) delete config;

    if(protokoll != NULL) delete protokoll;
}

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void MainWindow::Sleep(int ms)
{
    if(ms <= 0) return;
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

void MainWindow::SendCmd(char code, int data)
{
    serial->write(protokoll->GetSendCommandString(code, data).toAscii());
    serial->waitForBytesWritten(-1);
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
        qDebug() << tr("Fehler") << serial->errorString();
    }
}

void MainWindow::serial_incomming_data()
{
    #define PUFFER_SIZE 1024
    static char puffer[PUFFER_SIZE];
    static unsigned short puffer_pos;

    QByteArray data = serial->readAll();

    while (serial->waitForReadyRead(10))
        data += serial->readAll();

    for(int i=0;i<data.length();i++)
    {
        if(puffer_pos == PUFFER_SIZE) puffer_pos = 0;

        puffer[puffer_pos++] = data.at(i);
        if(puffer[puffer_pos-1] == ';')
        {
            // String ist fertig //
            puffer[puffer_pos-1] = 0;

            if(protokoll->CheckCommand(QString(puffer)))
            {
                //ui->label->setText("Alles OK!");
                switch(protokoll->command)
                {
                case code_Frequency:
                    SetFrequenzDisplay(protokoll->data);
                    break;

                case code_Waveform:
                    SetWaveForm(protokoll->data);
                    break;

                case code_Mode:
                    old_mode = protokoll->data;
                    SetModus(protokoll->data);
                    break;

                case code_PLLReferenceEnable:
                    ui->PLLEnable->setChecked(protokoll->data);
                    SetPLLEnable(protokoll->data);
                    break;

                case code_PLLFactor:
                    ui->PLLFactor->setValue((float)protokoll->data / 10.0f);
                    break;

                case code_PLLOffsett:
                    ui->PLLOffset->setValue(protokoll->data);
                    break;

                case code_SweepStartFrequency:
                    ui->SweepStartFreq->setValue((float)protokoll->data / 10.0f);
                    break;

                case code_SweepStopFrequency:
                    ui->SweepStopFreq->setValue((float)protokoll->data / 10.0f);
                    break;

                case code_SweepFrequency:
                    ui->SweepFreq->setValue((float)protokoll->data / 10.0f);
                    break;

                case code_SweepMode:
                    switch(protokoll->data)
                    {
                    case 0: // LOOP
                        ui->SweepLoop->setChecked(true);
                        break;
                    case 1: // SWING
                        ui->SweepSwing->setChecked(true);
                        break;
                    default:;
                    }
                    break;

                case code_HardwareRevision:
                    ui->HardwareVersion->setText(QVariant(protokoll->data).toString());
                    break;

                case code_FirmwareRevision:
                    ui->SoftwareVersion->setText(QVariant(protokoll->data).toString());
                    break;

                case code_ProduktID:
                    ui->ProduktID->setText(QVariant(protokoll->data).toString());
                    break;

                case code_KeepAlive:
                    keep_alive = true;
                    break;
                }
            }
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

            old_mode = 0;

            ui->actionVerbinden->setEnabled(false);
            ui->actionTrennen->setEnabled(true);
            ui->FrequenzGroup->setEnabled(true);
            ui->WaveGroup->setEnabled(true);
            ui->ModusGroup->setEnabled(true);
            ui->PLLGroup->setEnabled(true);
            ui->PresetGroup->setEnabled(true);
            ui->SweepGroup->setEnabled(true);
            ui->ModGroup->setEnabled(true);
            ui->InfoGroup->setEnabled(true);

            Sleep(1000);
            SendCmd(code_GetSettings, 0);
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
    ui->FrequenzGroup->setEnabled(false);
    ui->WaveGroup->setEnabled(false);
    ui->ModusGroup->setEnabled(false);
    ui->PLLGroup->setEnabled(false);
    ui->PresetGroup->setEnabled(false);
    ui->SweepGroup->setEnabled(false);
    ui->ModGroup->setEnabled(false);
    ui->InfoGroup->setEnabled(false);

    isConnected = false;
}

void MainWindow::SetFrequenzDisplay(int frequenz)
{
    if(frequenz > 100000000) return;

    ui->fr_8->setValue(frequenz / 100000000);
    frequenz = frequenz % 100000000;

    ui->fr_7->setValue(frequenz / 10000000);
    frequenz = frequenz % 10000000;

    ui->fr_6->setValue(frequenz / 1000000);
    frequenz = frequenz % 1000000;

    ui->fr_5->setValue(frequenz / 100000);
    frequenz = frequenz % 100000;

    ui->fr_4->setValue(frequenz / 10000);
    frequenz = frequenz % 10000;

    ui->fr_3->setValue(frequenz / 1000);
    frequenz = frequenz % 1000;

    ui->fr_2->setValue(frequenz / 100);
    frequenz = frequenz % 100;

    ui->fr_1->setValue(frequenz / 10);
    frequenz = frequenz % 10;

    ui->fr_0->setValue(frequenz / 1);
    frequenz = frequenz % 1;
}

void MainWindow::SetWaveForm(int waveform)
{
    ui->Wave0->setIcon(*wave0_off);
    ui->Wave1->setIcon(*wave1_off);
    ui->Wave2->setIcon(*wave2_off);

    switch(waveform)
    {
    case 0:
        ui->Wave0->setIcon(*wave0_on);

        break;
    case 1:
        ui->Wave1->setIcon(*wave1_on);
        break;
    case 2:
        ui->Wave2->setIcon(*wave2_on);
        break;
    default:
        break;
    }
}

void MainWindow::SetModus(int mode)
{
    ui->Mode0->setIcon(*led_off);
    ui->Mode1->setIcon(*led_off);
    ui->Mode2->setIcon(*led_off);

    switch(mode)
    {
    case 0:
        ui->Mode0->setIcon(*led_on);
        ui->SweepGroup->setEnabled(true);
        ui->FrequenzGroup->setEnabled(true);
        ui->WaveGroup->setEnabled(true);
        ui->PLLGroup->setEnabled(true);
        break;
    case 1:
        ui->Mode1->setIcon(*led_on);
        ui->SweepGroup->setEnabled(false);
        ui->FrequenzGroup->setEnabled(false);
        ui->WaveGroup->setEnabled(false);
        ui->PLLGroup->setEnabled(false);
        break;
    case 2:
        ui->Mode2->setIcon(*led_on);
        break;
    default:
        break;
    }
}

void MainWindow::OnFrequencyChanged(int)
{
    int frequenz =  ui->fr_0->value() + \
                    ui->fr_1->value()*10 + \
                    ui->fr_2->value()*100 + \
                    ui->fr_3->value()*1000 + \
                    ui->fr_4->value()*10000 + \
                    ui->fr_5->value()*100000 + \
                    ui->fr_6->value()*1000000 + \
                    ui->fr_7->value()*10000000 + \
                    ui->fr_8->value()*100000000;

    SendCmd(code_Frequency, frequenz);
}

void MainWindow::on_Wave0_clicked()
{
    SetWaveForm(0);
    SendCmd(code_Waveform, 0);
}

void MainWindow::on_Wave1_clicked()
{
    SetWaveForm(1);
    SendCmd(code_Waveform, 1);
}

void MainWindow::on_Wave2_clicked()
{
    SetWaveForm(2);
    SendCmd(code_Waveform, 2);
}

// Modus Normal setzen //
void MainWindow::on_Mode0_clicked()
{
    SetModus(0);
    SendCmd(code_ReturnFromSweep, 0);
    SendCmd(code_Mode, 0);
    old_mode = 0;

    ui->SweepGroup->setEnabled(true);
}

// Modus Sweep setzen //
void MainWindow::on_Mode1_clicked()
{
    SetModus(1);
    if(old_mode == 2) SendCmd(code_ReturnFromSweep, 0);
    SendCmd(code_Mode, 1);
    old_mode = 1;

    ui->SweepGroup->setEnabled(false);
    ui->FrequenzGroup->setEnabled(false);
    ui->WaveGroup->setEnabled(false);
    ui->PLLGroup->setEnabled(false);
}

// Modus Modulation setzen //
void MainWindow::on_Mode2_clicked()
{
    SetModus(2);
    if(old_mode == 1) SendCmd(code_ReturnFromSweep, 0);
    SendCmd(code_Mode, 2);
    old_mode = 2;
}

void MainWindow::SetPLLEnable(int data)
{
    if(data == 0)
    {
        // is Disabled
        ui->PLLFactor->setEnabled(false);
        ui->PLLOffset->setEnabled(false);
        ui->PLLLabel0->setEnabled(false);
        ui->PLLLabel1->setEnabled(false);
        ui->PLLLabel2->setEnabled(false);
    }
    else
    {
        // is Enabled
        ui->PLLFactor->setEnabled(true);
        ui->PLLOffset->setEnabled(true);
        ui->PLLLabel0->setEnabled(true);
        ui->PLLLabel1->setEnabled(true);
        ui->PLLLabel2->setEnabled(true);
    }
}

void MainWindow::on_PLLEnable_clicked(bool checked)
{
    SetPLLEnable(checked);
    SendCmd(code_PLLReferenceEnable,checked);
}

void MainWindow::on_PLLFactor_valueChanged(double arg1)
{
    SendCmd(code_PLLFactor,arg1 * 10);
}

void MainWindow::on_PLLOffset_valueChanged(int arg1)
{
    SendCmd(code_PLLOffsett,arg1);
}

void MainWindow::on_SweepStartFreq_valueChanged(double arg1)
{
    SendCmd(code_SweepStartFrequency,arg1 * 10);
}

void MainWindow::on_SweepStopFreq_valueChanged(double arg1)
{
    SendCmd(code_SweepStopFrequency,arg1 * 10);
}

void MainWindow::on_SweepFreq_valueChanged(double arg1)
{
    SendCmd(code_SweepFrequency,arg1 * 10);
}

void MainWindow::on_SweepLoop_clicked()
{
    SendCmd(code_SweepMode,0);
}

void MainWindow::on_SweepSwing_clicked()
{
    SendCmd(code_SweepMode,1);
}
