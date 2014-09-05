#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSettings>
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
    void CleanLanguageMenu();
    void RetranslateUi();

    QString appPath;
    QString langPath;
    QSettings *config;
    QTranslator appTranslator;
    QTranslator qtTranslator;


private slots:
    void slotLanguageChanged(QAction* action);
    void on_action_Beeden_triggered();
};

#endif // MAINWINDOW_H
