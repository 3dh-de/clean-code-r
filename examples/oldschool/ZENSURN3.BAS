'
'              * * * * * * * * * * * * * * * * * * * * * *
'                   EASY Mark2Point for MS-QBasic 1.1
'              * * * * * * * * * * * * * * * * * * * * * *
'
'
' Programmname..:  ZENSUREN.BAS
' Sprache.......:  MS-QBasic 1.1
' Funktionen....:  Umwandlung von Zensuren in Punkte, Noten mit + und -
'                  und Berechnung des Punkte-Durchschnitts von SchÅlern
'
' Merkmale......:  Hilfebildschirm ist mit der F1-Taste jederzeit aufrufbar.
'                  Alle Eingaben und Ergebnisse werden in Punkten
'                  und der entsprechenden Zensur (inklusive + und -)
'                  angezeigt, auch die Teilergebnisse erscheinen
'                  auf dem Bildschirm.
'                  Die Farben kînnen beliebig gewÑhlt werden (siehe Zeile 80).
'
' Autor.........:  Christian DÑhn (C) Rehna 1995-1996,
'                  in Zusammenarbeit mit Arne Heuschmann.
'
' Copyrights....:  Der Name EASY Mark2Point darf nicht unter
'                  einem anderen Copyright weitergegeben werden !
'
'
'       Dieses Programm ist Shareware, Sie dÅrfen es beliebig
'       weiterkopieren und Programmteile fÅr eigene Programme
'       verwenden. FÅr eventuelle Fehler und SchÑden im Zusam-
'       menhang mit Mark2Point Åbernehme ich keine Haftung !!
'


' SUBs deklarieren
'
DEFINT A-Z
'
DECLARE SUB FindNOTE (pnkt%)
DECLARE SUB FindPoint (Zensur AS STRING)
DECLARE SUB Hilfe ()
DECLARE SUB Default (f%)
DECLARE FUNCTION Trim$ (x$)
DECLARE FUNCTION GetString$ (row%, col%, start$, end$, Vis%, bereich%)


' Fehlerbehandlung installieren
'
ON ERROR GOTO Fehler


' Felder definieren
'
CONST TRUE = -1                 ' Konstante fÅr WAHR
CONST FALSE = NOT TRUE          ' Konstante fÅr FALSCH


' Variablen deklarieren
'
DIM Klausuren(99)       AS INTEGER      ' KlausDURCH AS SINGLE
DIM Muendlich(99)       AS INTEGER      ' MuendlDURCH AS SINGLE
DIM KlausNOTE(99)       AS STRING * 2
DIM MuendlNOTE(99)      AS STRING * 2
DIM KlausDURCHZensur    AS STRING * 2
DIM MuendlDURCHZensur   AS STRING * 2
DIM GesamtDURCHZensur   AS STRING * 2
DIM SHARED Zensur       AS STRING * 2
DIM SHARED Punkte, f, z
DIM SHARED TitelLeisteVordergrund, TitelLeisteHintergrund
DIM SHARED TitelLeisteHervorgehob
DIM SHARED TextFensterVordergrund, TextFensterHintergrund
DIM SHARED TeilErgebnisseVordergr, TeilErgebnisseHintergr
DIM SHARED GesamtDurschnittVorder, GesamtDurschnittHinter
DIM SHARED scrimagestr$(25), scrimage$(25, 70), Frage$


' Farbtabelle
'
'
'      Hinweis:      éndern Sie die Farben, indem Sie Zahlen von 0 bis 7
'      --------      fÅr den Hintergrund und die Zahlen 0 bis 15 fÅr den
'                    Vordergrund des Textes einsetzen.
'
'      Tabelle:         0 = Schwarz          8 = Grau
'      --------         1 = Blau             9 = Hellblau
'                       2 = GrÅn            10 = HellgrÅn
'                       3 = TÅrkis          11 = TÅrkis hell
'                       4 = Rot             12 = Orange
'                       5 = Rosa            13 = Hellrosa
'                       6 = Braun           14 = Gelb
'                       7 = Hellgrau        15 = Wei·
'


' Diese Variablen kînnen Sie mit anderen Farbattributen belegen
'
    TitelLeisteVordergrund = 11          ' Werte von 0 bis 15 mîglich
    TitelLeisteHintergrund = 4           ' Werte von 0 bis 7 mîglich
    TitelLeisteHervorgehob = 14          ' Werte von 0 bis 15 mîglich

    TextFensterVordergrund = 7           ' Werte von 0 bis 15 mîglich
    TextFensterHintergrund = 1           ' Werte von 0 bis 7 mîglich

    TeilErgebnisseVordergr = 15          ' Werte von 0 bis 15 mîglich
    TeilErgebnisseHintergr = 3           ' Werte von 0 bis 7 mîglich

    GesamtDurschnittVorder = 14          ' Werte von 0 bis 15 mîglich
    GesamtDurschnittHinter = 3           ' Werte von 0 bis 7 mîglich


' Sofern Vorder- und Hintergrundfarbe gleich, werden automatisch die
' Standardfarben verwendet
'
IF TitelLeisteVordergrund = TitelLeisteHintergrund THEN CALL Default(1)
IF TitelLeisteHervorgehob = TitelLeisteHintergrund THEN CALL Default(1)
IF TextFensterVordergrund = TextFensterHintergrund THEN CALL Default(2)
IF TeilErgebnisseVordergr = TextFensterHintergrund THEN CALL Default(3)
IF TeilErgebnisseVordergr = TeilErgebnisseHintergr THEN CALL Default(3)
IF GesamtDurschnittVorder = GesamtDurschnittHinter THEN CALL Default(4)


'
'              * * * * * * * * * * * * * * * * * * * * * *
'                   EASY Mark2Point  ! HAUPTPROGRAMM !
'              * * * * * * * * * * * * * * * * * * * * * *
'

' Bildschirmmaske aufbauen
'
Hauptprg.START:
  
    Titel$ = " Eingabebildschirm         Hilfe: F1-Taste"
    
    COLOR TextFensterVordergrund, TextFensterHintergrund
    VIEW PRINT 1 TO 25
    CLS
  
    COLOR TitelLeisteVordergrund, TitelLeisteHintergrund
    LOCATE 1: PRINT SPACE$(80)
    LOCATE 1: PRINT Titel$

    COLOR TitelLeisteHervorgehob
    LOCATE 1, 63: PRINT " EASY Mark2Point "
               

' Frage: Anzahl der Klausuren ?
'
Klausur.FRAGE:

    FOR i = 1 TO 99                          ' Inhalt der Variablen lîschen
        KlausNOTE(i) = "  "
    NEXT i
  
    COLOR TextFensterVordergrund, TextFensterHintergrund
    
    Frage$ = " Wieviele Klausuren/Arbeiten ? "
    PRINT
    PRINT Frage$                             ' Fragestellung zeigen
    
    i$ = GetString$(0, 32, "", st$, 2, 1)    ' String von Tastatur einlesen
    KlausANZAHL = VAL(st$)                   ' String in Zahl umwandeln

    PRINT
    PRINT
    

' Frage: Welche Noten ?  Antwort: Punktezahl, Durchschnitt
'
Klausur.NOTEN:
    
    GES1 = 0                                 ' Inhalt der Variablen lîschen
    KlausDURCH = 0
    KlausDURCHZensur = "  "
  
    FOR i = 1 TO KlausANZAHL                  ' Beginn der Abfrage-Schleife
       
        Zensur = "  "                         ' Inhalt der Variablen lîschen
        Punkte = 0
    
        Frage$ = "  Note Klausur.........: "
        PRINT Frage$
       
        i$ = GetString$(0, 26, "", Zensur, 2, 1)  ' String einlesen
        IF Zensur = "  " THEN
                KlausANZAHL = i - 1
                PRINT
                GOTO Klausuren.ERGEBNIS
        END IF
    
            CALL FindPoint(Zensur)           ' Punktezahl zur Zensur finden
           
            KlausNOTE(i) = Zensur
            Klausuren(i) = Punkte
           
            GES1 = GES1 + Klausuren(i)       ' Gesamte Punktezahl berechnen

        PRINT TAB(36); "  Punkte: "; Klausuren(i)

    NEXT i                                   ' Ende der Abfrage-Schleife


' Berechnung und Ausgabe der Teilergebnisse
'
Klausuren.ERGEBNIS:
  
    IF KlausANZAHL <= 0 THEN GOTO Muendlich.ABFRAGE

    KlausDURCH = GES1 / KlausANZAHL          ' Punktedurchschnitt berechnen
  
        CALL FindNOTE(KlausDURCH)            ' Zensur ermitteln
       
        KlausDURCHZensur = Zensur
   
    PRINT "  Gesamte Punktzahl....:";        ' Ergebnisse zeigen

        COLOR TeilErgebnisseVordergr
        PRINT GES1
  
        COLOR TextFensterVordergrund
        PRINT "  Punkte-Durchschnitt..:";

        COLOR TeilErgebnisseVordergr
        PRINT KlausDURCH;

        COLOR TextFensterVordergrund
        PRINT TAB(36); "  Zensur:  ";
  
        COLOR TeilErgebnisseVordergr
        PRINT KlausDURCHZensur

    COLOR TextFensterVordergrund             ' Ende des UnterPrg. Klausuren


' Abfrage: Anzahl der MÅndlichen Zensuren ?
'
Muendlich.ABFRAGE:

    FOR i = 1 TO 99                          ' Inhalt der Variablen lîschen
        MuendlNOTE(i) = "  "
    NEXT i
 
    Frage$ = " Wieviele mÅndliche Zensuren ? "
    PRINT
    PRINT Frage$
    
    i$ = GetString$(0, 32, "", st$, 2, 1)    ' String von Tastatur einlesen
    MuendlANZAHL = VAL(st$)                  ' String in Zahl umwandeln
  
    PRINT
    PRINT


' Frage: Welche Noten ?  Antwort: Punktezahl, Durchschnitt
'
Muendlich.NOTEN:
    
    GES2 = 0                                 ' Inhalt der Variablen lîschen
    MuendlDURCH = 0
  
    FOR i = 1 TO MuendlANZAHL                ' Beginn der Abfrage-Schleife
       
        Zensur = "  "                        ' Inhalt der Variablen lîschen
        Punkte = 0
    
        Frage$ = "  Note mÅndlich........: "
        PRINT Frage$
     
        i$ = GetString$(0, 26, "", Zensur, 2, 1)
        IF Zensur = "  " THEN
                MuendlANZAHL = i - 1
                PRINT
                GOTO Muendlich.ERGEBNIS
        END IF
     
            CALL FindPoint(Zensur)           ' Punkte zur Zensur finden
           
            MuendlNOTE(i) = Zensur
            Muendlich(i) = Punkte
    
        GES2 = GES2 + Muendlich(i)      ' Gesamt-Punktzahl fÅr mÅndl.Zensuren
    
        PRINT TAB(36); "  Punkte: "; Muendlich(i)

    NEXT i


' Berechnung und Ausgabe der Teilergebnisse
'
Muendlich.ERGEBNIS:
    
    IF MuendlANZAHL <= 0 THEN GOTO Gesamt.ERGEBNIS
   
    MuendlDURCH = GES2 / MuendlANZAHL        ' Punktedurchschnitt berechnen
  
        CALL FindNOTE(MuendlDURCH)           ' Zensur ermitteln
       
        MuendlDURCHZensur = Zensur
 
    PRINT "  Gesamte Punktzahl....:";        ' Teilergebnisse zeigen
  
        COLOR TeilErgebnisseVordergr
        PRINT GES2
  
        COLOR TextFensterVordergrund
        PRINT "  Punkte-Durchschnitt..:";
  
        COLOR TeilErgebnisseVordergr
        PRINT MuendlDURCH;
  
        COLOR TextFensterVordergrund
        PRINT TAB(36); "  Zensur:  ";
  
        COLOR TeilErgebnisseVordergr
        PRINT MuendlDURCHZensur

    COLOR TextFensterVordergrund             ' Ende des UnterPrg. Muendlich



' Berechnung und Ausgabe der End-Ergebnisse
'
Gesamt.ERGEBNIS:
    
    IF MuendlNOTE(1) = "  " AND KlausNOTE(1) = "  " THEN
        GOTO Hauptprg.ENDE
    END IF

    IF KlausANZAHL + MuendlANZAHL <= 0 THEN GOTO Hauptprg.ENDE

  
    GesDURCH = (GES1 + GES2) / (KlausANZAHL + MuendlANZAHL)   ' Durchschnitt
    
        CALL FindNOTE(GesDURCH)              ' Zensur ermitteln
        GesamtDURCHZensur = Zensur

   PRINT
   PRINT " Ges.Punkte-Durchschnitt:";
  
        COLOR GesamtDurschnittVorder
        PRINT GesDURCH;
       
        COLOR TextFensterVordergrund
        PRINT TAB(36); "  Zensur:  ";
  
        COLOR GesamtDurschnittVorder
        PRINT Zensur
  
    COLOR TextFensterVordergrund


' Abfrage: Sollen alle Ergebnisse ausgedruckt werden
'
Drucken.ABFRAGE:

    COLOR TextFensterVordergrund, TextFensterHintergrund

    Frage$ = " Die Ergebnisse ausdrucken (J/N)? "
    PRINT
    PRINT Frage$
  
    i$ = GetString$(0, 35, "", Druck$, 1, 0)
    
        IF UCASE$(Druck$) = "J" THEN GOTO Drucken.AUSFUEHREN


' Abfrage: Wollen Sie weitere Eingaben durchfÅhren ?  Bei "Nein" - Ende.
'
Hauptprg.ENDE:

    Frage$ = " Wollen Sie weitere Berechnungen durchfÅhren (J/N)? "
    PRINT
    PRINT
    PRINT Frage$
  
    i$ = GetString$(0, 53, "", weiter$, 1, 0)
    
        IF UCASE$(weiter$) = "N" THEN GOTO Hauptprg.VERLASSEN

    GOTO Hauptprg.START


' Programmende
'
Hauptprg.VERLASSEN:

    COLOR 7, 0
    CLS
    PRINT "TschÅÅÅ· !!!"                      ' Das ist mein Markenzeichen !
    PRINT

END


' Unterprogramm: Drucken auf LPT1
'
Drucken.AUSFUEHREN:
  
    COLOR TextFensterVordergrund, TextFensterHintergrund
  
    PRINT
  
    LINE INPUT " Name, Klasse, Fach des SchÅlers: ", info$
    PRINT
  
        COLOR TitelLeisteVordergrund, TitelLeisteHintergrund
        PRINT " Drucke... ";
  
        COLOR TextFensterVordergrund, TextFensterHintergrund
        PRINT SPACE$(80)
  
        COLOR TextFensterVordergrund, TextFensterHintergrund
       
        LPRINT "* * *  Zensuren von "; info$; "  * * *"
        LPRINT
        LPRINT "Zensuren/Punkte von Klausuren/Arbeiten:"
        LPRINT
  
    FOR i = 1 TO KlausANZAHL
        LPRINT "  Note Klausur.........: "; KlausNOTE(i);
        LPRINT "  Punkte...............: "; Klausuren(i)
    NEXT i
       
        LPRINT "  Gesamte Punktzahl....:"; GES1
                LPRINT "  Punkte-Durchschnitt..:"; KlausDURCH;
                        LPRINT "   Zensur: "; KlausDURCHZensur
        LPRINT
        LPRINT "Zensuren mÅndliche Leistungen:"
        LPRINT

    FOR i = 1 TO MuendlANZAHL
        LPRINT "  Note mÅndlich........: "; MuendlNOTE(i);
        LPRINT "  Punkte...............: "; Muendlich(i)
    NEXT i
        LPRINT "  Gesamte Punktzahl....:"; GES2
                LPRINT "  Punkte-Durchschnitt..:"; MuendlDURCH;
                        LPRINT "   Zensur: "; MuendlDURCHZensur
        LPRINT
        LPRINT " Punkte-Durchschnitt fÅr Klausuren und MÅndlich:";
                LPRINT GesDURCH; "   Zensur: "; GesamtDURCHZensur

    GOTO Hauptprg.ENDE

' *** Fehlerbehandlung ***

Fehler:
  
    COLOR TextFensterVordergrund, TextFensterHintergrund
        PRINT " "
    
    COLOR TitelLeisteHervorgehob, TitelLeisteHintergrund
  
' *** Zeigt Meldung anhand der Laufzeitfehler-Codes ***

    SELECT CASE ERR
      
        CASE 1
            PRINT "NEXT ohne FOR! ";
            GOTO Fehler.ENDE
        CASE 2
            PRINT "Syntaxfehler! ";
            GOTO Fehler.ENDE
        CASE 3
            PRINT " RETURN ohne GOSUB! ";
            GOTO Fehler.ENDE
        CASE 4
            PRINT "READ jenseits von DATA! ";
            GOTO Fehler.ENDE
        CASE 5
            PRINT "UnzulÑssiger Funktionsaufruf! ";
            GOTO Fehler.ENDE
        CASE 6
            PRINT "öberlauf! ";
            GOTO Fehler.ENDE
        CASE 7
            PRINT "Zu wenig Speicher! ";
            GOTO Fehler.ENDE
        CASE 8
            PRINT "Marke nicht definiert! ";
            GOTO Fehler.ENDE
        CASE 9
            PRINT "Index au·erhalb des zulÑssigen Bereichs! ";
            GOTO Fehler.ENDE
        CASE 10
            PRINT "Doppelte Definition! ";
            GOTO Fehler.ENDE
        CASE 11
            PRINT "Division durch Null! ";
            GOTO Fehler.ENDE
        CASE 12
            PRINT "UnzulÑssig im Direkt-Modus! ";
            GOTO Fehler.ENDE
        CASE 13
            PRINT "Falscher Datentyp! ";
            GOTO Fehler.ENDE
        CASE 14
            PRINT "Zu wenig Zeichenketten-Speicherplatz! ";
            GOTO Fehler.ENDE
        CASE 16
            PRINT "Zeichenketten-Formel zu umfangreich! ";
            GOTO Fehler.ENDE
        CASE 17
            PRINT "QBasic kann nicht fortsetzen! ";
            GOTO Fehler.ENDE
        CASE 18
            PRINT "QBasic-Funktion n. definiert! ";
            GOTO Fehler.ENDE
        CASE 19
            PRINT "RESUME fehlt! ";
            GOTO Fehler.ENDE
        CASE 20
            PRINT "RESUME ohne Fehler! ";
            GOTO Fehler.ENDE
        CASE 24
            PRINT "ZeitÅberschreitung am GerÑt! ";
            GOTO Fehler.ENDE
        CASE 25
            PRINT "GerÑtefehler! ";
            GOTO NochmalVersuchen
        CASE 26
            PRINT "FOR ohne NEXT! ";
            GOTO Fehler.ENDE
        CASE 27
            PRINT "Papier zu Ende! ";
            GOTO Fehler.ENDE
        CASE 29
            PRINT "WHILE ohne WEND! ";
            GOTO Fehler.ENDE
        CASE 30
            PRINT "WEND ohne WHILE! ";
            GOTO Fehler.ENDE
        CASE 33
            PRINT "Doppelt definierte Marke! ";
            GOTO Fehler.ENDE
        CASE 35
            PRINT "QBasic -Unterprogramm nicht definiert! ";
            GOTO Fehler.ENDE
        CASE 37
            PRINT "Falsche Anzahl von Argumenten! ";
            GOTO Fehler.ENDE
        CASE 38
            PRINT "Feld nicht definiert! ";
            GOTO Fehler.ENDE
        CASE 40
            PRINT "Variable erforderlich! ";
            GOTO Fehler.ENDE
        CASE 50
            PRINT "FIELD-öberlauf! ";
            GOTO Fehler.ENDE
        CASE 51
            PRINT "Interner Fehler! ";
            GOTO Fehler.ENDE
        CASE 52
            PRINT "Dateiname oder - nummer unzulÑssig! ";
            GOTO Fehler.ENDE
        CASE 53
            PRINT "Datei nicht gefunden! ";
            GOTO Fehler.ENDE
        CASE 54
            PRINT "Falscher Dateimodus! ";
            GOTO Fehler.ENDE
        CASE 55
            PRINT "Datei bereits geîffnet! ";
            GOTO Fehler.ENDE
        CASE 56
            PRINT "FIELD-Anweisung aktiv! ";
            GOTO Fehler.ENDE
        CASE 57
            PRINT "GerÑtefehler bei Ein/Ausgabe! ";
            GOTO Fehler.ENDE
        CASE 58
            PRINT "Datei existiert bereits! ";
            GOTO Fehler.ENDE
        CASE 59
            PRINT "UngÅltige DatensatzlÑnge! ";
            GOTO Fehler.ENDE
        CASE 61
            PRINT "Festplatte/Diskette voll! ";
            GOTO Fehler.ENDE
        CASE 62
            PRINT "Eingabe nach Dateiende! ";
            GOTO Fehler.ENDE
        CASE 63
            PRINT "UnzulÑssige Datensatznummer! ";
            GOTO Fehler.ENDE
        CASE 64
            PRINT "UnzulÑssiger Dateiname! ";
            GOTO Fehler.ENDE
        CASE 67
            PRINT "Zu viele Dateien! ";
            GOTO Fehler.ENDE
        CASE 68
            PRINT "GerÑt nicht verfÅgbar! ";
            GOTO Fehler.ENDE
        CASE 69
            PRINT "Kommunikationspuffer-öberlauf! ";
            GOTO Fehler.ENDE
        CASE 70
            PRINT "Zugriff verweigert! ";
            GOTO Fehler.ENDE
        CASE 71
            PRINT "Festplatte/Diskette nicht bereit! ";
            GOTO Fehler.ENDE
        CASE 72
            PRINT "DatentrÑgerfehler! ";
            GOTO Fehler.ENDE
        CASE 73
            PRINT "Funktion nicht verfÅgbar! ";
            GOTO Fehler.ENDE
        CASE 74
            PRINT "Umbenennen zwischen DatentrÑgern! ";
            GOTO Fehler.ENDE
        CASE 75
            PRINT "Pfad/Datei-Zugriffsfehler! ";
            GOTO Fehler.ENDE
        CASE 76
            PRINT "Pfad nicht gefunden! ";
            GOTO Fehler.ENDE
  
    END SELECT


' Fehlerbedingter Abbruch des Programms.
'
Fehler.ENDE:
  
    BEEP: BEEP
       
        PRINT " Programm wird jetzt beendet. "

        COLOR TextFensterVordergrund, TextFensterHintergrund
        PRINT SPACE$(80)
        PRINT "Beliebige Taste drÅcken...."

    DO UNTIL INKEY$ <> "": LOOP
  
    GOTO Hauptprg.VERLASSEN


' Versuch, Daten auf Drucker auszugeben, wiederholen.
'
NochmalVersuchen:
  
    COLOR TextFensterVordergrund, TextFensterHintergrund
    PRINT SPACE$(80)
  
    BEEP: BEEP
       
        PRINT "Bitte ÅberprÅfen Sie ihren Drucker!"
        PRINT ""
        PRINT "Wenn bereit, Eingabetaste drÅcken! Ansonsten";
        PRINT " ESC um Programm zu beenden."
    
    DO
        IF INKEY$ = CHR$(27) THEN GOTO Hauptprg.VERLASSEN
        IF INKEY$ = CHR$(13) THEN RESUME Drucken.AUSFUEHREN
    LOOP

'
SUB Default (f)

   SELECT CASE f
        
         CASE 1
          TitelLeisteVordergrund = 11
          TitelLeisteHintergrund = 4
          TitelLeisteHervorgehob = 14
          GOTO default.ENDE
   
         CASE 2
          TextFensterVordergrund = 7
          TextFensterHintergrund = 1
          GOTO default.ENDE
   
         CASE 3
          TeilErgebnisseVordergr = 15
          TeilErgebnisseHintergr = 1
          GOTO default.ENDE

         CASE 4
          GesamtDurschnittVorder = 14
          GesamtDurschnittHinter = 1
          GOTO default.ENDE
 
   END SELECT

default.ENDE:

END SUB

'
SUB FindNOTE (pnkt)

 'p = INT(pnkt)

 IF INT(pnkt) = 15 THEN Zensur = "1+"
 IF INT(pnkt) = 14 THEN Zensur = "1 "
 IF INT(pnkt) = 13 THEN Zensur = "1-"
 IF INT(pnkt) = 12 THEN Zensur = "2+"
 IF INT(pnkt) = 11 THEN Zensur = "2 "
 IF INT(pnkt) = 10 THEN Zensur = "2-"
 IF INT(pnkt) = 9 THEN Zensur = "3+"
 IF INT(pnkt) = 8 THEN Zensur = "3 "
 IF INT(pnkt) = 7 THEN Zensur = "3-"
 IF INT(pnkt) = 6 THEN Zensur = "4+"
 IF INT(pnkt) = 5 THEN Zensur = "4 "
 IF INT(pnkt) = 4 THEN Zensur = "4-"
 IF INT(pnkt) = 3 THEN Zensur = "5+"
 IF INT(pnkt) = 2 THEN Zensur = "5 "
 IF INT(pnkt) = 1 THEN Zensur = "5-"
 IF INT(pnkt) = 0 THEN Zensur = "6 "


END SUB

'
SUB FindPoint (Zensur AS STRING)

    z = VAL(LEFT$(Zensur, 1))
    f = VAL((RIGHT$(Zensur, 1) + "1"))

    IF RIGHT$(Zensur, 1) = " " THEN f = 0

    Punkte = 14 - ((z - 1) * 3) + f

END SUB

'
FUNCTION GetString$ (row, col, start$, end$, Vis, bereich)
    Max = Vis
    
     filling$ = " "
    
     von$ = " "
     bis$ = "}": z1$ = "Ñ": z2$ = "î": z3$ = "Å"
   
    IF row = 0 THEN row = CSRLIN - 1
      aktZeile = row

    IF bereich = 1 THEN
     von$ = "0"
     bis$ = "9": z1$ = "+": z2$ = "-": z3$ = "9"
    END IF
   
    curr$ = Trim$(LEFT$(start$, Max))
   
    IF curr$ = CHR$(8) THEN curr$ = ""
   
    LOCATE , , 1

    finished = FALSE
    DO
        GOSUB GetStringShowText
        GOSUB GetStringGetKey

        IF LEN(Kbd$) > 1 THEN
            finished = TRUE
            GetString$ = Kbd$
        ELSE
            SELECT CASE Kbd$
                CASE CHR$(0) + ";"
GetHelp:            LOCATE , , 0
                    CALL Hilfe
               
                CASE CHR$(13), CHR$(9)
                    finished = TRUE
                    GetString$ = Kbd$

                CASE CHR$(27)
                    curr$ = ""
                CASE CHR$(8)
                    IF curr$ <> "" THEN
                        curr$ = LEFT$(curr$, LEN(curr$) - 1)
                    END IF

                CASE von$ TO bis$, z1$, z2$, z3$, z4$
                    IF LEN(curr$) < Max THEN
                        curr$ = curr$ + Kbd$
                    ELSE
                        BEEP
                    END IF

                CASE ELSE
                    BEEP
            END SELECT
        END IF

    LOOP UNTIL finished


    end$ = curr$
    LOCATE , , 0
    EXIT FUNCTION
  

GetStringShowText:
    IF row = 0 THEN row = CSRLIN - 1
    LOCATE row, col
    IF LEN(curr$) > Vis THEN
        PRINT RIGHT$(curr$, Vis);
    ELSE
        PRINT curr$; STRING$(Vis - LEN(curr$), filling$);
        LOCATE row, col + LEN(curr$)
    END IF
    RETURN

GetStringGetKey:
    Kbd$ = ""
    WHILE Kbd$ = ""
        Kbd$ = INKEY$
        IF Kbd$ = CHR$(0) + ";" THEN GOTO GetHelp
    WEND
    RETURN

END FUNCTION

'
SUB Hilfe
  
' *** Bildschirminhalt abspeichern ***

   hilfezeile = CSRLIN

  FOR zeile = 1 TO CSRLIN
   FOR spalte = 1 TO 50
       scrimage$(zeile, spalte) = CHR$(SCREEN(zeile, spalte))
       scrimagestr$(zeile) = scrimagestr$(zeile) + scrimage$(zeile, spalte)
    NEXT spalte
  NEXT zeile

' *** Bildschirmmaske aufbauen ***

   Titel$ = " Hilfebildschirm           Weiter: beliebige Taste"
 
   COLOR TextFensterVordergrund, TextFensterHintergrund
     CLS
   COLOR TitelLeisteVordergrund, TitelLeisteHintergrund
     LOCATE 1, 1:  PRINT SPACE$(80)
     LOCATE 1, 1:  PRINT Titel$
   COLOR TitelLeisteHervorgehob
     LOCATE 1, 63: PRINT " EASY Mark2Point "

' *** Hilfetext zeigen ***

   COLOR TeilErgebnisseVordergr, TextFensterHintergrund
     PRINT
     PRINT " Tasten: "
   COLOR TextFensterVordergrund
     PRINT
     PRINT "     <Enter>  - Schlie·t die Eingabe ab. Wenn nichts eingegeben wurde,"
     PRINT "                wird die Abfrage Åbersprungen, bei Frage nach Noten"
     PRINT "                setzt das Programm automatisch 0 als Note ein."
     PRINT "     <ESC>    - Lîscht, sofern Sie etwas eingegeben und noch nicht"
     PRINT "                Enter gedrÅckt haben, alle eingegebenen Zeichen."
     PRINT "     <F1>     - Ruft diesen Hilfebildschirm auf."
     PRINT "     <J, N>   - Diese Tasten dÅrfen nur in Fragen verwendet werden,"
     PRINT "                die mit '(J/N)?' enden."
     PRINT ""
   COLOR TeilErgebnisseVordergr, TextFensterHintergrund
     PRINT " Hinweise:"
   COLOR TextFensterVordergrund
     PRINT
     PRINT "     Zensuren werden mit Plus und Minus HINTER der Ziffer eingegeben."
     PRINT "     Die Daten kînnen ausgedruckt werden. Dieses Shareware-Programm "
     PRINT "     wurde geschrieben von: Christian DÑhn (C) Rehna, Dezember 1995."
     PRINT ""
     PRINT "     Die Farben dieses Programms kînnen am Anfang der Datei ZENSUREN.BAS"
     PRINT "     geÑndert werden. Die Stelle im Programm ist Kommentiert."
     PRINT ""
   COLOR TeilErgebnisseVordergr, TextFensterHintergrund
     PRINT " Beliebige Taste drÅcken..."
     DO UNTIL INKEY$ <> "": LOOP
 
   COLOR TextFensterVordergrund, TextFensterHintergrund
     VIEW PRINT 1 TO 25
     CLS

' *** Bildschirminhalt wiederherstellen ***
     LOCATE 1, 1, 0
 
  FOR zeile = 1 TO hilfezeile
    LOCATE zeile, 1: PRINT scrimagestr$(zeile)
  NEXT zeile
 
  FOR zeile = 1 TO hilfezeile
    scrimagestr$(zeile) = ""
  NEXT zeile
    
  LOCATE , , 1

END SUB

'
FUNCTION Trim$ (x$)

    IF x$ = "" THEN
        Trim$ = ""
    ELSE
        lastChar = 0
        FOR a = 1 TO LEN(x$)
            y$ = MID$(x$, a, 1)
            IF y$ <> CHR$(0) AND y$ <> " " THEN
                lastChar = a
            END IF
        NEXT a
        Trim$ = LEFT$(x$, lastChar)
    END IF

END FUNCTION

