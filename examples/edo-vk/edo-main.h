/////////////////////////////////////////////////////////////////////////////
// EDO-VK 1.0gamma
//
// Beginn: 2000-03-23
//
// Beschreibung:  Neue Menuefuehrung und bessere Bedienung als Hauptziel.
//                Weitere Funktionen wie Sortierung, Listendarstellung,
//                Tresen-Bildschirm mit Anwesenheitserfassung, Geburts-
//                tagsreminder und Passwortschutz.
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>    // Bildschirmausgaben und Tastenabfrage
#include <conio.h>    // Bildschirm- und Tastensteuerung per BIOS-Fkt.
#include <string.h>   // Zeichenfolgen bearbeiten mit strcmp(), strcpy()
                      // und fuer 'strsep()' ( NOT ANSI??!! )
#include <io.h>       // Arbeit mit Dateien, _open(), _close()
#include <fcntl.h>    // Enthaelt Option-Flags fuer _open(), _close()
#include <unistd.h>   // Dateien Schreiben/Lesen, read(), write()
#include <ctype.h>    // Typ- und Zeichenumwandlungen - toupper()
#include <errno.h>    // error-strings ( ANSI C++ ), used by strerror()
     
extern int errno;     // variable for error-description-string - strerror()


/////////////////////////////////////////////////////////////////////////////
// FELDER und DEKLARATIONEN
//

// *** Rueckgabewerte von Funktionen ***
typedef enum
{
	eOK,					      // Vorgang erfolgreich
	eWRONG_PARAMS,			// falsche Parameter
	eFILE_NOT_OPENED,		// konnte Datei nicht oeffnen
	eFILE_NOT_CLOSED,		// konnte Datei nicht schliessen
	eFILE_READ_ERROR,		// konnte nicht aus Datei lesen
	eFILE_WRITE_ERROR,	// konnte nicht in Datei schreiben
  eNOT_OK             // Wert nicht in Ordnung
} ReturnValues;

// *** TASTEN ***
typedef enum
{
  Key_Enter = 13,
  Key_Escape = 27,
  Key_Insert = 82,
  Key_Delete = 83,
  Key_F1 = 59,
  Key_F2 = 60,
  Key_F3 = 61,
  Key_F4 = 62,
  Key_F5 = 63,
  Key_F6 = 64,
  Key_F7 = 65,
  Key_F8 = 66,
  Key_F9 = 67,
  Key_F10 = 68,
  Key_Up = 72,
  Key_Down = 80,
  Key_Left = 75,
  Key_Right = 77,
  Key_PageUp = 73,
  Key_PageDown = 81
} Keys;


// *** Globale Werte ***
typedef enum {
  eMAX_MEMBERS = 700,      // Max. Anzahl von Mitgliedern
  eMAX_FUNC_KEYS = 13,     // Max. Anzahl von Funktions-Tasten
  eSELECTED = 100,         // Status: ausgewaehlt
  eUNSELECTED = 110,       // Status: nicht gewaehlt
  eLIST_ENTRYNUM = 13      // num of entries visible in list
} GlobalValues;

// *** vorzeichenlose Ganzzahlen ***
typedef unsigned long ULONG;
typedef unsigned int UINT;

// *** tCOLORS: Farbwerte ***
typedef struct  {
  int text,textBk,    highlTxt,highlBk;
  int frameTxt,frameBk,  cursorTxt,cursorBk;
  int titleTxt,titleBk,  statusTxt,statusBk;
  int dialogTxt,dialogBk,dialogFrame,dialogTitle;
  int buttonTxt,buttonBk,selButtonTxt,selButtonBk;
	int shaddow;
} tCOLORS;

// *** tBUTTON: Button-Eigenschaften ***
typedef struct  {
  int x,y;
  char *label;
  int state;
} tBUTTON;

// *** tFUNC_KEYS: Funktionstasten ***
typedef struct  {
  char *name;
  char *descript;
} tFUNC_KEYS;

//
// *** Maximal-Laengen fuer Daten ***
//
typedef enum LengthValues
{
  eLEN_NAME = 23,                     // "Nachname"
  eLEN_FIRSTNAME = 33,                // "Vorname"
  eLEN_MEMBERID = 7,                  // "Mitglieds-Nummer"
  eLEN_GRADE = 7,                     // "Grad"
  eLEN_DATE = 13,                     // "Datum"
  eLEN_STREET = 41,                   // "Strasse"
  eLEN_PLACE = 41,                    // "Ort"
  eLEN_POSTALCODE = 9,                // "PostLeitZahl"
  eLEN_BANKNAME = 41,                 // "Bank-Name"
  eLEN_BANKID = 21,                   // "BankLeitZahl"
  eLEN_ACCOUNTNUM = 21,               // "Konto-Nr."
  eLEN_PHONE = 31,                    // "Telefon"
  eLEN_CALMMEMBERSHIP = 21,           // "Ruhige Mitgliedschaft"
  eLEN_FILENAME = 254,                // max length of filename, incl. path
	eMAX_ENTRIES = 6,                   // max entries of dataset
	eMAX_ENTRY_LEN = 100,               // max chars for one entry
  eMAX_DATASETS = 7000,               // max num of datasets to work with
  eMAX_STRBUF_CHARS = 254             // max chars for string-buffer..
                                      // ..line-orientated file-operations
} ;

// *** tMENU: Eigenschaften von Menue ***
typedef struct  {
  char *title;
  tCOLORS clrs;
  tFUNC_KEYS funcKey[eMAX_FUNC_KEYS];
} tMENU;

// *** tFILE: Datei-Eigenschaften ***
typedef struct  {
  int Hnd;
  FILE *Stream;
  char Name[eLEN_FILENAME];
} tFILE;

// *** DataSet with entry-strings ***
typedef struct {
  char *EntryStr[eMAX_ENTRIES];
	int Num;
 	int primaryEntry;
} tDATASET;             // used to import datasets

//
// *** tMEMBER: Mitgliedsdaten-Stamm ***
//
typedef struct  {
  char MemberID[eLEN_MEMBERID];     // Mitglieds-Nummer
  char Name[eLEN_NAME];             // Name
  char Firstname[eLEN_FIRSTNAME];   // Vorname
  char Street[eLEN_STREET];         // Strasse
  char Place[eLEN_PLACE];           // Wohnort
  char PostalCode[eLEN_POSTALCODE]; // PLZ
  char Grade[eLEN_GRADE];           // Kyu,Grad
  char BankName[eLEN_BANKNAME];     // Bankname
  char BankID[eLEN_BANKID];         // Bankleitzahl
  char AccountNum[eLEN_ACCOUNTNUM]; // Bankleitzahl, Konto-Nummer
  char Birthday[eLEN_DATE];         // Geburts-Datum
  char EntryDate[eLEN_DATE];        // Eintritts-Datum
  char LastExam[eLEN_DATE];         // Datum der letzten Pruefung
  char Phone[eLEN_PHONE];           // Telefon
  char CalmMembership[eLEN_CALMMEMBERSHIP];  // ruhende Mitgliedschaft
} tMEMBER;

tMEMBER sMember[eMAX_MEMBERS];

//
// Ende: Felder und Deklarationen
/////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// Import Class for CSV-Files
//
class cImportCSV
{
public:
  cImportCSV();                             // set default values
	~cImportCSV();                            // close file, if still open
  int openFile( tFILE *pCSV_File );         // open file
	int	closeFile( tFILE *pCSV_File );        // close file
  int strConvertWinASCII( char *pConvStr[eMAX_STRBUF_CHARS] );
                                            // convert Windows- to DOS-ASCII
	int fcheckLine( tFILE *pCSV_File, char tempBuf[eMAX_STRBUF_CHARS] );
                                            // check line
	int fgetDataSet( tFILE *pCSV_File, tDATASET *pImportData );
                                            // get entries of dataset
	virtual int importDataSets( int &rRealDataSets);  // get all datasets

	tFILE CSV_File;                           // file-properties
	tDATASET ImportData[eMAX_DATASETS]; // imported datasets

  int numDataSets;                          // num of datasets
	int numEntries;                           // num of entries in dataset

private:
	char seperatCh;                           // char to seperate entries
} ;
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// PROTOTYPEN
//
int openFile(tFILE *pFile, int optFlags);
int closeFile(tFILE *pFile);
int cwrite(char *string);
int cswrite(char *string, int fieldLen);
int showMsgBox(int x, int y, int width, int height, tCOLORS clrs);
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// EIN- und AUSGABE mit DATEIEN
//

///////////////////////////////////////////////////////////////////////////////
//	Datei oeffnen:	openFile(DateiHandle, Datei-Attribute);
//
//	Parameter:  tFILE *pFile		  - uebergibt das FileHandle	  (out)
//					    int optFlags		  - Option-Flags fuer _open()	  (in)
//
//	Returns:	  eOK               = OK
//					    eWRONG_PARAMS     = falsche/fehlende Parameter
//					    eFILE_NOT_OPENED  =	Datei konnte nicht geoeffnet werden
//
int openFile(tFILE *pFile, int optFlags)
{
	if (pFile->Name[0] <= ' ')	 {  // Dateiname pruefen
		cprintf("\r\nFEHLER: openFile() - kein Dateiname angegeben!\r\n");
		exit(eFILE_NOT_OPENED);	// Abbruch
	}
	if (optFlags < 0)	{	      // OptionFlags pruefen
		cprintf("\r\nFEHLER: openFile() - keine OptionFlags angegeben!\r\n");
		exit(eWRONG_PARAMS);		// Abbruch
	}

  // Datei oeffnen
  if ( (pFile->Hnd = _open(pFile->Name, optFlags)) == -1 ) {
    if ( (pFile->Hnd = _creat(pFile->Name, optFlags)) == -1 )  {
      cprintf("\r\nFEHLER: Konnte Datei nicht neu anlegen!\r\n");
      exit(eFILE_NOT_OPENED);
    }
    else {
      cprintf("\r\nDatei %s geoeffnet!\n\r", pFile->Name);
      return eOK;
    }
		cprintf("\r\nFEHLER: Konnte Datei %s nicht oeffnen!\r\n", pFile->Name);
    exit(eFILE_NOT_OPENED);
	}
	return eOK;
}
// Ende: openFile(..)


///////////////////////////////////////////////////////////////////////////////
//	Datei schliessen:	  closeFile(DateiHandle);
//
//	Parameter:		tFILE *pFile	   - uebergibt FileHandle	(out)
//
//	Returns:		  eOK              = OK
//					      eFILE_NOT_CLOSED = Datei konnte nicht geschlossen werden
//
int closeFile(tFILE *pFile)
{
	if ( _close(pFile->Hnd) != 0 ) 	{
		cprintf("\r\nFEHLER: Konnte Datei %s nicht schliessenn!\r\n",pFile->Name);
		exit(eFILE_NOT_CLOSED);		// Abbruch
	}
	return eOK;
}
// Ende: closeFile(..)

//
// Ende: Ein-/Ausgabe mit Dateien
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// EIN- und AUSGABE auf BILDSCHIRM
//

///////////////////////////////////////////////////////////////////////////////
//	String schreiben: 	cwrite(*string);
//
//  Parameter:          string        - diese Zeichenfolge schreiben
//
//  Returnwerte:        eOK           - Alles i.O.
//                      eWRONG_PARAMS - falsche Parameter
//
//  Laufzeitfehler:     Exceptions, wenn uebergebener String nicht initial.
//                      oder aber Aufruf im Format 'cswrite("xy"..)' erfolgt.
//
int cwrite(char *string)
{
  int stringLen = strlen(string);

  // ** Paramater pruefen **
  if (stringLen < 1 || stringLen > 80 || string == NULL) {
    cprintf("\n\rcwrite: Laenge der Zeichenfolge ungueltig!");
    cprintf("\n\r        string=%s  strlen=%d", string,stringLen);
    exit(eWRONG_PARAMS);
  }
  // ** String schreiben **
  cprintf("%s",string);

  return eOK;
}
// Ende: cwrite(..);


///////////////////////////////////////////////////////////////////////////////
//	String in Feld schreiben: 	cswrite(*string, fieldLen);
//
//  Parameter:          string        - diese Zeichenfolge ins Feld schreiben
//                      fieldLen      - Laenge des Ausgabe-Feldes
//
//  Returnwerte:        eOK           - Alles i.O.
//                      eWRONG_PARAMS - falsche Parameter
//
//  Laufzeitfehler:     Exceptions, wenn uebergebener String nicht initial.
//                      oder aber Aufruf im Format 'cwrite("xy")' erfolgt.
//
int cswrite(char *string, int fieldLen)
{
  int stringLen = strlen(string);
	char newString[fieldLen+1];

  // ** Paramater pruefen **
  if (fieldLen==0)  fieldLen=stringLen;
  if (fieldLen<=0 || fieldLen > 80 || string == NULL ) {
    cprintf("\n\rcswrite: Laenge der Zeichenfolge ungueltig!");
    cprintf("\n\r         string=%s  len=%d", string,fieldLen);
    exit(eWRONG_PARAMS);
  }

  // ** String in Feld mit Laenge 'fieldLen' **
  if (stringLen+1 > fieldLen)  {
    newString[0]='\0';
    strncat(newString, string, fieldLen);
    cprintf("%s",newString);
	}
  else {
    cprintf("%s",string);
    if (fieldLen != stringLen) {  // wenn String-Laenge < als fieldLen
      for (int count=stringLen+1; count<=fieldLen; count++)
        cprintf(" ");
    }
	}
  return eOK;
}
// Ende: cswrite(..)


///////////////////////////////////////////////////////////////////////////////
//	MessageBox zeichnen: 	showMsgBox(x,y, width,height, clrs);
//
//	Parameter:  x, y		          - Koordinaten von linker oberen Ecke
//					    width, height		  - Breite, Hoehe in Zeichen
//              clrs              - Feld vom Typ tCOLORS mit Farbwerten
//
//	Returns:	  eOK               = OK
//					    eWRONG_PARAMS     = falsche/fehlende Parameter
//
int showMsgBox(int x, int y, int width, int height, tCOLORS clrs)
{
  // ** Parameter pruefen **
  if (x < 1 || x > 80 || y < 1 || y > 25)  {
    cprintf("\n\rshowMsgBox: Wrong coordinates!");
    return eWRONG_PARAMS;
  }
  if (width < 3 || width > 80 || height < 4 || height > 23)	{
    cprintf("\n\rshowMsgBox: Wrong width or height!");
    cprintf("\n\r            x,y=%d,%d w,h=%d,%d",x,y,width,height);
    return eWRONG_PARAMS;
  }

  // ** Message-Box zeichnen **
  int x2  = x + width - 1;
  int y2  = y + height - 1;
  int row = 1,  col = 1;

  textbackground(clrs.dialogBk); textcolor(clrs.dialogFrame);
  gotoxy(x, y);               /* 1.Zeile des Rahmens */
    cputs("É");  col = 2;
    while (col < width)  {
      cputs("Í"); col++;
    }
    cputs("»");

  row = 1;
  while (row < height)  {     /* vertikale Linen */
    gotoxy(x, y+row); cputs("º");
    gotoxy(x2,y+row); cputs("º");
      textcolor(0); cputs("Û"); textcolor(clrs.dialogFrame);  // Schatten
    row++;
  }

  gotoxy(x, y2);              /* letzte Zeile des Rahmens */
    cputs("È");  col = 2;
    while (col < width)  {
      cputs("Í"); col++;
    }
    cputs("¼");
      textcolor(0); cputs("Û"); textcolor(clrs.dialogFrame);  // Schatten

  textcolor(0);
  gotoxy(x+2, y2+1);          /* Schatten unter letzter Zeile */
    col = 1;
    while (col < width)  {
      cputs("Û"); col++;
    }
  textbackground(clrs.dialogBk); textcolor(clrs.dialogTxt);
  window(x+1,y+1, x2-1,y2-1); /* Anzeigebereich auf x,y bis x2,y2 verkleinern */
  clrscr();                   /* den Bereich mit Box-Farben versehen */
  window( 1, 1, 80,25);       /* Anzeige wieder zuruecksetzen */

  textbackground(clrs.textBk); textcolor(clrs.text);

  return eOK; // Alles OK
}
// Ende: showMsgBox(..)

//
///////////////////////////////////////////////////////////////////////////////

