/////////////////////////////////////////////////////////////////////////////
// EDO-VK 1.01gamma
//
// Beginn: 2000-07-11
//
// Beschreibung:  (x) Neue Menuefuehrung und bessere Bedienung,
//                ( ) Sortierung,
//                (x) Listendarstellung,
//                ( ) Tresen-Bildschirm mit
//                ( ) Anwesenheitserfassung und mit
//                ( ) Geburtstagsreminder und
//                ( ) Passwortschutz,
//                ( ) Suchfunktion mit Liste der Ergebnisse,
//                ( ) Druckfunktion,
//                ( ) Online-Hilfe,
//                ( ) intelligente Tastaturabfrage
//                ( ) IMPORT von Excel-Tabellen
//
/////////////////////////////////////////////////////////////////////////////

#include "edo-main.h" // enthaelt Deklarationen von Typen und Feldern



/////////////////////////////////////////////////////////////////////////////
// PROTOTYPEN
//
int showButton(tBUTTON button, tCOLORS clrs, int backGr);
int showHelp(tCOLORS clrs);
int showForm(tMEMBER *pMember, char *formTitle, tCOLORS clrs);
int editForm(tMEMBER *pMember, char *formTitle, tCOLORS clrs);
int showList(char *listTitle, tCOLORS clrs, int numOfMembers);
int execList(char *listTitle, tCOLORS clrs, int *numOfMembers);

int copyData(tMEMBER *destinMember, tMEMBER *sourceMember);
int deleteData(tMEMBER *delMember);

tCOLORS msgBoxclrs;
//
// Ende: Prototypen
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// fields & vars
//
typedef tMEMBER MemberDataset[eMAX_MEMBERS];    // experimental!!!
typedef MemberDataset MemberDB;                 // experimental!!!
//
/////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  constructor
//
// Function:    set the default values of object-vars
//
cImportCSV::cImportCSV()
{
  seperatCh = ';';
	numDataSets = 0;
	numEntries = 5;  // +++ muss automatisiert werden: autom. Erkennung
	                 // +++ der Entries pro Zeile anhand Anzahl der ';' !!!
}
// End: cImportCSV()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  destructor
//
// Function:    if ImportFile is still open, close the file
//
cImportCSV::~cImportCSV()
{
  if ( CSV_File.Stream != NULL ) {
		closeFile( &CSV_File );
  }
}
// End: ~cImportCSV()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  open ImportFile
//
int cImportCSV::openFile( tFILE *pCSV_File )
{
	// open CSV-File as readonly in textmode
	pCSV_File->Stream = fopen( pCSV_File->Name, "rt");
	if ( pCSV_File->Stream == NULL ) {
		fprintf( stderr, "\n\r ERROR: File %s can't be opened!\n\r       %s",
             pCSV_File->Name, strerror( errno ) );
		return eNOT_OK;
  }

  return eOK;
}
// End: openFile()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  close ImportFile
//
int cImportCSV::closeFile( tFILE *pCSV_File )
{
	// close file(-stream)
	if ( fclose( pCSV_File->Stream ) == EOF ) {
		fprintf( stderr, "\n\r ERROR: File can't be closed!\n\r       %s",
						 strerror( errno) );
		return eNOT_OK;
  }

  return eOK;
}
// End: closeFile()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  string convert Windows-ASCII to DOS
//
int strConvertWinASCII( char *pConvStr[eMAX_STRBUF_CHARS] )
{

  return eOK;
}
// End: strConvertWinASCII()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  check line
//
// Function:    checks num of entries in one line,
//              if (entries != numEntries) the line is skipped
//
int cImportCSV::fcheckLine( tFILE *pCSV_File, char tempBuf[eMAX_STRBUF_CHARS] )
{
	int count = 0, entriesFound = 0;    // counters for compare

	// find and count seperatCh ( = entries )
  while ( ( count < eMAX_STRBUF_CHARS ) && ( tempBuf[count] != '\0' ) ) {
		if ( tempBuf[count] == seperatCh ) {
			entriesFound++;
    }
		count++;
  }

	// check, if num of entries found is ok
  if ( entriesFound != numEntries ) {
		fprintf( stderr, "\n\rERROR: fcheckLine(): Wrong num of entries" );
		fprintf( stderr, " each line.\n\r				%s", strerror( errno) );
		return eNOT_OK;
  }


  return eOK;
}
// End: fcheckLine()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  get DataSet
//
int cImportCSV::fgetDataSet( tFILE *pCSV_File, tDATASET *pImportData )
{
  // save line out of file, including different entries
	char dataSetBuf[eMAX_STRBUF_CHARS];
	int result = eNOT_OK;

	dataSetBuf[0] = '\0';

	while ( result != eOK ) {
		if ( fgets( dataSetBuf, eMAX_STRBUF_CHARS, pCSV_File->Stream ) == NULL ) {
      fprintf( stderr, "\n\rERROR: fgets() failed or EOF reached!" );
			fprintf( stderr, "\n\r       %s", strerror( errno) );
		  return eNOT_OK;
    }
    result = fcheckLine( pCSV_File, dataSetBuf );
  }

  char *pBuf = dataSetBuf;    // pointer on buf
  char **ppBuf = &pBuf;				// pointer on buf-pointer
  char *tempEntryStr = "";    // string for entry
  int count = 0;							// entry-counter

	// get entry-strings
  while ( tempEntryStr = strsep( ppBuf, ";" ) ) {
 	  if ( count >= eMAX_ENTRIES ) {
      break;
    }
	  pImportData->EntryStr[count] = tempEntryStr;
		count++;
  }

  return eOK;
}
// End: fgetDataSet()


//////////////////////////////////////////////////////////////////////////////
// cImportCSV:  get all DataSets out of file
//
int cImportCSV::importDataSets( int &rRealDataSets )
{
	int count = 0, realNum = 0;

	numDataSets = 0;    // zuruecksetzen

  fgetDataSet( &CSV_File, &ImportData[0] ); // get description of 1. line

	for ( count=0; count <= ( eMAX_MEMBERS-rRealDataSets ); count++ ) {
	  if ( fgetDataSet( &CSV_File, &ImportData[count] ) != eOK ) {
			break;
    }
    realNum = count+rRealDataSets;
    // set "Firstname"
    strcpy( sMember[realNum].Firstname, ImportData[count].EntryStr[0] );
    // set "Name"
    strcpy( sMember[realNum].Name, ImportData[count].EntryStr[1] );
    // set "Street"
    strcpy( sMember[realNum].Street, ImportData[count].EntryStr[2] );
    // set "PostalCode"
    strncpy( sMember[realNum].PostalCode, ImportData[count].EntryStr[3], 6 );
    // set "Place"
    strcpy( sMember[realNum].Place, ImportData[count].EntryStr[3] );
    // set "Birthday"
    strcpy( sMember[realNum].Birthday, ImportData[count].EntryStr[4] );
    // import "CalmMembership", if imported entry not empty
    strcpy( sMember[realNum].CalmMembership, ImportData[count].EntryStr[5] );

    numDataSets++;    // count datasets
  }

  rRealDataSets += numDataSets;

  return eOK;
}
// End: importDataSets()
//
//////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Datenbank aktualisieren: refreshData(*DataBank, *NumOfDatasets)
//
int refreshData(int *NumOfDatasets)
{
  int moveCount = 0, oldNumOfDatasets = *NumOfDatasets;
  *NumOfDatasets = 0;

  for (int count=0; count < eMAX_MEMBERS; count++) {
     if ( (strlen(sMember[count].MemberID) > 0)  &&
          (sMember[count].MemberID[0] != '@') ) {
       moveCount++;
     }
  }
  *NumOfDatasets = moveCount;

  textcolor(0); textbackground(7);
  gotoxy(66,1); clreol(); gotoxy(66,1); cprintf("EintrÑge: %04d", *NumOfDatasets);
};
// Ende: refreshData(..)



/////////////////////////////////////////////////////////////////////////////
// Daten loeschen: copyData(*delMember)
//
// !!! Anzahl der Datensaetze muss anschliessend verringert werden !!!
//
int deleteData(tMEMBER *delMember)
{
  // Mitgliedsdaten zuruecksetzen
  delMember->MemberID[0]='@';
  delMember->Name[0]='\0';
  delMember->Firstname[0]='\0';
  delMember->Street[0]='\0';
  delMember->Place[0]='\0';
  delMember->Phone[0]='\0';
  delMember->PostalCode[0]='\0';
  delMember->BankName[0]='\0';
  delMember->AccountNum[0]='\0';
  delMember->BankID[0]='\0';
  delMember->Grade[0]='\0';
  delMember->Birthday[0]='\0';
  delMember->EntryDate[0]='\0';
  delMember->LastExam[0]='\0';
  delMember->CalmMembership[0]='\0';

  // pruefen, ob Datensatz als "leer" markiert
  if (delMember->MemberID[0] != '@') {
    cprintf("\n\rdeleteData: Datensatz nicht korrekt geloescht!");
    cprintf("\n\r            MemberID: %s", delMember->MemberID);
    exit(eNOT_OK);
  }

  return eOK; // Alles OK
}
// Ende: deleteData(..)


/////////////////////////////////////////////////////////////////////////////
// Datensatz kopieren: copyData(*destinMember, sourceMember)
//
//  Daten per strncpy() von sourceMember[] nach destinMember[] kopieren.
//
int copyData(tMEMBER *destinMember, tMEMBER *sourceMember)
{
  // Daten in destinMember kopieren
  strncpy(destinMember->MemberID      ,sourceMember->MemberID, eLEN_MEMBERID);
  strncpy(destinMember->Name          ,sourceMember->Name, eLEN_NAME);
  strncpy(destinMember->Firstname     ,sourceMember->Firstname, eLEN_FIRSTNAME);
  strncpy(destinMember->Street        ,sourceMember->Street, eLEN_STREET);
  strncpy(destinMember->Place         ,sourceMember->Place, eLEN_PLACE);
  strncpy(destinMember->Phone         ,sourceMember->Phone, eLEN_PHONE);
  strncpy(destinMember->PostalCode    ,sourceMember->PostalCode, eLEN_POSTALCODE);
  strncpy(destinMember->BankName      ,sourceMember->BankName, eLEN_BANKNAME);
  strncpy(destinMember->AccountNum    ,sourceMember->AccountNum, eLEN_ACCOUNTNUM);
  strncpy(destinMember->BankID        ,sourceMember->BankID, eLEN_BANKID);
  strncpy(destinMember->Grade         ,sourceMember->Grade, eLEN_GRADE);
  strncpy(destinMember->Birthday      ,sourceMember->Birthday, eLEN_DATE);
  strncpy(destinMember->EntryDate     ,sourceMember->EntryDate, eLEN_DATE);
  strncpy(destinMember->LastExam      ,sourceMember->LastExam, eLEN_DATE);
  strncpy(destinMember->CalmMembership,sourceMember->CalmMembership, eLEN_CALMMEMBERSHIP);

  // pruefen, ob Kopie = Original
  if (strcmp(destinMember->MemberID, sourceMember->MemberID) != 0) {
    cprintf("\n\rcopyData: Datensatz nicht korrekt kopiert!");
    cprintf("\n\r          Kopiere Mitgl.Nr.: %s, Name: %s", sourceMember->MemberID, sourceMember->Name);
    cprintf("\n\r          zur Mitgl.Nr.: %s, Name: %s", destinMember->MemberID, destinMember->Name);
    exit(eNOT_OK);
  }

  return eOK; // Alles OK
}
// Ende: copyData(..)


/////////////////////////////////////////////////////////////////////////////
// Datensatz tauschen: swapData(*destinMember, sourceMember)
//
//  Inhalt der Datensaetze per copyData() tauschen.
//
int swapData(tMEMBER *destinMember, tMEMBER *sourceMember)
{
  // Zwischenspeicher fuer neue Daten
  tMEMBER tempMember;
  // Mitgliedsdaten zuruecksetzen
  deleteData(&tempMember);

  // Inhalt der Datensaetze tauschen
  copyData(&tempMember, destinMember);
  copyData(destinMember, sourceMember);
  copyData(sourceMember, &tempMember);

  return eOK; // Alles OK
}
// Ende: swapData(..)


/////////////////////////////////////////////////////////////////////////////
// Daten sortieren: sortData(int sortCategory, int *numOfMembers)
//
//  Parameter:    sortCategory:		0 = MemberID
//                                1 = Name
//                                2 = Firstname
//                                3 = Strasse
//                                4 = Place
//                                5 =	PostalCode
//                                6 = Phone
//                                7 =	Birthday
//                                8 = Grade
//                                9 = LastExam
//                               10 = EntryDate
//                               11 = BankName
//                               12 = BankID
//                               13 = AccountNum
//                               14 = CalmMembership
//
//  !!! Wenn leerer Datensatz gefunden, wird dieser geloescht !!!
//      --> wenn MemberID[0]=='@' dann ist Dateint sortData(int sortCategory, int *numO  refreshData(&sMember, numOfMembers);
int sortData(int sortCategory, int *numOfMembers)
{

//&&&&&&&&&&&
 refreshData(numOfMembers);

	int countX =0, countY =0;

  // Schleife sortiert Datensaetze von 'klein' nach 'gross'
  for (countX = 0; countX < *numOfMembers; countX++) {
    // wenn Datensatz nicht leer, Daten sortieren
		for (countY = countX+1; countY == *numOfMembers; countY++) {
      switch (sortCategory) {
        case 0:
          // nach MEMBERID sortieren
          if ( (strcmp(sMember[countX].MemberID, sMember[countY].MemberID) > 0)
					   || (sMember[countY].MemberID[0] == '@') ) {
            // wenn Vorgaenger < Nachfolger, tausche Datensaetze
		  			swapData(&sMember[countX], &sMember[countY]);
          }
          if ( strlen(sMember[countX].MemberID) > strlen(sMember[countY].MemberID) ) {
		  			swapData(&sMember[countX], &sMember[countY]);
          }
          break;
        case 1:
          // nach NAME sortieren
          if ( (strcmp(sMember[countY].Name, sMember[countX].Name) > 0)
             ||	(sMember[countX].MemberID[0]=='@') ) {
            // wenn Vorgaenger < Nachfolger, tausche Datensaetze
	    			swapData(&sMember[countX], &sMember[countY]);
          }
          if (strlen(sMember[countY].Name) > strlen(sMember[countX].Name) ) {
	    			swapData(&sMember[countX], &sMember[countY]);
          }
          break;
      } // switch(..)
    } // countY
  } // countX

//+++	refreshData(numOfMembers);

  return eOK; // Alles OK
}
// Ende: sortData(..)


/////////////////////////////////////////////////////////////////////////////
// EIN- und AUSGABE auf BILDSCHIRM
//

/////////////////////////////////////////////////////////////////////////////
//	Button zeigen: 	showButton(tBUTTON button, clrs, backGr);
//
//  Parameter:      button        - enthaelt x,y,label,state
//                  clrs          - Farbwerte
//                  backGr        - Hintergrundfarbe des aktiven Fensters
//
int showButton(tBUTTON button, tCOLORS clrs, int backGr)
{
  int stringLen = strlen(button.label);

	// ** Parameter pruefen **
	if (button.x < 1 || button.x > 77 || button.y < 0 || button.y > 22)  {
    cprintf("\n\rshowButton: x,y-Koordinaten ungueltig!");
    exit(eWRONG_PARAMS);
  }
	if ( stringLen <= 0 )  {
    cprintf("\n\rshowButton: keinen Button-Text angegeben!");
    exit(eWRONG_PARAMS);
  }

  // ** Button-zeichnen **
  if (button.state != eSELECTED)  {   // un-selektierter Button
    button.state = eUNSELECTED;
    textbackground(clrs.buttonBk); textcolor(clrs.buttonTxt);
	  gotoxy(button.x,button.y); cputs(" "); cwrite(button.label); cputs(" ");
  }
  else {                              // selektierter Button
  	textbackground(clrs.selButtonBk); textcolor(clrs.selButtonTxt);
	  gotoxy(button.x,button.y); cputs(""); cwrite(button.label); cputs("");
  }

  // ** Button-Schatten zeichnen **
	textbackground(backGr); textcolor(clrs.shaddow);
	gotoxy(button.x+stringLen+2,button.y); cputs("‹");
	gotoxy(button.x+1,button.y+1);
	for (int count=0; count <= stringLen+1; count++)
		 cputs("ﬂ");

  return eOK; // Alles OK
}
// Ende: showButton(..)

//
// Ende: Ein- und Ausgabe auf Bildschirm
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// MENUE-SYSTEM
//

/////////////////////////////////////////////////////////////////////////////
//	Hilfebildschirm zeigen: 	showHelp(clrs);
//
int showHelp(tCOLORS clrs)
{

  // ** Variablen fuer Dialogbox **
  int x=7, y=3;
  int width=64, height=20;

  // ** Dialogbox anzeigen **
  if (showMsgBox(x,y, width,height, clrs) != eOK)
    exit(eNOT_OK);

  textbackground(clrs.dialogBk); textcolor(clrs.dialogTitle);
  gotoxy(x+4, y); cputs(" Hilfe ");

  textbackground(clrs.highlBk); textcolor(clrs.highlTxt);
  gotoxy(x+3, y+2); cputs(" Bedienung ");

  textbackground(clrs.dialogBk); textcolor(clrs.dialogTxt);
  gotoxy(x+3, y+4); cputs("MenÅ:  Bewegen mit PFEILTASTEN, Datensatz îffnen oder ");
  gotoxy(x+3, y+5); cputs("       SchaltflÑche bestÑtigen mit der EINGABETASTE.  ");
  gotoxy(x+3, y+6); cputs("       In unterster Bildschirmzeile sind Funktions-   ");
  gotoxy(x+3, y+7); cputs("       Tasten aufgefÅhrt (F1 bis F10). EINFG = fÅgt   ");
  gotoxy(x+3, y+8); cputs("       neue DatensÑtze hinzu, ENTF = lîscht Datensatz.");
  gotoxy(x+3, y+9); cputs("       ESC-Taste = MenÅ verlassen, F1-Taste = Hilfe");
  gotoxy(x+3,y+10); cputs("       ");
  gotoxy(x+3,y+11); cputs("Daten Eingeben/Bearbeiten:");
  gotoxy(x+3,y+12); cputs("       In jedem Feld die Eingabe mit der EINGABETASTE ");
  gotoxy(x+3,y+13); cputs("       bestÑtigen -> nur so gelangt man in das nÑchste");
  gotoxy(x+3,y+14); cputs("       Feld. Mit OK werden Daten gespeichert, ABBRUCH");
  gotoxy(x+3,y+15); cputs("       speichert nicht und kehrt ins HauptmenÅ zurÅck.");

  // ** Button anlegen **
  tBUTTON menuButton = { x+3,y+height-3, "ZurÅck", eSELECTED };

  // ** Button anzeigen **
	showButton(menuButton, clrs, clrs.dialogBk);

  // ** Variable fuer Tasten-Steuerung
  char menuKey = ' ';         // Var fuer Tastenabfrage

  // ** Tastenabfrage, bis Esc gedrueckt ***
  while (menuKey != Key_Escape)  {
     // ++ Taste abfragen und pruefen ++
    menuKey = getch();
    switch( menuKey )  {
      case Key_Escape:  // ESC = Hilfebildschirm verlassen
        return eOK;
        break;
      case Key_Enter:
        return eOK;
        break;
    }
  }

  return eOK; // Alles OK
}
// Ende: showHelp(..)


/////////////////////////////////////////////////////////////////////////////
//	Formular zeichnen: 	showForm(pMember, *formTitle, clrs);
//
int showForm(tMEMBER *pMember, char *formTitle, tCOLORS clrs)
{
	// ** Parameter pruefen **
	if (strlen(formTitle)==0) {
		cprintf("\n\rshowForm: Kein Titelnamen angegeben!");
		exit(eWRONG_PARAMS);
  }

  // ** Feld-Bezeichner **
  char *formDescript[15];
  formDescript[ 0]="MitgliedsNr:   ";
  formDescript[ 1]="Name.......:   ";
  formDescript[ 2]="Vorname....:   ";
  formDescript[ 3]="Strasse....:   ";
  formDescript[ 4]="Ort........:   ";
  formDescript[ 5]="PLZ........:   ";
  formDescript[ 6]="Telefon....:   ";
  formDescript[ 7]="Geburtstag.:   ";
  formDescript[ 8]="Grad.......:   ";
  formDescript[ 9]="LetztePruef:   ";
  formDescript[10]="Eintritt...:   ";
  formDescript[11]="Bankname...:   ";
  formDescript[12]="BLZ........:   ";
  formDescript[13]="KontoNr....:   ";
  formDescript[14]="Ruh.Mitgl..:   ";

  // ** Variablen fuer Dialogbox **
  int x=7, y=3, count = 0;
  int width=61, height=20;

  // ** CURSOR anzeigen **
  _setcursortype(_SOLIDCURSOR);

  // ** Dialogbox anzeigen **
  if (showMsgBox(x,y, width,height, clrs) != eOK)
    exit(eNOT_OK);

  textbackground(clrs.dialogBk); textcolor(clrs.dialogTitle);
  gotoxy(x+4, y); cputs(" "); cwrite(formTitle); cputs(" ");

  // ** Feld-Bezeichner zeigen **
  textbackground(clrs.dialogBk); textcolor(clrs.dialogTxt);
  for (int row=0; row<15; row++)  {
    gotoxy(x+3, y+1+row); cwrite(formDescript[row]);
  }
  gotoxy(x+18+eLEN_DATE+2, y+8); cwrite("immer Format: 31.12.2000");

  // ** Felder zeigen **
  textbackground(clrs.highlBk); textcolor(clrs.highlTxt);
  if (pMember->MemberID[0]=='@')  {
    gotoxy(x+18, y+1); cswrite("leer", eLEN_MEMBERID-1);
  }
  else {
    gotoxy(x+18, y+1); cswrite(pMember->MemberID, eLEN_MEMBERID-1);
  }
  gotoxy(x+18, y+2); cswrite(pMember->Name, eLEN_NAME-1);
  gotoxy(x+18, y+3); cswrite(pMember->Firstname, eLEN_FIRSTNAME-1);
  gotoxy(x+18, y+4); cswrite(pMember->Street, eLEN_STREET-1);
  gotoxy(x+18, y+5); cswrite(pMember->Place, eLEN_PLACE-1);
  gotoxy(x+18, y+6); cswrite(pMember->PostalCode, eLEN_POSTALCODE-1);
  gotoxy(x+18, y+7); cswrite(pMember->Phone, eLEN_PHONE-1);
  gotoxy(x+18, y+8); cswrite(pMember->Birthday, eLEN_DATE-1);
  gotoxy(x+18, y+9); cswrite(pMember->Grade, eLEN_GRADE-1);
  gotoxy(x+18, y+10); cswrite(pMember->LastExam, eLEN_DATE-1);
  gotoxy(x+18, y+11); cswrite(pMember->EntryDate, eLEN_DATE-1);
  gotoxy(x+18, y+12); cswrite(pMember->BankName, eLEN_BANKNAME-1);
  gotoxy(x+18, y+13); cswrite(pMember->BankID, eLEN_BANKID-1);
  gotoxy(x+18, y+14); cswrite(pMember->AccountNum, eLEN_ACCOUNTNUM-1);
  gotoxy(x+18, y+15); cswrite(pMember->CalmMembership, eLEN_CALMMEMBERSHIP-1);

  // ** Buttons anlegen **
  tBUTTON menuButton[4] = {
    { (x+ 3),(y+height-3), "ZurÅck", eUNSELECTED },
    { (x+14),(y+height-3), "Bearbeiten", eUNSELECTED },
    { (x+29),(y+height-3), "Lîschen", eUNSELECTED },
    { (x+41),(y+height-3), "Drucken", eUNSELECTED}
  };
  int buttonNum = 4;
  int activeButton = 1;

  // ** Variable fuer Tasten-Steuerung
  char menuKey = ' ';         // Var fuer Tastenabfrage

  // ** Tastenabfrage, bis Esc gedrueckt ***
  while (menuKey != Key_Escape)  {

    // ** Buttons anzeigen **
    if (activeButton < 1)  activeButton = buttonNum;
    if (activeButton > buttonNum)  activeButton = 1;

    for (count=0; count <= buttonNum; count++)        // alle Buttons inaktiv
       menuButton[count-1].state = eUNSELECTED;

       menuButton[activeButton-1].state = eSELECTED;  // aktiven Btn. setzen

    for (count=0; count <= buttonNum; count++)        // alle Buttons zeigen
      showButton(menuButton[count-1], clrs, clrs.dialogBk);

    // ++ Taste abfragen und pruefen ++
    menuKey = getch();
    switch( menuKey )  {
      case Key_Escape:    // ESC = Hilfebildschirm verlassen
        _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
        return eOK;
        break;
      case Key_Enter:			// Enter = Button-Kommando ausfuehren
        switch (activeButton) {
          case 1:
            _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
            return eOK; break;
          case 2:
            if (editForm(pMember,"Bearbeiten", clrs) != eOK)
              exit(eNOT_OK);
            _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
            return eOK;
          case 3:
          case 4:
            break;
        }
        break;
      case Key_F1:				// F1 = Hilfe aufrufen
        _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
        if (showHelp(clrs) != eOK)
          exit(eNOT_OK);
        if (showForm(pMember, formTitle,clrs) != eOK)
          exit(eNOT_OK);
        return eOK;
        break;
      case Key_Left:			// einen Button nach links
        activeButton--;
        break;
      case Key_Right:			// einen Button nach rechts
        activeButton++;
        break;
    }
  } // Ende der Tastenabfrage

  _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
  return eOK; // Alles OK
}
// Ende: showForm(..)


/////////////////////////////////////////////////////////////////////////////
//	Daten in Formular eingeben: editForm(pMember,formTitle,clrs,numOfMembers)
//
int editForm(tMEMBER *pMember, char *formTitle, tCOLORS clrs)
{
	// ** Parameter pruefen **
	if (strlen(formTitle)==0) {
		cprintf("\n\reditForm: Kein Titelnamen angegeben!");
		exit(eWRONG_PARAMS);
  }

  // Zwischenspeicher fuer neue Daten
  tMEMBER tempMember;
  tMEMBER origMember;
  // Mitgliedsdaten zuruecksetzen
  deleteData(&tempMember);

  // Mitgliedsdaten sichern
  copyData(&origMember, pMember);

  // ** Feld-Bezeichner **
  char *formDescript[15];
  formDescript[ 0]="MitgliedsNr:   ";
  formDescript[ 1]="Name.......:   ";
  formDescript[ 2]="Vorname....:   ";
  formDescript[ 3]="Strasse....:   ";
  formDescript[ 4]="Ort........:   ";
  formDescript[ 5]="PLZ........:   ";
  formDescript[ 6]="Telefon....:   ";
  formDescript[ 7]="Geburtstag.:   ";
  formDescript[ 8]="Grad.......:   ";
  formDescript[ 9]="LetztePruef:   ";
  formDescript[10]="Eintritt...:   ";
  formDescript[11]="Bankname...:   ";
  formDescript[12]="BLZ........:   ";
  formDescript[13]="KontoNr....:   ";
  formDescript[14]="Ruh.Mitgl..:   ";

  // ** Variablen fuer Dialogbox **
  int x=7, y=3, count = 0;
  int width=61, height=20;
	int currentMember;

  // ** CURSOR anzeigen **
  _setcursortype(_SOLIDCURSOR);

  // ** Dialogbox anzeigen **
  if (showMsgBox(x,y, width,height, clrs) != eOK)
    exit(eNOT_OK);

  textbackground(clrs.dialogBk); textcolor(clrs.dialogTitle);
  gotoxy(x+4, y); cputs(" "); cwrite(formTitle); cputs(" ");

  // ** Buttons anlegen **
  tBUTTON menuButton[2] = {
    { (x+ 3),(y+height-3), "OK", eUNSELECTED },
    { (x+10),(y+height-3), "Abbrechen", eUNSELECTED },
  };
  int buttonNum = 2;
  int activeButton = 1;

	// ** Titel anzeigen **
  textbackground(clrs.dialogBk); textcolor(clrs.dialogTitle);
  gotoxy(x+4, y); cputs(" "); cwrite(formTitle); cputs(" ");

	// ** Buttons anzeigen **
  for (count=0; count <= buttonNum; count++)        // alle Buttons inaktiv
     menuButton[count-1].state = eUNSELECTED;

  for (count=0; count <= buttonNum; count++)        // alle Buttons zeigen
    showButton(menuButton[count-1], clrs, clrs.dialogBk);

  // ** Feld-Bezeichner zeigen **
  textbackground(clrs.dialogBk); textcolor(clrs.dialogTxt);
  for (int row=0; row<15; row++)  {
    gotoxy(x+3, y+1+row); cwrite(formDescript[row]);
  }
  gotoxy(x+18+eLEN_DATE+2, y+8); cwrite("immer Format: 31.12.2000");
   
    // ** Felder zeigen **
    textbackground(clrs.highlBk); textcolor(clrs.highlTxt);
    if (pMember->MemberID[0]=='@')  {
      gotoxy(x+18, y+1); cswrite("leer", eLEN_MEMBERID-1);
    }
    else {
      gotoxy(x+18, y+1); cswrite(pMember->MemberID, eLEN_MEMBERID-1);
    }
    gotoxy(x+18, y+2); cswrite(pMember->Name, eLEN_NAME-1);
    gotoxy(x+18, y+3); cswrite(pMember->Firstname, eLEN_FIRSTNAME-1);
    gotoxy(x+18, y+4); cswrite(pMember->Street, eLEN_STREET-1);
    gotoxy(x+18, y+5); cswrite(pMember->Place, eLEN_PLACE-1);
    gotoxy(x+18, y+6); cswrite(pMember->PostalCode, eLEN_POSTALCODE-1);
    gotoxy(x+18, y+7); cswrite(pMember->Phone, eLEN_PHONE-1);
    gotoxy(x+18, y+8); cswrite(pMember->Birthday, eLEN_DATE-1);
    gotoxy(x+18, y+9); cswrite(pMember->Grade, eLEN_GRADE-1);
    gotoxy(x+18, y+10); cswrite(pMember->LastExam, eLEN_DATE-1);
    gotoxy(x+18, y+11); cswrite(pMember->EntryDate, eLEN_DATE-1);
    gotoxy(x+18, y+12); cswrite(pMember->BankName, eLEN_BANKNAME-1);
    gotoxy(x+18, y+13); cswrite(pMember->BankID, eLEN_BANKID-1);
    gotoxy(x+18, y+14); cswrite(pMember->AccountNum, eLEN_ACCOUNTNUM-1);
    gotoxy(x+18, y+15); cswrite(pMember->CalmMembership, eLEN_CALMMEMBERSHIP-1);

    /* MEMBER_ID eingeben */
    gotoxy(x+18, y+1); cswrite(" ", eLEN_MEMBERID-1);
    gotoxy(x+18, y+1); gets(tempMember.MemberID);
    if (tempMember.MemberID[0] !=' ' && tempMember.MemberID[0] !='\0')
      pMember->MemberID = tempMember.MemberID;
    if (pMember->MemberID[0]=='@') {
      gotoxy(x+18, y+1); cswrite("leer", eLEN_MEMBERID-1);
    }
    else {
      gotoxy(x+18, y+1); cswrite(pMember->MemberID, eLEN_MEMBERID-1);
    }

    /* NAME eingeben */
    gotoxy(x+18, y+2); cswrite(" ", eLEN_NAME-1);
    gotoxy(x+18, y+2); gets(tempMember.Name);
    if (tempMember.Name[0] !=' ' && tempMember.Name[0] !='\0')
      pMember->Name = tempMember.Name;
    gotoxy(x+18, y+2); cswrite(pMember->Name, eLEN_NAME-1);

    /* FIRSTNAME eingeben */
    gotoxy(x+18, y+3); cswrite(" ", eLEN_FIRSTNAME-1);
    gotoxy(x+18, y+3); gets(tempMember.Firstname);
    if (tempMember.Firstname[0] !=' ' && tempMember.Firstname[0] !='\0')
      pMember->Firstname = tempMember.Firstname;
    gotoxy(x+18, y+3); cswrite(pMember->Firstname, eLEN_FIRSTNAME-1);

    /* STREET eingeben */
    gotoxy(x+18, y+4); cswrite(" ", eLEN_STREET-1);
    gotoxy(x+18, y+4); gets(tempMember.Street);
    if (tempMember.Street[0] !=' ' && tempMember.Street[0] !='\0')
      pMember->Street = tempMember.Street;
    gotoxy(x+18, y+4); cswrite(pMember->Street, eLEN_STREET-1);

    /* PLACE eingeben */
    gotoxy(x+18, y+5); cswrite(" ", eLEN_PLACE-1);
    gotoxy(x+18, y+5); gets(tempMember.Place);
    if (tempMember.Place[0] !=' ' && tempMember.Place[0] !='\0')
      pMember->Place = tempMember.Place;
    gotoxy(x+18, y+5); cswrite(pMember->Place, eLEN_PLACE-1);

    /* POSTALCODE eingeben */
    gotoxy(x+18,y+6); cswrite(" ", eLEN_POSTALCODE-1);
    gotoxy(x+18,y+6); gets(tempMember.PostalCode);
    if (tempMember.PostalCode[0] !=' ' && tempMember.PostalCode[0] !='\0')
      pMember->PostalCode = tempMember.PostalCode;
    gotoxy(x+18,y+6); cswrite(pMember->PostalCode, eLEN_POSTALCODE-1);

    /* PHONE eingeben */
    gotoxy(x+18,y+7); cswrite(" ", eLEN_PHONE-1);
    gotoxy(x+18,y+7); gets(tempMember.Phone);
    if (tempMember.Phone[0] !=' ' && tempMember.Phone[0] !='\0')
      pMember->Phone = tempMember.Phone;
    gotoxy(x+18,y+7); cswrite(pMember->Phone, eLEN_PHONE-1);

    /* BIRTHDAY eingeben */
    gotoxy(x+18,y+8); cswrite(" ", eLEN_DATE-1);
    gotoxy(x+18,y+8); gets(tempMember.Birthday);
    if (tempMember.Birthday[0] !=' ' && tempMember.Birthday[0] !='\0')
      pMember->Birthday = tempMember.Birthday;
    gotoxy(x+18,y+8); cswrite(pMember->Birthday, eLEN_DATE-1);

    /* GRADE eingeben */
    gotoxy(x+18,y+9); cswrite(" ", eLEN_GRADE-1);
    gotoxy(x+18,y+9); gets(tempMember.Grade);
    if (tempMember.Grade[0] !=' ' && tempMember.Grade[0] !='\0')
      pMember->Grade = tempMember.Grade;
    gotoxy(x+18,y+9); cswrite(pMember->Grade, eLEN_GRADE-1);

    /* LASTEXAM eingeben */
    gotoxy(x+18,y+10); cswrite(" ", eLEN_DATE-1);
    gotoxy(x+18,y+10); gets(tempMember.LastExam);
    if (tempMember.LastExam[0] !=' ' && tempMember.LastExam[0] !='\0')
      pMember->LastExam = tempMember.LastExam;
    gotoxy(x+18,y+10); cswrite(pMember->LastExam, eLEN_DATE-1);

    /* ENTRYDATE eingeben */
    gotoxy(x+18,y+11); cswrite(" ", eLEN_DATE-1);
    gotoxy(x+18,y+11); gets(tempMember.EntryDate);
    if (tempMember.EntryDate[0] !=' ' && tempMember.EntryDate[0] !='\0')
      pMember->EntryDate = tempMember.EntryDate;
    gotoxy(x+18,y+11); cswrite(pMember->EntryDate, eLEN_DATE-1);

    /* BANKNAME eingeben */
    gotoxy(x+18,y+12); cswrite(" ", eLEN_BANKNAME-1);
    gotoxy(x+18,y+12); gets(tempMember.BankName);
    if (tempMember.BankName[0] !=' ' && tempMember.BankName[0] !='\0')
      pMember->BankName = tempMember.BankName;
    gotoxy(x+18,y+12); cswrite(pMember->BankName, eLEN_BANKNAME-1);

    /* BANK_ID eingeben */
    gotoxy(x+18,y+13); cswrite(" ", eLEN_BANKID-1);
    gotoxy(x+18,y+13); gets(tempMember.BankID);
    if (tempMember.BankID[0] !=' ' && tempMember.BankID[0] !='\0')
      pMember->BankID = tempMember.BankID;
    gotoxy(x+18,y+13); cswrite(pMember->BankID, eLEN_BANKID-1);

    /* ACCOUNT_NUM eingeben */
    gotoxy(x+18,y+14); cswrite(" ", eLEN_ACCOUNTNUM-1);
    gotoxy(x+18,y+14); gets(tempMember.AccountNum);
    if (tempMember.AccountNum[0] !=' ' && tempMember.AccountNum[0] !='\0')
      pMember->AccountNum = tempMember.AccountNum;
    gotoxy(x+18,y+14); cswrite(pMember->AccountNum, eLEN_ACCOUNTNUM-1);

    /* CALM_MEMBERSHIP eingeben */
    gotoxy(x+18,y+15); cswrite(" ", eLEN_CALMMEMBERSHIP-1);
    gotoxy(x+18,y+15); gets(tempMember.CalmMembership);
    if (tempMember.CalmMembership[0] != ' ' && tempMember.CalmMembership[0] != '\0')
      pMember->CalmMembership = tempMember.CalmMembership;
    gotoxy(x+18,y+15); cswrite(pMember->CalmMembership, eLEN_CALMMEMBERSHIP-1);


  // ** Variable fuer Tasten-Steuerung
  char menuKey = ' ';         // Var fuer Tastenabfrage

  // ***Tastenabfrage, bis ESC***
  while (menuKey != Key_Escape)  {

    // Buttons anzeigen
    if (activeButton < 1)  activeButton = buttonNum;
    if (activeButton > buttonNum)  activeButton = 1;

    for (count=0; count <= buttonNum; count++)        // alle Buttons inaktiv
       menuButton[count-1].state = eUNSELECTED;

    menuButton[activeButton-1].state = eSELECTED;     // aktiven Btn. setzen

    for (count=0; count <= buttonNum; count++)        // alle Buttons zeigen
      showButton(menuButton[count-1], clrs, clrs.dialogBk);

    // Taste abfragen und pruefen
    menuKey = getch();
    switch( menuKey )  {
      case Key_Escape:    // ESC = Bildschirm verlassen
        // Mitgliedsdaten zuruecksetzen
        deleteData(pMember);
        _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
        // Mitgliedsdaten zuruecksichern
        copyData(pMember, &origMember);
        return eOK;
        break;
      case Key_Enter:		  // Enter = Button-Kommando ausfuehren
        switch (activeButton) {
          case 1:
            _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
            return eOK; break;
          case 2:
            _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
            // Mitgliedsdaten zuruecksetzen
            deleteData(pMember);
            // Mitgliedsdaten zuruecksichern
            copyData(pMember, &origMember);
            return eOK; break;
        }
        break;
      case Key_F1:        // F1 = Hilfe aufrufen
        _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
        if (showHelp(clrs) != eOK)
          exit(eNOT_OK);
        if (editForm(pMember, formTitle,clrs) != eOK)
          exit(eNOT_OK);
        return eOK;
        break;
      case Key_Left:      // einen Button nach links
        activeButton--;
        break;
      case Key_Right:     // einen Button nach rechts
        activeButton++;
        break;
    }
  } // Ende -Tastenabfrage

  _setcursortype(_NOCURSOR); // ** CURSOR ausblenden **
  return eOK; // Alles OK
}
// Ende: editForm(..)



/////////////////////////////////////////////////////////////////////////////
//	Neue Daten eingeben: addnewForm(formTitle,clrs,numOfMembers)
//
int addnewForm(char *formTitle, tCOLORS clrs, int *numOfMembers)
{
	// ** Parameter pruefen **
	if (strlen(formTitle)==0) {
		cprintf("\n\raddnewForm: Kein Titelnamen angegeben!");
		exit(eWRONG_PARAMS);
  }
  if (*numOfMembers+1 >= eMAX_MEMBERS) {
    cprintf("\n\raddnewForm: Maximale Anzahl erlaubter Datensaetze erreicht!");
    cprintf("\n\r            Maximal: %d,  Aktuell: %d", eMAX_MEMBERS, numOfMembers+1);
    exit(eWRONG_PARAMS);
  }

  // Zwischenspeicher fuer neue Daten
  tMEMBER tempMember;
  // Mitgliedsdaten zuruecksetzen
  deleteData(&tempMember);

  // Neue Mitgliedsdaten abfragen
  editForm(&tempMember, "Neuer Datensatz", clrs);

  // Pruefen, ob neuer Datensatz leer
  if (tempMember.MemberID[0] != '@') {

    refreshData(numOfMembers);

    int nMember = *numOfMembers+1;
    // Neue Daten in Datenbank kopieren
    copyData(&sMember[nMember], &tempMember);

    refreshData(numOfMembers);
  };

  return eOK; // Alles OK
}
// Ende: addnewForm(..)


///////////////////////////////////////////////////////////////////////////////
//	Liste zeichnen: 	showList(formTitle, clrs);
//
int showList(char *listTitle, tCOLORS clrs)
{
	// ** Parameter pruefen **
	if (strlen(listTitle)==0) {
		cprintf("\n\rshowList: Kein Titelname angegeben!");
		exit(eWRONG_PARAMS);
  }

  // ** Variablen fuer Dialogbox **
  int x=7, y=3;
  int width=63, height=20;

  // ** Dialogbox anzeigen **
  if (showMsgBox(x,y, width,height, clrs) != eOK)
    exit(eNOT_OK);

  textbackground(clrs.dialogBk); textcolor(clrs.dialogTitle);
  gotoxy(x+4, y); cputs(" "); cwrite(listTitle); cputs(" ");

	// ** Tabellentitel anzeigen **
	textbackground(clrs.dialogBk); textcolor(clrs.dialogTxt);
	gotoxy(x+3, y+1);
	  cwrite("Nr.   Name           Vorname       ");
	  cwrite("  Grad    Geburtstag");

  return eOK;
}
// Ende: showList(..)


///////////////////////////////////////////////////////////////////////////////
//	Listen-Bildschirm: 	execList(formTitle, clrs);
//
int execList(char *listTitle, tCOLORS clrs, int *numOfMembers)
{
  showList(listTitle, clrs);

  // ** Variablen fuer Dialogbox **
  int x=7, y=3, count=0;
  int width=63, height=20;

  // ** Variablen fuer Tasten-Steuerung
  char menuKey = ' ';         // Var fuer Tastenabfrage
  char dialogKey = ' ';       // Var fuer Tastenabfrage
  int curPos = 1;             // aktuelle CursorPosition in der Liste
  int firstMemberInList = 0;  // Nummer des 1.Mitgliedes in sichtbarer Liste
  int static maxEntries = 14;        // Anz. maximal sichtbarer Eintraege in Liste
  int selEntry = 1;           // derzeit gewaehlter Datensatz:
                              //   selEntry = firstMemberInList + curPos -1

  // ** Buttons anlegen **
  tBUTTON menuButton[5] = {
    { (x+ 3),(y+height-3), "Neu", eUNSELECTED },
    { (x+11),(y+height-3), "Anzeigen", eUNSELECTED },
    { (x+24),(y+height-3), "Bearbeiten", eUNSELECTED },
    { (x+39),(y+height-3), "Lîschen", eUNSELECTED },
    { (x+51),(y+height-3), "Suchen", eUNSELECTED}
  };
  int buttonNum = 5;
  int activeButton = 2;


  // ** Tastenabfrage, bis Esc gedrueckt ***
  while (menuKey != Key_Escape)  {

  sortData(1, numOfMembers);

    // ++ Cursor-Position setzen ++

    // Cursor nach oben
    if (curPos < 1)  {
			if (firstMemberInList > 0) {
        // Seitenweise in Liste blaettern
        if (firstMemberInList + curPos >= 0) {
          firstMemberInList += curPos;
        }
        else {  firstMemberInList=0;  }
				curPos = 1;
      }
      else {
				firstMemberInList = *numOfMembers - maxEntries;
				curPos = maxEntries;
      }
    }
    // Cursor nach unten
    if (curPos > maxEntries) {
			if (firstMemberInList+maxEntries >= *numOfMembers) {
				firstMemberInList = 0;
				curPos = 1;
      }
      else {
        if ( firstMemberInList + curPos -1 <= *numOfMembers - maxEntries) {
          firstMemberInList += curPos - 1;
        }
        else {  firstMemberInList = *numOfMembers - maxEntries;  }
        curPos = maxEntries;
      }
		}
    selEntry = firstMemberInList + curPos - 1;

    // ++ alle sichtbaren Listen-Eintraege zeigen ++
    textbackground(clrs.highlBk); textcolor(clrs.highlTxt);
    for (count=firstMemberInList; count < maxEntries+firstMemberInList; count++)  {
      gotoxy(x+3, y+2+count-firstMemberInList);

      if (sMember[count].MemberID[0] != '@')  {
        cswrite(sMember[count].MemberID, 4);

        textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
	      cswrite(sMember[count].Name, 14);
	    	textbackground(clrs.dialogBk); cputs(","); textbackground(clrs.highlBk);
	      cswrite(sMember[count].Firstname, 14);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite(sMember[count].Grade, 6);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite(sMember[count].Birthday, 10);
      }
    // ***** WENN DATENSATZ LEER::
    else {
        cswrite("leer", 4);
        textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
	      cswrite("               ", 14);
	    	textbackground(clrs.dialogBk); cputs(","); textbackground(clrs.highlBk);
	      cswrite("               ", 14);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite("               ", 6);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite("               ", 10);
      }
    }

    // ++ markierten Eintrag zeigen ++
    textbackground(clrs.cursorBk); textcolor(clrs.cursorTxt);
    gotoxy(x+3, y+1+curPos);
    if (sMember[selEntry].MemberID[0] != '@')  {
      cswrite(sMember[selEntry].MemberID, 4);

      textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.cursorBk);
	    cswrite(sMember[selEntry].Name, 14);
	  	textbackground(clrs.dialogBk); cputs(","); textbackground(clrs.cursorBk);
	    cswrite(sMember[selEntry].Firstname, 14);

	  	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.cursorBk);
      cswrite(sMember[selEntry].Grade, 6);

	  	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.cursorBk);
      cswrite(sMember[selEntry].Birthday, 10);
    }
    // ***** WENN DATENSATZ LEER::
    else {
        cswrite("leer", 4);
        textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
	      cswrite("               ", 14);
	    	textbackground(clrs.dialogBk); cputs(","); textbackground(clrs.highlBk);
	      cswrite("               ", 14);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite("               ", 6);

	    	textbackground(clrs.dialogBk); cputs("  "); textbackground(clrs.highlBk);
        cswrite("               ", 10);
      }


    // ** Buttons anzeigen **
    if (activeButton < 1)  activeButton = buttonNum;
    if (activeButton > buttonNum)  activeButton = 1;

    for (count=0; count <= buttonNum; count++)        // alle Buttons inaktiv
       menuButton[count-1].state = eUNSELECTED;

       menuButton[activeButton-1].state = eSELECTED;  // aktiven Btn. setzen

    for (count=0; count <= buttonNum; count++)        // alle Buttons zeigen
      showButton(menuButton[count-1], clrs, clrs.dialogBk);

    // ++ Taste abfragen und pruefen ++
    menuKey = getch();
    switch( menuKey )  {
      case Key_Escape:    // Listen-Ansicht verlassen
        return eOK;
        break;

      case Key_Enter:     // markierten Eintrag oeffnen

        switch (activeButton) {
          case 1:
            if (addnewForm("Neuer Datensatz", clrs, numOfMembers) != eOK)
				    	exit(eNOT_OK);
              sortData(1, numOfMembers);   // Daten sortieren
            curPos = 1; firstMemberInList = 0; selEntry = 1;
            if (showList(listTitle, clrs) != eOK)
              exit(eNOT_OK);
            break;

          case 2:
            if (showForm(&sMember[selEntry], "Anzeigen", clrs) != eOK)
              exit(eNOT_OK);
            sortData(1, numOfMembers);   // Daten sortieren
            if (showList(listTitle, clrs) != eOK)
              exit(eNOT_OK);
            break;

          case 3:
            if (editForm(&sMember[selEntry],"Bearbeiten", clrs) != eOK)
              exit(eNOT_OK);
            sortData(1, numOfMembers);   // Daten sortieren
            curPos = 1; firstMemberInList = 0; selEntry = 1;
            if (showList(listTitle, clrs) != eOK)
              exit(eNOT_OK);
            break;

          case 4:
            if (showMsgBox(10,10, 60,6, msgBoxclrs) != eOK)
              exit(eNOT_OK);

            textbackground(msgBoxclrs.dialogBk);
            textcolor(msgBoxclrs.dialogTitle);
            gotoxy(15,10); cwrite(" Datensatz Loeschen ");

            textbackground(msgBoxclrs.dialogBk);
            textcolor(msgBoxclrs.dialogTxt);
            gotoxy(14,12); cwrite("Wollen Sie diesen Datensatz wirklich loeschen (J/N)?");

            textbackground(msgBoxclrs.highlBk);
            textcolor(msgBoxclrs.highlTxt);
            gotoxy(14,13); cswrite(sMember[selEntry].Firstname,20);
            gotoxy(35,13); cswrite(sMember[selEntry].Name,20);
            gotoxy(56,13); cswrite(sMember[count].Birthday, 10);

            dialogKey = getch();
            if ( dialogKey == 'j' || dialogKey == 'J') {
              deleteData(&sMember[selEntry]);
              *numOfMembers--;
            }

            sortData(1, numOfMembers);   // Daten sortieren
            curPos = 1; firstMemberInList = 0; selEntry = 1;
            if (showList(listTitle, clrs) != eOK)
              exit(eNOT_OK);
            break;

          case 5:
            break;
        }
        break;

      case Key_F1:        // F1 = Hilfebildschirm anzeigen
        if (showHelp(clrs) != eOK)
          exit(eNOT_OK);
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;

      case Key_Insert:
      case Key_F2:        // F2 = neuen Datensatz anlegen
        if (addnewForm("Neuer Datensatz", clrs, numOfMembers) != eOK)
					exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
        curPos = 1; firstMemberInList = 0; selEntry = 1;
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;

      case Key_F3:        // F3 = Datensatz anzeigen
        if (showForm(&sMember[selEntry], "Anzeigen", clrs) != eOK)
          exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;

      case Key_F4:        // F4 = Datensatz Bearbeiten
        if (editForm(&sMember[selEntry],"Bearbeiten", clrs) != eOK)
          exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
        curPos = 1; firstMemberInList = 0; selEntry = 1;
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;
/* +++++
      case Key_F5:        // F5 = Datensatz Suchen
        if (searchForm("Suchen", clrs) != eOK)
          exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
   curPos = 1; firstMemberInList = 0; selEntry = 1;
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;
+++++ */
/* +++++
      case Key_F6:        // F6 = Datensatz Drucken
        if (searchForm("Drucken", clrs) != eOK)
          exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
   curPos = 1; firstMemberInList = 0; selEntry = 1;
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;
+++++ */
      case Key_Delete:
      case Key_F8:        // F8 = Datensatz Loeschen
        if (showMsgBox(10,10, 60,6, msgBoxclrs) != eOK)
          exit(eNOT_OK);

        textbackground(msgBoxclrs.dialogBk);
        textcolor(msgBoxclrs.dialogTitle);
        gotoxy(15,10); cwrite(" Datensatz Loeschen ");

        textbackground(msgBoxclrs.dialogBk);
        textcolor(msgBoxclrs.dialogTxt);
        gotoxy(14,12); cwrite("Wollen Sie diesen Datensatz wirklich loeschen (J/N)?");

        textbackground(msgBoxclrs.highlBk);
        textcolor(msgBoxclrs.highlTxt);
        gotoxy(14,13); cswrite(sMember[selEntry].Firstname,20);
        gotoxy(35,13); cswrite(sMember[selEntry].Name,20);
        gotoxy(56,13); cswrite(sMember[count].Birthday, 10);

        dialogKey = getch();
        if ( dialogKey == 'j' || dialogKey == 'J') {
          deleteData(&sMember[selEntry]);
          //++++ *numOfMembers--;
        }

        sortData(1, numOfMembers);   // Daten sortieren
        curPos = 1; firstMemberInList = 0; selEntry = 1;
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;
/* +++++
      case Key_F10:       // F10 = Konfiguration des Programms
        if (configForm("Konfiguration", clrs) != eOK)
          exit(eNOT_OK);
        sortData(1, numOfMembers);   // Daten sortieren
        if (showList(listTitle, clrs) != eOK)
          exit(eNOT_OK);
        break;
+++++ */
      case Key_Up:        // Cursor in Liste nach oben
        curPos--;
        break;
      case Key_Down:      // Cursor in Liste nach unten
        curPos++;
        break;
      case Key_PageUp:        // Cursor in Liste 1 Seite nach oben
        curPos -= maxEntries;
        break;
      case Key_PageDown:      // Cursor in Liste 1 Seite nach unten
        curPos += maxEntries;
        break;
      case Key_Left:      // einen Button nach links
        activeButton--;
        break;
      case Key_Right:     // einen Button nach rechts
        activeButton++;
        break;
    }
  } // Ende der Tastenabfrage

  return eOK; // Alles OK
}
// Ende: execList(..)

//
// Ende: Menue-System
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//	MAIN
//
int main()
{
  ////////////////////////////////////////////////////////////////////////////
  // Umgebungs-Variablen

  // ** Eigenschaften von HauptMenue **
  tMENU mainMenu;
  // ++ Titel ++
  mainMenu.title="MitgliedsDatenbank             EDO VeKa 0.04";
  // ++ Farben ++
  mainMenu.clrs.text = 11;       mainMenu.clrs.textBk = 1;
  mainMenu.clrs.dialogTxt = 0;   mainMenu.clrs.dialogBk = 7;
  mainMenu.clrs.dialogFrame =15; mainMenu.clrs.dialogTitle = 14;
  mainMenu.clrs.frameTxt = 4;    mainMenu.clrs.frameBk = 3;
  mainMenu.clrs.titleTxt = 4;    mainMenu.clrs.titleBk = 7;
  mainMenu.clrs.highlTxt = 0;    mainMenu.clrs.highlBk = 3;
  mainMenu.clrs.cursorTxt = 15;  mainMenu.clrs.cursorBk = 2;
  mainMenu.clrs.statusTxt = 0;   mainMenu.clrs.statusBk = 3;
  mainMenu.clrs.buttonTxt = 0;   mainMenu.clrs.buttonBk = 6;
  mainMenu.clrs.selButtonTxt=15; mainMenu.clrs.selButtonBk = 2;
	mainMenu.clrs.shaddow = 8;
  // ++ Funktionstasten ++
  mainMenu.funcKey[0].name="F1"; mainMenu.funcKey[0].descript="Hilfe";
  mainMenu.funcKey[1].name="F2"; mainMenu.funcKey[1].descript="Neu";
  mainMenu.funcKey[2].name="F3"; mainMenu.funcKey[2].descript="Anzeigen";
  mainMenu.funcKey[3].name="F4"; mainMenu.funcKey[3].descript="Bearbeiten";
  mainMenu.funcKey[4].name="F5"; mainMenu.funcKey[4].descript="Suchen";
  mainMenu.funcKey[5].name="F6"; mainMenu.funcKey[5].descript="Drucken";
  mainMenu.funcKey[6].name="F8"; mainMenu.funcKey[6].descript="Lîschen";
  mainMenu.funcKey[7].name="F10"; mainMenu.funcKey[7].descript="Konfig";
  // ++ Farben ++
  msgBoxclrs.text = 15;       msgBoxclrs.textBk = 1;
  msgBoxclrs.dialogTxt = 7;   msgBoxclrs.dialogBk = 4;
  msgBoxclrs.dialogFrame = 14; msgBoxclrs.dialogTitle = 15;
  msgBoxclrs.frameTxt = 0;    msgBoxclrs.frameBk = 3;
  msgBoxclrs.titleTxt =14;    msgBoxclrs.titleBk = 4;
  msgBoxclrs.highlTxt = 15;    msgBoxclrs.highlBk = 2;
  msgBoxclrs.cursorTxt = 15;  msgBoxclrs.cursorBk = 2;
  msgBoxclrs.statusTxt = 0;   msgBoxclrs.statusBk = 3;
  msgBoxclrs.buttonTxt = 0;   msgBoxclrs.buttonBk = 6;
  msgBoxclrs.selButtonTxt=15; msgBoxclrs.selButtonBk = 2;
	msgBoxclrs.shaddow = 8;
  // ENDE: Umgebungs-Variablen
  ////////////////////////////////////////////////////////////////////////////

  // ** Variablen, Felder **
  int membersNum = 0;             // Anzahl der gespeicherten Mitglieder

  ////////////////////////////////////////////////////////////////////////////
  // Datei-Handle einrichten, Datei oeffnen
  tFILE MemberFile = { NULL, NULL, "members.dat"};
  openFile(&MemberFile, O_RDONLY );

  // Alle Mitgliedsdaten zuruecksetzen
  for (int num=0; num <= eMAX_MEMBERS; num++)  {
     deleteData(&sMember[num]);
  }

  ////////////////////////////////////////////////////////////////////////////
  // Daten LADEN aus Datei
  membersNum = 0;
  for (int num=0; num <= eMAX_MEMBERS; num++)   // bis EOF Eintraege lesen
  {
    // Daten einlesen + in sMember[1..n] speichern, bis Dateiende erreicht
    if (read(MemberFile.Hnd, &sMember[num], sizeof(tMEMBER)) != 0)  {
        if (sMember[num].MemberID[0] !=' ' &&
        sMember[num].MemberID[0] != '\0' && sMember[num].MemberID[0] !='@')
        {
          membersNum++;
        }
    }
    else
      break;  // Schleife beenden, wenn EOF: read(..)=0
  }

  sortData(1, &membersNum);  // Daten sortieren
  // Daten einlesen beendet
  ////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////
  //
  // !!!!  IMPORT  CSV-FILE  !!!!
  //
  //  EXPERIMENTAL!!!!

	cImportCSV memberFile;

	strcpy( memberFile.CSV_File.Name, "test.csv" );

	memberFile.openFile( &memberFile.CSV_File );

	memberFile.importDataSets( membersNum );
  //
  // END: Import CSV-File
  ////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////
  // Desktop
  textmode(C80);                      // Textmodus setzen
  _setcursortype(_NOCURSOR);
  textbackground(mainMenu.clrs.textBk); textcolor(mainMenu.clrs.text);
  clrscr();

  // Titelleiste in 1.Zeile
  textbackground(mainMenu.clrs.titleBk); textcolor(mainMenu.clrs.titleTxt);
  gotoxy(1,1); clreol(); cputs(" "); cwrite(mainMenu.title);

  // Funktionstasten in Statuszeile
  gotoxy(1,25);
  for (int count=0; count <= 7; count++) {
    textbackground(mainMenu.clrs.textBk); textcolor(mainMenu.clrs.text);
    cputs(" "); cwrite(mainMenu.funcKey[count].name);

    textbackground(mainMenu.clrs.statusBk); textcolor(mainMenu.clrs.statusTxt);
    cwrite(mainMenu.funcKey[count].descript);
  }

  // Hauptmenue oeffnen
  execList("Liste", mainMenu.clrs, &membersNum);

  // Menuesystem beendet
  ////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////
  // Datei schliessen, Inhalt loeschen, Datei neu oeffnen
  closeFile(&MemberFile);
  truncate("members.dat",0);
  openFile(&MemberFile, O_WRONLY );

  // Daten SPEICHERN - alle Datensaetze neu schreiben
  membersNum = 0;
  lseek(MemberFile.Hnd, 0, SEEK_SET);  // Zeiger auf Dateianfang
  for (int num=0; num <= eMAX_MEMBERS; num++)  {
    if (sMember[num].MemberID[0] == '@' || sMember[num].Name[0]=='\0') {
    }
    else {
      if (write(MemberFile.Hnd, &sMember[num], sizeof(tMEMBER)) != 0)  {
          membersNum++;
      }
    }
  }

  // Datei schliessen
  closeFile(&MemberFile);
  // Speichern beendet
  ////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////
  // Programm verlassen
  textbackground(0); textcolor(7);
  clrscr();
  printf("\n\rTschÅÅÅ·!!!\n\r\n");

  return eOK;
}
//
// Ende: MAIN()
/////////////////////////////////////////////////////////////////////////////
