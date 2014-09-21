#include "info_dialog.h"
#include "ui_info_dialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);

    ui->version_txt->setText("Version: " + QString(VERSION_STRING));
}

InfoDialog::~InfoDialog()
{
    delete ui;
}
