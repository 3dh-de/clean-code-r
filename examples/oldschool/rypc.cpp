// DiskTools Shell - a shell for some good disk-tools under DOS
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>

int maxEntries=5, ESC=27, key=0, pos=0;

struct {
 char *btn, *txt1, *txt2, *txt3;
} entry[5];


void initgrafx()
{
  int grDriver = DETECT, grMode, grError = grOk;
  initgraph(&grDriver,&grMode,"");
  grError = graphresult();
  if (grError != grOk)  {
    clrscr();
		printf ("Fatal Error: could't initialize Graphic Adaptor!");
    exit(1);
	}
  return;
}


void closegrafx()
{
	closegraph();
	textcolor(7); textbackground(0); clrscr;
}

void centertxt(int xp1, int y, int xp2, int color, char *txtstr)
{
	setcolor(color); outtextxy(xp1+0.5*(xp2-xp1-textwidth(txtstr)),y,txtstr);
	return;
}

void showtxt(int x, int y, int clr, char *txt)
{
	setcolor(clr); outtextxy(x,y,txt);
}

void box(int x1,int y1,int x2,int y2,int bk,int titbk,int titclr,char *txt)
{
	setfillstyle(1,bk);    bar(x1,y1,x2,y2);
	setcolor(0); rectangle(x1,y1,x2,y2);
	setfillstyle(1,titbk); bar(x1+1,y1+1,x2-1,y1+15);
	setfillstyle(9,8);     bar(x2+1,y1+7,x2+7,y2+7); bar(x1+7,y2+1,x2+7,y2+7);
	setfillstyle(1,8);     bar(x1+1,y1+1,x1+14,y1+14);
	rectangle(x1,y1,x1+15,y1+15);
	setfillstyle(1,14); bar(x1+4,y1+6,x1+11,y1+9); rectangle(x1+4,y1+6,x1+11,y1+9);
	line(x1,y1+15,x2,y1+15);
	centertxt(x1,y1+4,x2,titclr,txt);
}


void button(int x1, int y1, int x2, int y2, int txtclr, int bkclr, char *txt)
{
	if(textwidth(txt)>x2-x1) {
		closegrafx(); printf("button: txt to big!"); exit(0);
	}
	setcolor(7); rectangle(x1,y1,x2,y2);
	setfillstyle(1,bkclr); bar(x1+1,y1+1,x2-1,y2-1);
	setcolor(15); line(x1+1,y1+1,x2-1,y1+1); line(x1+1,y1+1,x1+1,y2-1);
	setcolor(0); line(x1+1,y2-1,x2-1,y2-1); line(x2-1,y1+1,x2-1,y2-1);
	int clr=0; if(txtclr==0) clr=7;
	centertxt(x1-1,y1+4,x2-1,clr,txt);
	centertxt(x1,y1+5,x2,txtclr,txt);
}

void movecur(int oldpos, int move)
{
	button(120,100+40*oldpos,240,116+40*oldpos,3,4,entry[oldpos].btn);
	showtxt(260,100+(oldpos*40),0,entry[oldpos].txt1);
	showtxt(260,110+(oldpos*40),0,entry[oldpos].txt2);
	showtxt(260,120+(oldpos*40),0,entry[oldpos].txt3);
	int newpos=pos+move;
	if(newpos<1) newpos=1;
	if(newpos>5) newpos=5;
	button(120,100+40*newpos,240,116+40*newpos,11,12,entry[newpos].btn);
	showtxt(260,100+(newpos*40),5,entry[newpos].txt1);
	showtxt(260,110+(newpos*40),5,entry[newpos].txt2);
	showtxt(260,120+(newpos*40),5,entry[newpos].txt3);
	pos=newpos;
}

void showdesk()
{
	setfillstyle(11,15); bar(0,0,43,465);
	setfillstyle(9,6); bar(44,0,639,465);
	setcolor(0); rectangle(0,0,639,465);
	setfillstyle(1,0); bar(400,20,609,77);
	setcolor(6); rectangle(400,20,609,77);
	setcolor(0); rectangle(401,21,608,76);
	showtxt(410,30,2,"Benutzer:"); showtxt(490,30,7,"Chris.D.");
	showtxt(410,44,2,"Computer:"); showtxt(490,44,7,"iP 200 MMX");
	showtxt(410,58,2,"OS.-Name:"); showtxt(490,58,7,"Windows 95a");
	settextstyle(0,1,3);
	showtxt(33,14,12,"Erste Hilfe fÅr PC");
	showtxt(34,15,12,"Erste Hilfe fÅr PC");
	showtxt(35,16,4,"Erste Hilfe fÅr PC");
	settextstyle(0,0,1);
	centertxt(-2,472,637,1,"F1 = Hilfe   ESC = Ende   Pfeiltasten = Bewegen");
	centertxt(-1,471,638,1,"F1 = Hilfe   ESC = Ende   Pfeiltasten = Bewegen");
	centertxt(0,470,639,9,"F1 = Hilfe   ESC = Ende   Pfeiltasten = Bewegen");
	box(100,100,500,350,15,1,14,"DiskTools Shell (c) Chris.D.");
	for(int i=1; i<=maxEntries; i++)
	{
		button(120,100+40*i,240,116+40*i,3,4,entry[i].btn);
	}
	for(i=0; i<=5; i++)
	{
		showtxt(260,100+(i*40),0,entry[i].txt1);
		showtxt(260,110+(i*40),0,entry[i].txt2);
		showtxt(260,120+(i*40),0,entry[i].txt3);
	}
}


int main()
{
	entry[1].btn="DiskCheck";
	entry[1].txt1="Untersucht Disketten im";
	entry[1].txt2="Laufwerk A: schnell nach";
	entry[1].txt3="Datenfehlern.";

	entry[2].btn="Systemdateien";
	entry[2].txt1="Ersetzt alle Systemdateien";
	entry[2].txt2="der Festplatte durch Kopien";
	entry[2].txt3="von dieser Diskette.";

	entry[3].btn="Scandisk";
	entry[3].txt1="Sucht und behebt Datenfehler";
	entry[3].txt2="auf der Festplatte.";
	entry[3].txt3="";

	entry[4].btn="WIN-NOTFALL";
	entry[4].txt1="Tauscht das defekte Windows";
	entry[4].txt2="gegen eine funktionsfÑhige";
	entry[4].txt3="Kopie von Windows aus.";

	entry[5].btn="Ende";
	entry[5].txt1="Beendet dieses kleine";
	entry[5].txt2="Programm.               ;-)";
	entry[5].txt3="";
	initgrafx();
	showdesk();
	movecur(1,1);
	while(key!=ESC) {
		key=getch();
		if(key==72) movecur(pos,-1);
		if(key==80) movecur(pos,1);
		if(key==13) if(pos==5) { closegrafx(); exit(0); }
		if(key==59) {
			box(50,40,560,440,3,9,14,"Kurz-Hilfe");
			showtxt(72,77,8,"BEDIENUNG");
			showtxt(70,75,10,"BEDIENUNG");
			showtxt(80,100,15,"Das Bewegen im MenÅ erfolgt mit Hilfe der Pfeiltasten.");
			showtxt(80,115,15,"Nach dem AuswÑhlen kann der gewÅnschte MenÅpunkt durch");
			showtxt(80,130,15,"BetÑtigen der Eingabetaste aufgerufen werden.");
			showtxt(80,145,15,"Sie kînnen dieses Programm jederzeit durch DrÅcken der");
			showtxt(80,160,15,"Abbruchtaste ESC verlassen.");
			showtxt(72,197,8,"HINWEISE");
			showtxt(70,195,14,"HINWEISE");
			showtxt(80,220,15,"Jeder MenÅpunkt wird im rechts daneben befindlichem Text");
			showtxt(80,235,15,"in kurzer Form erklÑrt.  ACHTUNG! Benutzen Sie diese");
			showtxt(80,250,15,"Rettungsdiskette nur an dem PC, fÅr den diese Diskette");
			showtxt(80,265,15,"erstellt wurde (siehe Infobox rechts oben)!!! Es kann");
			showtxt(80,280,15,"sonst beim 2. und 4. MenÅpunkt zu Datenverlusten fÅhren!");
			showtxt(72,307,8,"COPYRIGHT");
			showtxt(70,305,13,"COPYRIGHT");
			showtxt(80,330,15,"Dies ist FREEWARE, also kostenlose und uneingeschrÑnkte");
			showtxt(80,345,15,"Software! Autor ist ein PC-Hobbyist namens Christian DÑhn");
			showtxt(80,360,15,"aus Mecklenburg-Vorpommern. Es ist nicht gestattet, diese");
			showtxt(80,375,15,"Software unvollstÑndig oder mit verÑndertem Copyright");
			showtxt(80,390,15,"weiterzugeben. Jegliche vollstÑndige VervielfÑltigung ist");
			showtxt(80,405,15,"erwÅnscht! FÅr SchÑden Åbernimmt der Autor keine Haftung!");
			while (!kbhit() && (getch() != 27)) { };
			showdesk();
			movecur(pos,0);
		}
		if(key==13)
		{
			switch (pos) {
				case 1:
					closegrafx();
					system("dcheck");
					initgrafx(); showdesk(); movecur(pos,0);
					break;
				case 2:
					closegrafx();
					//system("a:\sys a: c:");
					//system("ren c:\autoexec.bat *.RET");
					//system("ren c:\config.sys *.RET");
					//system("copy a:\autoexec.bat c:\");
					//system("copy a:\config.sys c:\");
					system("edit c:\autoexec.bat");
					initgrafx(); showdesk(); movecur(pos,0);
					break;
				case 3:
					closegrafx();
					system("scandisk c:");
					initgrafx(); showdesk(); movecur(pos,0);
					break;
				case 4:
					closegrafx();
					system("command");
					initgrafx(); showdesk(); movecur(pos,0);
					break;
		}
	}
	closegrafx();
	return 0;
}

