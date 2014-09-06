#ifndef SETUP_DIALOG_H
#define SETUP_DIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();

    void SetPort(QString Port);
    void SetBaudRate(int BaudRate);
    void SetDataBits(int DataBits);
    void SetParity(int Parity);
    void SetStopBits(int StopBits);
    void SetFlowControl(int FlowControl);
    void SetAvailablePorts(QStringList AvailablePorts);

    QString GetPort(void);
    int GetBaudRate(void);
    int GetDataBits(void);
    int GetParity(void);
    int GetStopBits(void);
    int GetFlowControl(void);

private:
    Ui::SetupDialog *ui;

    QString Port;
    int BaudRate;
    int DataBits;
    int Parity;
    int StopBits;
    int FlowControl;
};

#endif // SETUP_DIALOG_H
