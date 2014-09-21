#ifndef LICENSE_DIALOG_H
#define LICENSE_DIALOG_H

#include <QDialog>
#include <QTextStream>

namespace Ui {
class LicenseDialog;
}

class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenseDialog(QWidget *parent = 0);
    ~LicenseDialog();

private:
    Ui::LicenseDialog *ui;
};

#endif // LICENSE_DIALOG_H
