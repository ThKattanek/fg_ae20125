#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void CreateLanguageMenu(QString defaultLocale);
    void RetranslateUi();

    QString appPath;
    QString langPath;
    QTranslator appTranslator;
    QTranslator qtTranslator;

private slots:
    void slotLanguageChanged(QAction* action);
};

#endif // MAINWINDOW_H
