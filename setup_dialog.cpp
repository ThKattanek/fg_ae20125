#include "setup_dialog.h"
#include "ui_setup_dialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);

    ui->Parity->addItems(QStringList()<<"None"<<"Even"<<"Odd"<<"Mark"<<"Space");
    ui->FlowControl->addItems(QStringList()<<"None"<<"RTS/CTS"<<"XON/XOFF");
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::SetPort(QString Port)
{
    this->Port = Port;
    ui->SeriellPort->setEditText(Port);
}

void SetupDialog::SetBaudRate(int BaudRate)
{
    this->BaudRate = BaudRate;
    ui->BaudRate->setValue(BaudRate);
}

void SetupDialog::SetDataBits(int DataBits)
{
    this->DataBits = DataBits;
    ui->DataBits->setValue(DataBits);
}

void SetupDialog::SetParity(int Parity)
{
    this->Parity = Parity;
    ui->Parity->setCurrentIndex(Parity);
}

void SetupDialog::SetStopBits(int StopBits)
{
    this->StopBits = StopBits;
    ui->StopBits->setValue(StopBits);
}

void SetupDialog::SetFlowControl(int FlowConrol)
{
    this->FlowControl = FlowControl;
    ui->FlowControl->setCurrentIndex(FlowConrol);
}

void SetupDialog::SetAvailablePorts(QStringList AvailablePorts)
{
    ui->SeriellPort->addItems(AvailablePorts);
}

QString SetupDialog::GetPort()
{
    return ui->SeriellPort->currentText();
}

int SetupDialog::GetBaudRate()
{
    return ui->BaudRate->value();
}

int SetupDialog::GetDataBits()
{
    return ui->DataBits->value();
}

int SetupDialog::GetParity()
{
    return ui->Parity->currentIndex();
}

int SetupDialog::GetStopBits()
{
    return ui->StopBits->value();
}

int SetupDialog::GetFlowControl()
{
    return ui->FlowControl->currentIndex();
}
