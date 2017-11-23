#include "HfCardReaderWidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QWindowsStyle>
#include <QSettings>

/**
 * Init gui and cardreader
 */
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	a.setStyle( new QWindowsStyle );

	QCoreApplication::setOrganizationName("Nclub");
    QCoreApplication::setOrganizationDomain("Nclub.de");
	QCoreApplication::setApplicationName("HfCardReader");

	QMainWindow *pMain = new QMainWindow;
	
	QSettings settings("HfCardReader.ini", QSettings::IniFormat);

	HfCardReaderWidget *pLabel = new HfCardReaderWidget(pMain);
	pLabel->setMinimumSize(400, 300);
	pLabel->setProperty("EnableBeep", settings.value("HfCardReader/EnableBeep", false));
	pLabel->setProperty("ScriptURL", settings.value("HfCardReader/ScriptURL", "http://localhost/nclub.php?cardid="));
	pLabel->setProperty("ScriptLoader", settings.value("HfCardReader/ScriptLoader", "explorer"));
	
	pMain->setCentralWidget(pLabel);
	pMain->show();

	settings.setValue("HfCardReader/EnableBeep", pLabel->property("EnableBeep").toBool());
	settings.setValue("HfCardReader/ScriptURL", pLabel->property("ScriptURL").toString());
	settings.setValue("HfCardReader/ScriptLoader", pLabel->property("ScriptLoader").toString());
	settings.sync();

	return a.exec();
}
