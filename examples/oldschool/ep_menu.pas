{   LOGO aus Zeichen des ASCII-Codes
             ⁄ƒƒƒƒƒƒø  ⁄ƒƒƒƒƒƒƒƒø
             ≥      ≥  ≥        ≥
             ≥  ⁄ƒƒƒŸ  ¿ø  ⁄ƒø  ≥
             ≥  ≥       ≥  ≥ ≥  ≥
             ≥  ≥       ≥  ≥ ≥  ≥
             ≥  ¿ƒƒƒø   ≥  ¿ƒŸ  ≥
             ≥      ≥   ≥       ≥
             ¿ƒƒƒƒƒƒŸ   ¿ƒƒƒƒƒƒƒŸ
   ENDE des LOGOs       EASY QuickStart Vers.1.3 - Frei kopierbar !!!
   ..geschrieben und entwickelt von: Christian DÑhn (C) Rehna 1996   }

PROGRAM Unknown;  {$M 8192,0,0 } {$I-}

  USES Crt,Dos;
  TYPE Menu = ARRAY[1..10] OF RECORD
       Beschreib: STRING[60];
       Verzeichn: STRING[60];
       Befehl:    STRING[60];
  END;

  VAR
    Taste, Flag: Char;
    I, A, CurPosMin, CurPosMax, CurPos: Integer;
    HauptMen: Menu;
    Piepton_EIN: Boolean;
    AbfragStr, Directory, Beschreib, Verzeichn, Befehl, Beschreibung: STRING;
    Datei: Text;
    Verzeichnis: STRING;
    y, m, d, dow : Word;
    AutoStart: Char;
  CONST
    CheckBreak: Boolean = False;
    Leerzeichen: STRING[52] = '                                                    ';


  { -- FORWARD-Deklarationen -- }
  { }


  PROCEDURE Start_Initialisieren; FORWARD;
  PROCEDURE Haupt; FORWARD;
  PROCEDURE Haupt_Bildschirmmaske; FORWARD;
  PROCEDURE Haupt_Tastensteuerung; FORWARD;
  PROCEDURE Haupt_Cursor; FORWARD;
  PROCEDURE Hilfe; FORWARD;
  PROCEDURE Einstellungen; FORWARD;
  PROCEDURE Entfernen (Beschreibung: STRING); FORWARD;
  PROCEDURE Hinzufuegen; FORWARD;
  PROCEDURE Speichern; FORWARD;
  PROCEDURE Einlesen; FORWARD;
  PROCEDURE BatchFile (Befehl: STRING); FORWARD;
  PROCEDURE Ende; FORWARD;
  PROCEDURE Beep1; FORWARD;
  PROCEDURE Beep2; FORWARD;
  PROCEDURE Fenster (X1, Y1, X2, Y2, F1, F2: Integer); FORWARD;
  FUNCTION Vielfach (Laenge: Byte; Zeichen: Char): STRING; FORWARD;
  PROCEDURE Zentriere (Zeile: Integer; TextStr: STRING); FORWARD;
  PROCEDURE Ausfuehren (Befehl: STRING); FORWARD;
  PROCEDURE Fehler (Error: Integer); FORWARD;


  { -- Prozeduren fÅr Initialisierung und Programmablauf -- }
  { }


  PROCEDURE Start;
    { S T A R T }
    { }
    { Steuerung des Programmstarts }
    BEGIN
      CurPos:=1;
      Start_Initialisieren;
    END;

  PROCEDURE Start_Initialisieren;
    { Umgebung initialisieren }
    BEGIN
      Window(1,1, 80,25);
      CheckBreak:=FALSE;
      GetDate(y, m, d, dow);
      Verzeichnis:=GetEnv('QSTART');
      { MenÅeintrÑge und Einstellungen einlesen }
      Einlesen;
    END;


  PROCEDURE Haupt;
    { H A U P T }
    { }
    { Hier wird der Programmablauf gesteuert }
    BEGIN
      CurPosMin:=1; CurPosMax:=10;
      Haupt_Bildschirmmaske;
      Haupt_Tastensteuerung;
    END;

  PROCEDURE Haupt_Bildschirmmaske;
    { Aufbau der HauptmenÅ-Bildschirmmaske }
    BEGIN
      { Hintergrund und Titelleisten }
      TextColor(3); TextBackGround(0); ClrScr;
      FOR I:= 1 TO 2000 DO Write(#177);
      TextColor(15); TextBackGround(4);
      GotoXY(1,1); Write(' #HauptmenÅ                             ',
                         '                                        ');
      GotoXY(70,1); Write(d,'.',m,'.',y);
      TextColor(14);
      Zentriere(1,'EASY QuickStart Vers.1.3');
      TextColor(15); TextBackGround(4);
      GotoXY(1,25); DelLine;
      GotoXY(1,25); Write('  F1=Hilfe  ESC=Beenden  Pfeile=Bewegen  Zahl',
                          '=Direkte Auswahl');
      TextColor(14);
      GotoXY(03,25); Write('F1');       GotoXY(13,25); Write('ESC');
      GotoXY(26,25); Write('Pfeile');   GotoXY(42,25); Write('Zahl');
      { HauptmenÅ - Fenster }
      Fenster(7,10,20,70, 15,1);
      TextColor(15); TextBackGround(1);
      Zentriere(7,'µ  HauptmenÅ  ∆');
      { Info-Text }
      TextColor(3); TextBackGround(0);
      Zentriere(23,'Christian DÑhn (C) Rehna 1996');
      { Signalton }
      IF Piepton_EIN=TRUE THEN Beep1;
    END; { Ende Haupt_Bildschirmmaske }


  PROCEDURE Haupt_Tastensteuerung;
    { Tastensteuerun und Programmablauf im HauptmenÅ }
    LABEL Start_Abfrage1;
    BEGIN
        Haupt_Cursor;
        { Schleife fÅr die Tastenabfrage - Zuweisung von Ereignissen }
      Start_Abfrage1:;
        Taste:=ReadKey;
        (* 1 *)
        IF (Taste=#13) AND (CurPos=1) OR (Taste='1') THEN
          BEGIN
            CurPos:=1;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[1].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[1].Befehl);
            Haupt;
          END;
        (* 2 *)
        IF (Taste=#13) AND (CurPos=2) OR (Taste='2') THEN
          BEGIN
            CurPos:=2;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[2].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[2].Befehl);
            Haupt;
          END;
        (* 3 *)
        IF (Taste=#13) AND (CurPos=3) OR (Taste='3') THEN
          BEGIN
            CurPos:=3;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[3].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[3].Befehl);
            Haupt;
          END;
        (* 4 *)
        IF (Taste=#13) AND (CurPos=4) OR (Taste='4') THEN
          BEGIN
            CurPos:=4;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[4].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[4].Befehl);
            Haupt;
          END;
        (* 5 *)
        IF (Taste=#13) AND (CurPos=5) OR (Taste='5') THEN
          BEGIN
            CurPos:=5;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[5].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[5].Befehl);
            Haupt;
          END;
        (* 6 *)
        IF (Taste=#13) AND (CurPos=6) OR (Taste='6') THEN
          BEGIN
            CurPos:=6;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[6].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[6].Befehl);
            Haupt;
          END;
        (* 7 *)
        IF (Taste=#13) AND (CurPos=7) OR (Taste='7') THEN
          BEGIN
            CurPos:=7;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(HauptMen[7].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[7].Befehl);
            Haupt;
          END;
        (* 8 *)
        IF (Taste=#13) AND (CurPos=8) OR (Taste='8') THEN
          BEGIN
            CurPos:=8;
            TextColor(7); TextBackGround(0); ClrScr;
            ChDir(Hauptmen[8].Verzeichn); IF IOResult<>0 THEN Fehler(3);
            BatchFile(HauptMen[8].Befehl);
            Haupt;
          END;
        (* 9 *)
        IF (Taste=#13) AND (CurPos=9) OR (Taste='9') THEN
          BEGIN
            CurPos:=9;
            Window(1,1,80,25); TextColor(7); TextBackGround(0); ClrScr;
            Fenster(1,1, 5,79, 7,0);
            Zentriere(2,'MS-DOS bzw. MS-Windows 95 EINGABEAUFFORDEUNG');
            Zentriere(3,'FÅr Hilfe HELP eingeben');
            Zentriere(4,'Mit EXIT zum Programm zurÅckkehren');
            GotoXY(1,5); WriteLn;
            Ausfuehren('COMMAND');
            Haupt;
          END;
        (* E *)
        IF (Taste=#13) AND (CurPos=10) OR (Taste='E') OR (Taste='e') THEN
          BEGIN
            CurPos:=10;
            Einstellungen;
            Haupt;
          END;
        (* Funtionstasten *)
        IF Taste=#27 THEN Ende;
        IF Taste=#59 THEN BEGIN Hilfe; Haupt; END;
        IF Taste='H' THEN BEGIN CurPos:=CurPos-1; Haupt_Cursor; END;
        IF Taste='P' THEN BEGIN CurPos:=CurPos+1; Haupt_Cursor; END;
      GOTO Start_Abfrage1;
    END; { Ende Haupt_Tastensteuerung }


  PROCEDURE Haupt_Cursor;
    { Cusorsteuerung des HauptmenÅs }
    { -> Bewegen des Auswahl-Balkens mit den Pfeiltasten }
    BEGIN
      { MenÅeintrÑge in original-Farbe zeigen }
      TextColor(7); TextBackGround(1);
      FOR I:= 1 TO 10 DO
        BEGIN GotoXY(13,8+I); Write(' ',HauptMen[I].Beschreib,' '); END;
      { Cursorposition ÅberprÅfen }
      IF CurPos<=CurPosMin-1 THEN CurPos:=CurPosMax;
      IF CurPos>=CurPosMax+1 THEN CurPos:=CurPosMin;
      { Balken auf der neuen Position zeigen }
      TextColor(0); TextBackGround(2);
      GotoXY(13,8+CurPos); Write(' ',HauptMen[CurPos].Beschreib,' ');
    END; { Ende Haupt_Cursor }


  { -- Unterprogramm: "Hilfe" -- }
  { }


  PROCEDURE Hilfe;
    { Zeigt einen Hilfebildschirm, mit kurzen ErklÑrungen }
    BEGIN
      { Hintergrund und Titelleisten }
      TextColor(3); TextBackGround(0); ClrScr;
      FOR I:= 1 TO 2000 DO Write(#177);
      TextColor(15); TextBackGround(4);
      GotoXY(1,1); Write(' #Hilfebildschirm                       ',
                         '                                        ');
      GotoXY(70,1); Write(d,'.',m,'.',y);
      TextColor(14);
      Zentriere(1,'EASY QuickStart Vers.1.3');
      TextColor(15); TextBackGround(4);
      GotoXY(1,25); DelLine;
      GotoXY(1,25); Write('  ESC=ZurÅck zum HauptmenÅ');
      TextColor(14);
      GotoXY(03,25); Write('ESC');
      { Fenster mit Hilfestellungen zum Programm }
      Fenster (3,4, 23,76, 15,1);
      Zentriere(3,'µ Hilfebildschirm ∆');
      TextColor(10);
      GotoXY(8,05); Write('Bedienung: ');
      GotoXY(8,14); Write('Infos....: ');
      TextColor(7);
      (* Bedienung *)
      GotoXY(20,05); Write('Im MenÅ "Programmeigenschaften" kînnen Sie immer');
      GotoXY(20,06); Write('zwischen J oder N wÑhlen. Mit Enter kînnen Sie');
      GotoXY(20,07); Write('die Abfragen mit "(J/N)?" Åberspringen - die');
      GotoXY(20,08); Write('Einstellungen werden dadurch NICHT verÑndert!');
      GotoXY(20,09); Write('In der letzten Bildschirmzeile finden Sie die');
      GotoXY(20,10); Write('verfÅgbaren Tasten und deren Funktion.');
      GotoXY(20,11); Write('Nur im HauptmenÅ kann man mit den Pfeiltasten');
      GotoXY(20,12); Write('oder bestimmten Zahlen etwas AuswÑhlen');
      (* Infos *)
      GotoXY(20,14); Write('Bei weiteren Fragen zum Programm oder bei Problemen');
      GotoXY(20,15); Write('helfe ich Ihnen gerne - schnell und unkompliziert!');
      GotoXY(20,17); Write('   Christian DÑhn');
      GotoXY(20,18); Write('   Puschkinplatz 8, 19217 Rehna');
      GotoXY(20,19); Write('   038872/51558');
      GotoXY(20,21); Write('EASY QuickStart 1.3 - Christian DÑhn (C) Rehna 1996');

      IF Piepton_EIN=TRUE THEN Beep1;
      ReadKey;
    END; { Ende Hilfe }

  { Unterprogramm: "Einstellungen"  }
  { }


  PROCEDURE Einstellungen;
    { éndern der MenÅeintrÑge und einiger Programm-Parameter }
    LABEL Start_Abfrag2, Einstell_OK, A1, A2, A2_not_OK, A3;
    BEGIN
     Start_Abfrag2:;
      { Bildschirmmaske - Titelleisten und Hintergrundmaske }
      TextColor(15); TextBackGround(4);
      GotoXY(1,1); Write(' #Einstellungen                         ',
                         '                                        ');
      GotoXY(70,1); Write(d,'.',m,'.',y);
      TextColor(14);
      Zentriere(1,'EASY QuickStart Vers.1.3');

      TextColor(14); TextBackGround(4);
      GotoXY(1,25); DelLine;
      GotoXY(1,25); Write('  Enter=Abfrage Åberspringen  ESC=Zeile lîschen',
                          '  ');
      { Maske fÅr das MenÅ "Einstellungen" }
      Fenster(10, 10, 23,69, 0,2);
      TextColor(14);
      Zentriere(11,'Einstellungen');
      TextColor(0);
      GotoXY(11,12); Write(Vielfach(59,#196));
      TextColor(15);
      GotoXY(14,14); Write('Piepton erzeugen (J/N)?......................... ');
      GotoXY(14,15); Write('Neues Programm ins MenÅ einfÅgen (J/N)?......... ');
      GotoXY(14,16); Write('Programm aus dem MenÅ entfernen (J/N)?.......... ');
      GotoXY(14,17); Write('MenÅ automatisch nach dem Start (J/N)?.......... ');
      TextColor(0);
      GotoXY(11,19); Write(Vielfach(59,#196));
      IF Piepton_EIN=TRUE THEN Beep1;
      { Vorgegebene Wahlmîglichkeiten anzeigen }
      TextColor(11);
      GotoXY(64,14);
      IF Piepton_EIN=TRUE THEN Write('J')
        ELSE Write('N');
      GotoXY(64,15); Write('N');
      GotoXY(64,16); Write('N');
      GotoXY(64,17); Write('J');
      { Auswahl im MenÅ - "piepton erzeugen (j/n)?" }
     A1:;
      (* Bildschirm vorbereiten *)
      TextColor(4);
      Zentriere(20,' J oder N eingeben, mit ENTER Abfrage Åberspringen. ');
      TextColor(11);
      GotoXY(64,14);
      IF Piepton_EIN=TRUE THEN Flag:='J'
        ELSE Flag:='N';
      Write(Flag);
      (* Abfrage von J/N *)
      Window(64,14, 69,14);
      GotoXY(64,14); ReadLn(AbfragStr);
      Window(1,1, 80,25);
      (* Antwort prÅfen *)
      IF AbfragStr='' THEN AbfragStr:=Flag;
      IF (AbfragStr<>'N') AND (AbfragStr<>'J')
      AND (AbfragStr<>'n') AND (AbfragStr<>'j') THEN
        BEGIN
          (* Falsche Antwort *)
          TextColor(4);
          Zentriere(20,'                Falsche Eingabe !                   ');
          Beep2; Beep2;
          Delay(400);
          GOTO A1;
        END;
      (* Richtige Antwort *)
      GotoXY(64,14); Write(AbfragStr);
      IF (AbfragStr='J') OR (AbfragStr='j') THEN Piepton_EIN:=TRUE
      ELSE Piepton_EIN:=FALSE;
      AbfragStr:='';
      Zentriere(20,'                                                    ');
      { Auswahl im MenÅ - "neues programm... (j/n)?" }
     A2:;
      TextColor(4);
      Zentriere(20,' J oder N eingeben, mit ENTER Abfrage Åberspringen. ');
      TextColor(11);
      Window(64,15, 69,15);
      GotoXY(64,15); ReadLn(AbfragStr);
      Window(1,1, 80,25);
      IF AbfragStr='' THEN AbfragStr:='N';
      IF (AbfragStr<>'N') AND (AbfragStr<>'J')
      AND (AbfragStr<>'n') AND (AbfragStr<>'j') THEN
        (* Falsche Antwort *)
        BEGIN
          TextColor(4);
          Zentriere(20,'                Falsche Eingabe !                   ');
          Beep2;
          Delay(400);
          GOTO A2;
        END;
      GotoXY(64,15); Write(AbfragStr);
      (* Abfrage der Daten zum neuen MenÅeintrag *)
     A2_not_OK:;
      IF (AbfragStr='J') OR (AbfragStr='j') THEN
        BEGIN
          TextColor(4);
          Zentriere(20, Leerzeichen);
          Zentriere(21, Leerzeichen);
          Zentriere(22,'Beschreibung ohne die Nummer (z.B.: "1. ") eingeben!');
          TextColor(15);
          GotoXY(14,20); Write('Beschreibung: ');
          TextColor(1); ReadLn(Beschreib); TextColor(15);
          Zentriere(22, Leerzeichen);
          GotoXY(14,21); Write('Verzeichnis: C:\'); TextColor(1);
            GotoXY(27,21); ReadLn(Verzeichn); TextColor(15);
            IF Verzeichn='' THEN Verzeichn:='C:\';
          GotoXY(14,22); Write('Befehl: ');
          TextColor(1); ReadLn(Befehl);
          (* Sicherheitsabfragen - Speichern der Daten *)
          IF Piepton_EIN=TRUE THEN Beep1;
          TextColor(4);
          GotoXY(14,19); Write('Eingabe in Ordnung (J/N)? ');
          TextColor(1); ReadLn(AbfragStr);
          IF (AbfragStr<>'J') AND (AbfragStr<>'j') THEN
            BEGIN AbfragStr:='J'; GOTO A2_not_OK; END;
          TextColor(4);
          GotoXY(14,19); Write('Diese Daten speichern (J/N)? ');
          TextColor(1); ReadLn(AbfragStr);
          IF (Beschreib<>'') AND (AbfragStr='J') OR (AbfragStr='j') THEN Hinzufuegen;
          (* Weiter zur nÑchsten Abfrage *)
          Zentriere(20, Leerzeichen);
          Zentriere(21, Leerzeichen);
          Zentriere(22, Leerzeichen);
          TextColor(0);
          GotoXY(11,19); Write(Vielfach(59,#196));
        END;
      AbfragStr:='';
      Zentriere(20, Leerzeichen);
     { Auswahl im MenÅ - "programm aus menÅ entfernen (j/n)?" }
     A3:;
      TextColor(4);
      Zentriere(20,' J oder N eingeben, mit ENTER Abfrage Åberspringen. ');
      TextColor(11);
      Window(64,16, 69,16);
      GotoXY(64,16); ReadLn(AbfragStr);
      Window(1,1, 80,25);
      IF AbfragStr='' THEN AbfragStr:='N';
      IF (AbfragStr<>'N') AND (AbfragStr<>'J')
      AND (AbfragStr<>'n') AND (AbfragStr<>'j') THEN
        BEGIN
          TextColor(4);
          Zentriere(20,'                Falsche Eingabe !                   ');
          Beep2;
          Delay(400);
          GOTO A3;
        END;
      GotoXY(64,16); Write(AbfragStr);
      IF (AbfragStr='J') OR (AbfragStr='j') THEN
        BEGIN
          Zentriere(20, Leerzeichen);
          TextColor(4);
          Zentriere(22,'Beschreibung ohne die Nummer (z.B.: "1. ") eingeben!');
          TextColor(15);
          GotoXY(14,20); Write('Beschreibung: ');
          TextColor(1); ReadLn(Beschreib);
          IF Piepton_EIN=TRUE THEN Beep1;
          TextColor(4);
          GotoXY(14,19); Write('Programm aus dem MenÅ entfernen (J/N)? ');
          TextColor(1); ReadLn(AbfragStr);
          IF (AbfragStr='J') OR (AbfragStr='j') THEN
            BEGIN
              TextColor(4);
              GotoXY(14,19); Write('Diesen Eintrag wirklich lîschen (J/N)? ');
              IF Piepton_EIN=TRUE THEN Beep1;
              TextColor(1); ReadLn(AbfragStr);
              IF (AbfragStr='J') OR (AbfragStr='j') THEN Entfernen (Beschreib);
            END;
          Zentriere(20, Leerzeichen);
          Zentriere(22, Leerzeichen);
          TextColor(0);
          GotoXY(11,19); Write(Vielfach(59,#196));
        END;
      AbfragStr:='';
      Zentriere(20, Leerzeichen);
     { Auswahl im MenÅ - "menÅ autom. nach dem start (j/n)?" }
      TextColor(4);
      Zentriere(20,' J oder N eingeben, mit ENTER Abfrage Åberspringen. ');
      TextColor(11);
      Window(64,17, 69,17);
      GotoXY(64,17); ReadLn(AbfragStr);
      Window(1,1, 80,25);
      IF (AbfragStr='N') OR (AbfragStr='n') THEN
        BEGIN
          Ausfuehren('IF EXIST '+Verzeichnis+'\AUTOEXEC.BAT DEL C:\AUTOEXEC.BAT');
          Ausfuehren('COPY '+Verzeichnis+'\AUTOEXEC.BAT C:\ >NUL');
          AutoStart:='N';
        END
      ELSE
        BEGIN
          Assign(Datei, 'C:\AUTOEXEC.BAT');
          Reset(Datei);
          Flag:='N';
          Directory:=Concat(Verzeichnis,'\QSTART.BAT');
          WHILE NOT EOF(Datei) DO
            BEGIN
              ReadLn(Datei,AbfragStr);
              IF AbfragStr=Directory THEN
                BEGIN Flag:='J'; Break; END;
            END;
          Close(Datei);
          IF Flag='N' THEN
            BEGIN
              Ausfuehren('COPY C:\AUTOEXEC.BAT '+Verzeichnis+' >NUL');
              Ausfuehren('ECHO SET PATH=%PATH%;'+Verzeichnis+' >>C:\AUTOEXEC.BAT');
              Ausfuehren('ECHO '+Verzeichnis+'\QSTART.BAT >>C:\AUTOEXEC.BAT');
            END;
          AutoStart:='J';
          TextColor(11);
          GotoXY(64,17); Write(AutoStart);
        END;
      TextColor(11); TextBackGround(2);
      GotoXY(64,17); Write(AutoStart);
      Zentriere(20, Leerzeichen);
      { Sicherheitsabfrage - Speichern der Einstellungen }
      IF Piepton_EIN=TRUE THEN Beep1;
      TextColor(15);
      GotoXY(14,20); Write('Sind alle Eingaben in Ordnung (J/N)? ');
      Window(51,20, 64,20);
      TextColor(11); ReadLn(AbfragStr);
      Window(1,1, 80,25);
      IF (AbfragStr='J') OR (AbfragStr='j') THEN GOTO Einstell_OK;
      Zentriere(20, Leerzeichen);
      GOTO Start_Abfrag2;
      { Einstellungen speichern }
     Einstell_OK:;
     Speichern;
    END; { Ende Einstellungen }


  { -- Unterprogramme fÅr zusÑtzliche Funktionen -- }
  { }


  PROCEDURE Entfernen (Beschreibung: STRING);
    { PrÅft den eintrag und entfernt ihn aus dem MenÅ }
    BEGIN
      FOR I:=1 TO 8 DO
        BEGIN
          IF (Beschreibung='Eingabeaufforderung') OR
          (Beschreibung='Programmeinstellungen') THEN
            BEGIN Fehler(12); Haupt; END;
          AbfragStr:=COPY(HauptMen[I].Beschreib,4,36);
          IF AbfragStr=Beschreibung THEN
            BEGIN
              HauptMen[I].Beschreib:='';
              HauptMen[I].Verzeichn:='';
              HauptMen[I].Befehl:='';
              Speichern;
              Haupt;
            END;
        END;
    END; { Ende Entfernen }


  PROCEDURE Hinzufuegen;
    { öberprÅft, ob noch platz fÅr einen neuen MenÅeintrag }
    { ist und fÅgt einen neuen MenÅeintrag ein }
    BEGIN
      FOR I:=1 TO 8 DO
        BEGIN
          AbfragStr:=COPY(HauptMen[I].Beschreib,1,1);
          IF AbfragStr='' THEN
            BEGIN
              Str(I,AbfragStr);
              HauptMen[I].Beschreib:=AbfragStr+'. '+Beschreib;
              HauptMen[I].Verzeichn:=Verzeichn;
              HauptMen[I].Befehl:=Befehl;
              Speichern;
              Haupt;
            END;
          IF (AbfragStr<>'') AND (I=8) THEN BEGIN Fehler(14); Haupt; END;
        END;
    END; { Ende Hinzufuegen }


  PROCEDURE Speichern;
    { Speichert alle aktuellen Einstellungen und Daten in der MenÅdatei }
    BEGIN
      { Leere MenÅeintrÑge nach unten verlagern }
      FOR I:=1 TO  8 DO
        BEGIN
          IF HauptMen[I].Beschreib='' THEN
            BEGIN
              (* -- *)
              FOR A:=0 TO 8-I DO
                BEGIN
                  HauptMen[I+A].Beschreib:=HauptMen[I+A+1].Beschreib;
                  HauptMen[I+A].Verzeichn:=HauptMen[I+A+1].Verzeichn;
                  HauptMen[I+A].Befehl:=HauptMen[I+A+1].Befehl;
                END;
              (* -- *)
              HauptMen[08].Beschreib:='';
              HauptMen[08].Verzeichn:='';
              HauptMen[08].Befehl:='';
            END;
        END;
      { Die Zahlen der MenÅeintrÑge korrigieren }
      FOR I:=1 TO 8 DO
        BEGIN
          Str(I, AbfragStr);
          AbfragStr:=AbfragStr+'.';
          IF Copy(HauptMen[I].Beschreib,4,1)<>'' THEN
           HauptMen[I].Beschreib:=AbfragStr+Copy(HauptMen[I].Beschreib,3,57);
        END;
      { Daten in Datei Speichern }
      Directory:=Concat(Verzeichnis,'\MENUE.DAT');
      Assign(Datei,Directory);
      Rewrite(Datei);
      WriteLn(Datei,Verzeichnis);
      IF Piepton_EIN=TRUE THEN AbfragStr:='Piep_EIN'
      ELSE AbfragStr:='Piep_AUS';
      WriteLn(Datei,AbfragStr);
      FOR I:=1 TO 10 DO
        BEGIN
          WriteLn(Datei,HauptMen[I].Beschreib);
          WriteLn(Datei,HauptMen[I].Verzeichn);
          WriteLn(Datei,HauptMen[I].Befehl);
        END;
      Close(Datei); IF IOResult<>0 THEN Halt(10)
    END; { Ende Speichern }


  PROCEDURE Einlesen;
    { Liest alle Einstellungen und Daten aus der MenÅdatei ein }
    BEGIN
      IF Copy(Directory,Length(Directory)-1,1)='\' THEN
        Directory:=Copy(Directory,1,Length(Directory)-1);
      { Daten aus Datei einlesen }
      Directory:=Concat(Verzeichnis,'\MENUE.DAT');
      Assign(Datei,Directory);
      Reset(Datei);
      IF IOResult<>0 THEN BEGIN Fehler(20); Exit; END;
      ReadLn(Datei,Verzeichnis);
      ReadLn(Datei,AbfragStr);
      IF AbfragStr='Piep_EIN' THEN Piepton_EIN:=TRUE
      ELSE Piepton_EIN:=FALSE;
      FOR I:=1 TO 10 DO
        BEGIN
          ReadLn(Datei,HauptMen[I].Beschreib);
          ReadLn(Datei,HauptMen[I].Verzeichn);
          ReadLn(Datei,HauptMen[I].Befehl);
        END;
      Close(Datei);
      {IF (HauptMen[09].Beschreib='') OR
      (HauptMen[10].Beschreib='') THEN Fehler(20);}
    END; { Ende Einlesen }


  PROCEDURE BatchFile (Befehl: STRING);
    { Erzeugt eine Stapelverarbeitungsdatei mit dem angegebenen Befehl }
    BEGIN
      IF Befehl='' THEN BEGIN Fehler(16); Haupt; END;
      Str(CurPos,AbfragStr);
      Directory:=Concat(Verzeichnis,'\MENUE',AbfragStr,'.BAT');
      Assign(Datei,Directory);
      Rewrite(Datei);
      WriteLn(Datei,'@ECHO OFF');
      WriteLn(Datei,Befehl);
      Close(Datei);
      Speichern;
      Halt(curPos);
    END; { Ende BatchFile }


  PROCEDURE Ende;
    { Beendet dieses Programm und zeigt eine InfoBox auf dem Bildschirm }
    BEGIN
      { Programm beenden }
      Window(1,1, 80,25);
      TextColor(7); TextBackGround(0);
      ClrScr;
      Fenster(1,1, 5,79, 7,0);
      Zentriere(2,'E A S Y  QuickStart');
      Zentriere(3,'Version 1.3');
      Zentriere(4,'von Christian DÑhn (C) Rehna, Juli 1996');
      GotoXY(1,9); WriteLn;
      Delay(700);
      Speichern;
      Halt;
    END; { Unterprogramm: Ende }


  PROCEDURE Beep1;
    { Erzeugt einen Piepton fÅr Zustandsmeldungen im MenÅ }
    BEGIN
      Sound(800); Delay(100); NoSound;
    END; { Ende Beep1 }


  PROCEDURE Beep2;
    { Erzeugt einen Piepton fÅr Warnungen und Fehlermeldungen }
    BEGIN
      Sound(160); Delay(260); NoSound;
    END; { Ende Beep2 }


  PROCEDURE Fenster (X1, Y1, X2, Y2, F1, F2: Integer);
    { Beliebig gro·e Fensterrahmen                                      }
    { Erzeugt einen Rahmen mit den gegebenen Koordinaten und Farben     }
    { X1,Y1 = Obere linke Ecke des Fensters                             }
    { X2,Y2 = Untere rechte Ecke des Fensters,  Y2-Y1 = Breite          }
    { F1,F2 = Vordergrund- und Hintergrundfarbe                         }
    VAR Zaehler, Breite: Integer;
    BEGIN
      TextColor(F1); TextBackGround(F2);
      Breite:=Y2-Y1;
      GotoXY(Y1,X1); Write('…',Vielfach(Breite,'Õ'),'ª');
      FOR Zaehler:= X1+1 TO X2-1 DO
        BEGIN
          GotoXY(Y1,Zaehler); Write('∫',Vielfach(Breite,' '),'∫');
        END;
      GotoXY(Y1,X2); Write('»',Vielfach(Breite,'Õ'),'º');
    END; { Ende Fenster }


  FUNCTION Vielfach (Laenge: Byte; Zeichen: Char): STRING;
    { Mehrmalige Ausgabe einer Zeichenfolge auf dem Bildschirm          }
    VAR Zaehler: Integer;
    BEGIN
      FOR Zaehler:= 1 TO Laenge DO Write(Zeichen);
      Vielfach:='';
    END; { Ende Vielfach }


  PROCEDURE Zentriere (Zeile: Integer; TextStr: STRING);
    { Text erscheint in der Mitte der angegebenen Zeile                 }
    VAR Spalte: Integer;
    BEGIN
      Spalte:=40-(Length(TextStr) DIV 2);
      GotoXY(Spalte,Zeile); Write(TextStr);
    END; { Ende Zentriere }


  PROCEDURE Ausfuehren (Befehl: STRING);
    { FÅhrt einen DOS-Befehl oder ein Programm aus                      }
    BEGIN
      DosError:=0;
      SwapVectors;
      IF (Befehl <> '') AND (Befehl <> 'COMMAND') THEN
        Exec(GetEnv('COMSPEC'), '/C' + Befehl);
      IF Befehl = 'COMMAND' THEN Exec(GetEnv('COMSPEC'),'');
      SwapVectors;
      Delay(600);
    END; { Ende Ausfuehren }


  PROCEDURE Fehler (Error: Integer);
    { Zeigt ein Fenster mit der Meldung zum angegebenen Fehler }
    { Zu einigen Fehlern gehîrt eine Fehlerbehebung, }
    { die es dem Benutzer ermîglicht, den Fehler selbst zu beheben }
    LABEL Zuweisen, VerzEingeben, VerzSuchen, Eingabe;
    BEGIN
      IF Error=0 THEN Exit;
      { Meldungsfenster darstellen }
      Zentriere(25,'                    ESC lîscht den Inhalt der Zeile                           ');
      Fenster(10,10, 19,69, 14,4);
      TextColor(7); TextBackGround(1);
      Zentriere(10, ' Fehler bei ProgrammausfÅhrung ');
      TextColor(15); TextBackGround(4);
      { Fehler Nr.02 }
      IF Error=2 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.02 -->  Datei nicht gefunden');
          GotoXY(14,13);
          Write('Es wurde ein falscher Befehl eingegeben!');
          GotoXY(14,14);
          Write('Um den Befehl beizubehalten, Enter drÅcken.');
          TextBackGround(2);
          GotoXY(16,15); Write(HauptMen[CurPos].Befehl);
          TextBackGround(4);
          Beep2; Beep2;
          GotoXY(14,17); Write('Neuer Befehl: ');
          TextBackGround(1);
          Window(26,18, 65,17); ReadLn(Befehl); Window(1,1,80,25);
          IF Befehl<>'' THEN HauptMen[CurPos].Befehl:=Befehl;
        END;
      { Fehler Nr.03 }
      IF Error=3 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.03 -->  Pfad nicht gefunden');
          GotoXY(14,13);
          Write('Es wurde ein falsches Verzeichnis eingegeben!');
          GotoXY(14,14);
          Write('Um das Verzeichnis beizubehalten, Enter drÅcken.');
          TextBackGround(2);
          GotoXY(16,15); Write(HauptMen[CurPos].Verzeichn);
          TextBackGround(4);
          Beep2; Beep2;
          GotoXY(14,17); Write('Neues Verzeichnis: ');
          TextBackGround(1);
          Window(26,18, 65,17); ReadLn(Verzeichn); Window(1,1,80,25);
          IF Verzeichn<>'' THEN HauptMen[CurPos].Verzeichn:=Verzeichn;
        END;
      { Fehler Nr.05 }
      IF Error=5 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.05 -->  Zugriff verweigert');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.06 }
      IF Error=6 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.06 -->  UngÅltiges Handle');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.08 }
      IF Error=8 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.08 -->  Nicht genug Speicher');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.10 }
      IF Error=10 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.10 -->  UngÅltige Umgebung');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.11 }
      IF Error=11 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.11 -->  UngÅltiges Format');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.12 }
      IF Error=12 THEN
        BEGIN
          GotoXY(14,12);
          Write('Dieser MenÅeintrag ist ein fester Bestandteil');
          GotoXY(14,13);
          Write('des Programms und kann daher nicht entfernt werden!');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.14 }
      IF Error=14 THEN
        BEGIN
          GotoXY(14,12);
          Write('Es ist kein Platz fÅr weitere MenÅeintrÑge vorhanden,');
          GotoXY(14,13);
          Write('da nur bis zu 8 eigene MenÅeintrÑge mîglich sind.');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.16 }
      IF Error=16 THEN
        BEGIN
          GotoXY(14,12);
          Write('Es ist kein Befehl vorhanden.');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.18 }
      IF Error=18 THEN
        BEGIN
          GotoXY(14,12);
          Write('DOS-Fehler Nr.18 -->  Keine weiteren Dateien');
          Zentriere(17, 'Beliebige Taste drÅcken..');
          Beep2; Beep2;
          ReadKey;
        END;
      { Fehler Nr.20 }
      IF Error=20 THEN
        BEGIN
          GotoXY(14,12);
          Write('Prg.Fehler Nr.03 -->  MenÅeintrÑge nicht gefunden!');
          GotoXY(14,13);
          Write('Die Datei mit den MenÅeintrÑgen wurde gelîscht oder');
          GotoXY(14,14);
          Write('befindet sich nicht im aktuellen Verzeichnis.');
          GotoXY(14,15);
          Write('Die Datei wird neu erstellt, die EintrÑge gehen verloren');
          Beep2; Beep2;
          Zentriere(17, 'Beliebige Taste drÅcken..');
          ReadKey;
          HauptMen[09].Beschreib:='9. Eingabeaufforderung';
          HauptMen[10].Beschreib:='E. Programm-Einstellungen';
          Verzeichnis:='';
          Speichern;
          Einlesen;
        END;
      Window(1,1,80,25);
    END; { Ende Fehler }


  { -- Hauptprogramm -- }
  { }


  BEGIN
    { Aufrufen der beiden gro·en Unterprogramme }
    Start;
    Haupt;
  END.


  { -- Programmende -- }
  { }
