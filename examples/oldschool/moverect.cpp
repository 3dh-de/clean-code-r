/* This is a little try-out in graphics */
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <dos.h>

int alpha=150, ESC=27, F1=59, key=0;
float px,py;


void initgrafx()
{
  int grDriver = DETECT, grMode, grError = grOk;
  initgraph(&grDriver,&grMode,"");
  grError = graphresult();
  if (grError != grOk)
  {
    clrscr();
    printf ("Fatal Error: could't initialize Graphic Adaptor!");
    exit(1);
  }
  clearviewport();
  return;
}


void closegrafx()
{
  closegraph();  textcolor(7); textbackground(0); clrscr;
}


void centertxt(int xp1, int y, int xp2, int color, char *txtstr)
{
  setcolor(color); outtextxy(xp1+0.5*(xp2-xp1-textwidth(txtstr)),y,txtstr);
  return;
}


void box(int x1,int y1,int x2,int y2,int bk,int titbk,int titclr,char *txt)
{
  setfillstyle(1,bk);    bar(x1,y1,x2,y2);
  setcolor(0); rectangle(x1,y1,x2,y2);
  setfillstyle(1,titbk); bar(x1+1,y1+1,x2-1,y1+15);
  setfillstyle(9,8);     bar(x2+1,y1+7,x2+7,y2+7); bar(x1+7,y2+1,x2+7,y2+7);
  setfillstyle(1,7);     bar(x1+1,y1+1,x1+14,y1+14);
  rectangle(x1,y1,x1+15,y1+15); rectangle(x1+4,y1+6,x1+11,y1+9);
  line(x1,y1+15,x2,y1+15);
  centertxt(x1,y1+4,x2,titclr,txt);
}

void button(int x1, int y1, int x2, int y2, int txtclr, char *txt)
{
  if(textwidth(txt)>x2-x1) {
    closegrafx(); printf("button: txt to big!"); exit(0);
  }
  setcolor(8); rectangle(x1,y1,x2,y2);
  setfillstyle(1,7); bar(x1+1,y1+1,x2-1,y2-1);
  setcolor(15); line(x1+1,y1+1,x2-1,y1+1); line(x1+1,y1+1,x1+1,y2-1);
  setcolor(0); line(x1+1,y2-1,x2-1,y2-1); line(x2-1,y1+1,x2-1,y2-1);
  int clr=0; if(txtclr==0) clr=7;
  centertxt(x1-1,y1+4,x2-1,clr,txt);
  centertxt(x1,y1+5,x2,txtclr,txt);
}


func (int angle, int inc)
{
  if (angle<=0) angle=360;
  angle+=inc;
  px=cos(3.145*angle /180);   /* Formula: x=cos(Piùalpha/180)ùradius */
  py=sin(3.145*angle /180);   /* Formula: y=sin(Piùalpha/180)ùradius */
  return angle;
}

void showrect(int midx, int midy, int r1, int r2, int c)
{
  setcolor(c);  /* Movin' Rectangle */
  line(midx-px*r1,midy-py*r2,midx+px*r1,midy-py*r2);
  line(midx-px*r1,midy-py*r2,midx-px*r1,midy+py*r2);
  line(midx+px*r1,midy-py*r2,midx+px*r1,midy+py*r2);
  line(midx+px*r1,midy+py*r2,midx-px*r1,midy+py*r2);
}

void showrect2(int midx, int midy, int r1, int r2, int c)
{
  setcolor(c);  /* Movin' Rectangle */
  line(midx-px*r1,midy-r2,midx+px*r1,midy-r2);
  line(midx-px*r1,midy-r2,midx-px*r1,midy+r2);
  line(midx+px*r1,midy-r2,midx+px*r1,midy+r2);
  line(midx+px*r1,midy+r2,midx-px*r1,midy+r2);
}

void showbar(int midx, int midy, int r1, int r2, int c)
{
  setcolor(c);  /* Movin' Rectangle */
  setfillstyle(1,c);
  line(midx-px*r1,midy-py*r2,midx+px*r1,midy-py*r2);
  line(midx-px*r1,midy-py*r2,midx-px*r1,midy+py*r2);
  line(midx+px*r1,midy-py*r2,midx+px*r1,midy+py*r2);
  line(midx+px*r1,midy+py*r2,midx-px*r1,midy+py*r2);
  floodfill(midx,midy,c);
}

int main(void)
{
  initgrafx();
//  setactivepage(1);
  clearviewport();
  setfillstyle(8,1); bar(0,0,639,479);
  box(90,5,549,469,1,4,14,"Graphikdemo (c) Chris.D.");
  button(110,440,190,456,5,"F1 Info");
  button(440,440,530,456,14,"ESC Ende");
//  setactivepage(0);
  clearviewport;
  setfillstyle(8,1); bar(0,0,639,479);
  box(90,5,549,469,1,4,14,"Graphikdemo (c) Chris.D.");
  button(110,440,190,456,5,"F1 Info");
  button(440,440,530,456,14,"ESC Ende");
  while (key!=ESC)
  {
    if(kbhit()) {
      key=getch();
      if(key==F1) {
	box(200,200,500,400,15,1,15,"Info");
	centertxt(199,234,499,7,"Winkelfunktionen in C++");
	centertxt(200,235,500,12,"Winkelfunktionen in C++");
	centertxt(200,260,500,1,"..oder wie man mit");
	centertxt(200,274,500,1,"mathematischen Funktionen komplexe");
	centertxt(200,288,500,1,"Grafiken programmieren kann.");
	centertxt(200,310,500,1,"Demo einiger M”glichkeiten der");
	centertxt(200,324,500,1,"Erzeugung von 2D und 3D Objekten.");
	centertxt(200,346,500,1,"Programm und Quelltext frei");
	centertxt(200,360,500,1,"zu kopieren und zu „ndern.");
	getch();
	box(90,5,549,469,1,4,14,"Graphikdemo (c) Chris.D.");
	button(110,440,190,456,5,"F1 Info");
	button(440,440,530,456,14,"ESC Ende");
      }
    }
    alpha = func(alpha,1);
//    showrect(320,240,100,100,10);
//    setactivepage(1);
//    showbar(320,240,100,100,10);
//    setactivepage(0);
//    showbar(320,240,100,100,11);

    showrect2(320,240,100,100,10);
    showrect2(420,240,100,100,14);
//    setvisualpage(1);
    delay(10);
    showrect2(320,240,100,100,1);
    showrect2(420,240,100,100,1);
//    setvisualpage(0);
  }
  closegrafx();
  return 0;
}

