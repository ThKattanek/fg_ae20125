#ifndef INFO_DIALOG_H
#define INFO_DIALOG_H

#include <QDialog>
#include "version.h"

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget *parent = 0);
    ~InfoDialog();

private:
    Ui::InfoDialog *ui;
};

#endif // INFO_DIALOG_H
