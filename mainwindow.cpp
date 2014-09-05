#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Configurationsdatei öffnen
    config = new QSettings(QDir::homePath() + "/.fg_ae20125.conf",QSettings::IniFormat,this);

    config->beginGroup("MainWindow");
    QString SystemLocale = config->value("language","nope").toString();
    config->endGroup();

    ui->setupUi(this);

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
}

MainWindow::~MainWindow()
{   
    delete ui;

    // Configurationsdatei schließen
    if(config != NULL) delete config;
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
