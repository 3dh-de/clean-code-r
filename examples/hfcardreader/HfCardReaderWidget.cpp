#include "HfCardReaderWidget.h"
#include "HfCardReader.h"

#include <QMessageBox>
#include <QVariant>

/**
 * Default constructor inits card reader device and starts timer
 */
HfCardReaderWidget::HfCardReaderWidget(QWidget *parent) : 
	QLabel(parent),
	m_Device(0),
	m_pCardReader(0),
	m_LastCardId(0)
{ 
	setProperty("EnableBeep", false);
	setProperty("ScriptURL", "http://localhost/nclub.php?cardid=");
	setProperty("ScriptLoader", "explorer");

	setFont( QFont("Arial", 14) );
	setAlignment( Qt::AlignCenter );
	setFrameStyle( QFrame::StyledPanel|QFrame::Sunken );
	setPalette( QPalette( Qt::white ) );
	setAutoFillBackground( true );

	setText( tr("NO DEVICE") );

	if (initCardReader())
		this->startTimer(500);
}


/**
 * Init the card reader on virtual com port "COM1" (via USB connection)
 */
bool HfCardReaderWidget::initCardReader(void) {
	if (::LoadLibrary( L"EasyD12.dll" ) == NULL)
		QMessageBox::warning(0, tr("HfCardReader"), tr("Cannot load EasyD12.dll!"));

	// create object for api wrapper
	m_pCardReader = new HfCardReader;

	if (!m_pCardReader)
		return false;

	if (!m_pCardReader->Init()) {
		QMessageBox::warning(0, tr("HfCardReader"), tr("Cannot load CVAPIV01.dll!"));
		return false;
	}

	char port[256] = "COM1";
	char *tmp1 = new char[1024];

	int retries(3);
	
	while(retries--) {
		if (m_pCardReader->OpenComm(port, 115200) != 0x00) {
			Sleep(500);
			continue;
		}

		// read user infos
		memset(tmp1, 1024, 0);
		m_pCardReader->GetUserInfo(m_Device, tmp1);
		if ( QString(tmp1).toLower().contains("nclub") )
			break;

		m_pCardReader->CloseComm();
		Sleep(500);
	}

	if ( !QString(tmp1).toLower().contains("nclub") ) {
		QMessageBox::warning(0, "HfCardReader", tr("The card reader cannot be initialized.\n\nPlease restart the program or reconnect the device!"));
		return false;
	}

	setText( QString("Device found on COM1\n\nuser info: %1").arg(tmp1));

	// enable blue led
	m_pCardReader->SetLED(m_Device, 0x01);

	// both leds blink for 400ms 3 times
	// m_pCardReader->ActiveLED(m_Device, 0x03, 4, 3);

	// m_pCardReader->ActiveBuzzer(m_Device, 0x04, soundPattern);

	// set up beep on correct read
	// m_pCardReader->SetWiegandStatus(m_Device, 0x10);

	delete [] tmp1;

	return true;
}


/**
 * Look for cards and display the found card number
 *
 * If multiple cards are found, the result is 0.
 */
void HfCardReaderWidget::timerEvent(QTimerEvent *) {
	if (!m_pCardReader)
		return;

	// beep x100ms, pause x100ms, beep x100ms, pause x100ms, x times
	unsigned char soundPattern[5] = { 1, 1, 1, 0, 1 };

	// colors
	QPalette default;
	default.setBrush( QPalette::Base, Qt::white);
	default.setBrush( QPalette::Window, Qt::white);
	QPalette ok;
	ok.setBrush( QPalette::Base, Qt::green);
	ok.setBrush( QPalette::Window, Qt::green);
	QPalette error;
	error.setBrush( QPalette::Base, Qt::red);
	error.setBrush( QPalette::Window, Qt::red);

	setPalette(default);
	setText( m_LastCardId ? tr("Scanning...\n\n\nLast card found: %1").arg(m_LastCardId) : tr("Scanning...") );

	unsigned char atq[2] = { 0, 0 };
	unsigned char coll;
	unsigned char uid[4] = { 0, 0, 0, 0 };

	if (m_pCardReader->MF_Request(m_Device, 0x00, atq) == 0x00 && atq[0]) {
		while( m_pCardReader->MF_Anticoll(m_Device, uid, coll) != 0x00 && coll != 0x00) {
			Sleep(100);
		}
		unsigned long serial;
		serial = unsigned long(uid[0]) << 24 | unsigned long(uid[1]) << 16 | unsigned long(uid[2]) << 8 | unsigned long(uid[3]);
		if (serial) {
			if (serial == m_LastCardId)
				return; // skip

			m_LastCardId = serial;
			setPalette( ok );
			setText( QString("Found card %1!").arg(m_LastCardId));
			update();
			m_pCardReader->ActiveLED(m_Device, 0x02, 5, 2);
			if (property("EnableBeep").toBool())
				m_pCardReader->ActiveBuzzer(m_Device, 0x04, soundPattern);

			QString cmd = QString("%1%2").arg(property("ScriptURL").toString()).arg( m_LastCardId );
			m_RunningProcs.append(new QProcess(this));
			m_RunningProcs.last()->start(property("ScriptLoader").toString(), QStringList() << cmd);
		} else {
			setPalette( error );
		}
	}
}
