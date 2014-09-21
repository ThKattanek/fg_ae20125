#include "license_dialog.h"
#include "ui_license_dialog.h"

LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);

    // Lizenz Datei aus Resource laden //

    QFile myFile(":/txt/license.txt");
    myFile.open(QIODevice::ReadOnly);
    QTextStream textStream(&myFile);
    QString line = textStream.readAll();
    myFile.close();

    ui->LicenseOut->setPlainText(line);
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}
