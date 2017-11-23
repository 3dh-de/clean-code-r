#ifndef HFCARDREADERWIDGET_H
#define HFCARDREADERWIDGET_H

#include <QLabel>
#include <QList>
#include <QProcess>

/// forward for api wrapper class
class HfCardReader;


/**
 * Simple widget for hf reader
 *
 * Every 100ms the reader checks for a new card and display the found card number
 */
class HfCardReaderWidget : public QLabel
{
	Q_OBJECT
public:
	HfCardReaderWidget(QWidget *parent = 0); 
	virtual ~HfCardReaderWidget() { }

protected:
	bool			initCardReader(void);
	void			timerEvent(QTimerEvent*);

private:
	unsigned char	m_Device;		///< handle for reader, is -1 if not connected to m_Device
	HfCardReader   *m_pCardReader;	///< wrapper for api dll
	unsigned long   m_LastCardId;
	QList<QProcess*> m_RunningProcs;
};


#endif