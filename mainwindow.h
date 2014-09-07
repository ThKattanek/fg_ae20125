#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSettings>
#include <QTranslator>
#include <QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

#include "ascel_protokoll.h"
#include "setup_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Sleep(int ms);
    void SendCmd(char code, int data);

    void SetFrequenzDisplay(int frquenz);
    void SetWaveForm(int waveform);
    void SetModus(int mode);
    void SetPLLEnable(int data);

private:
    Ui::MainWindow *ui;

    void CreateLanguageMenu(QString defaultLocale);
    void RetranslateUi();

    QString appPath;
    QString langPath;
    QTranslator appTranslator;
    QTranslator qtTranslator;


    SetupDialog *setup_dialog;

    // Schnittstellen Einstellungen //
    QString Port;
    int BaudRate;
    int DataBits;
    int Parity;
    int StopBits;
    int FlowControl;

    // Allgemeine Einstellungen //
    char LFKennung;
    bool EnableUartLine;

    // Konfiguration //
    QSettings *config;

    // Seielle Port Klasse //
    QSerialPort *serial;
    QSerialPortInfo *serial_info;
    QStringList AvailablePorts;
    bool isConnected;

    // ASCEL Protokoll Klasse
    AscelProtokoll *protokoll;

    QIcon *led_on, *led_off;
    QIcon *wave0_on, *wave0_off, *wave1_on, *wave1_off, *wave2_on, *wave2_off;

    bool keep_alive;
    int old_mode;

private slots:
    void slotLanguageChanged(QAction* action);
    void on_action_Beeden_triggered();
    void on_actionEinstellungen_triggered();
    void serial_error(QSerialPort::SerialPortError);
    void on_actionVerbinden_triggered();
    void on_actionTrennen_triggered();
    void serial_incomming_data();
    void OnFrequencyChanged(int);
    void on_Wave0_clicked();
    void on_Wave1_clicked();
    void on_Wave2_clicked();
    void on_Mode0_clicked();
    void on_Mode1_clicked();
    void on_Mode2_clicked();
    void on_PLLEnable_clicked(bool checked);
    void on_PLLFactor_valueChanged(double arg1);
    void on_PLLOffset_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
