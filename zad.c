
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define TRUE 1
#define FALSE 0

typedef struct LetterVar{
  int x;
  int y;
  int w;
  int h;
};
struct LetterVar bigK = {150, 50, 40, 120}, bigN = {100, 30, 40, 120}, smallK = {180, 140, 20, 40}, smallN = {180, 140, 10, 40};
struct LetterVar speedK = {-3, -9, 2, 8}, speedN = {-8, -11, 3, 8};
int red, green, blue, yellow,white,black;
unsigned long foreground, background;

void _add(struct LetterVar *f, struct LetterVar s) {
  f->x += s.x;
  f->y += s.y;
  f->w += s.w;
  f->h += s.h;
}
void _sub(struct LetterVar *f, struct LetterVar s) {
  f->x -= s.x;
  f->y -= s.y;
  f->w -= s.w;
  f->h -= s.h;
}

void TransformLetter(int *order, int *isTransforming, struct LetterVar *K, struct LetterVar *N) {
  //if (*isTransforming)
  //  return;

  if(*order) {
      _sub(K, speedK);
      _add(N, speedN);
      if (K->w <= smallK.w)
        *order = FALSE;
    } else {
      _add(K, speedK);
      _sub(N, speedN);
      if (K->w >= bigK.w)
        *order = TRUE;
    }
  
}

int Tarcza(Display* display, Window window, GC gc, unsigned long color) {
  int x = 100, y = 25, width = 200, height = 125;
  XSetForeground(display,gc,red); //zmiana koloru
  XPoint shieldPoints[] = {
    // Pierwszy prostokąt
    {x,y},
    {x+width, y},
    {x+width, y+ height},
    {x, y+height}, 

    // arki
    {x,y*4},
    
    // Drugi prostokąt 
    {x+width/4,y*2},
    {x+width/4 + width/2, y*2},
    {x+width/4 + width/2, y*2+height*1.4},
    {x+width/4, y*2+height*1.4},

    // arki dopełniające
    {x*2,y*7.20},
    {0,y*7.20}
  };
  XFillPolygon(display,window,gc,shieldPoints, 4 ,Complex, CoordModeOrigin);

  XFillArc(display,window,gc,shieldPoints[3].x,shieldPoints[3].y - height*0.4,width,height*0.75,(180)*64,(180)*64);
  //XFillArc(display,window,gc,shieldPoints[4].x,shieldPoints[4].y,width,height*0.75,(0+deg)*64,(-180)*64);
  
  XFillPolygon(display,window,gc,shieldPoints+5, 4 ,Complex, CoordModeOrigin);
  
  XSetForeground(display,gc,white);
  XFillArc(display,window,gc,shieldPoints[9].x,shieldPoints[9].y,width,height*0.75,180*64,-60*64);
  XFillArc(display,window,gc,shieldPoints[10].x,shieldPoints[10].y,width,height*0.75,0*64,60*64);


 // XSetForeground(display,gc,black);

 // XDrawArc(display,window,gc,x,y*4,width,height*0.75,180*64,60*64);
 // XDrawArc(display,window,gc,x,y*4,width,height*0.75,0*64,-60*64);
 // XDrawArc(display,window,gc,x*2,y*7.20,width,height*0.75,180*64,-60*64);
 //  XDrawArc(display,window,gc,0,y*7.20,width,height*0.75,0*64,60*64);


}

int DrawLetter(Display* display, Window window, GC gc,XPoint* points, unsigned int n, unsigned long color, unsigned long frameColor, int x, int y, unsigned int width, unsigned int height) {
  XSetForeground(display,gc,color);
  XFillPolygon(display,window,gc,points,n, Complex, CoordModeOrigin);
  XSetForeground(display,gc,frameColor);

  XSegment segments[n];
  for(int i = 0; i<n-1; i++) {
    segments[i].x1 = points[i].x;
    segments[i].y1 = points[i].y;
    segments[i].x2 = points[i+1].x;
    segments[i].y2 = points[i+1].y;
  }
  segments[n-1].x1 = points[n-1].x;
  segments[n-1].y1 = points[n-1].y;
  segments[n-1].x2 = points[0].x;
  segments[n-1].y2 = points[0].y;

  XDrawSegments(display,window,gc,segments,n);
}

int DrawK(Display* display, Window window, GC gc, unsigned long color, unsigned long frameColor, int x, int y, unsigned int width, unsigned int height) {
  XPoint Kpoints[] = {
    {x,y},
    {x+width, y},
    {x+width, y+height/3},
    {x+width*2, y},
    {x+width*3, y},
    {x+width*2.5, y+height/2},
    {x+width*3, y+height},
    {x+width*2, y+height},
    {x+width, y+height*2/3},
    {x+width, y+height},
    {x, y+height},
  };
  DrawLetter(display,window,gc,Kpoints,11,color,frameColor,x,y,width,height);
}

int DrawN(Display* display, Window window, GC gc, unsigned long color, unsigned long frameColor, int x, int y, unsigned int width, unsigned int height) {
  XPoint Npoints[] = {
      {x,y},
      {x+width,y},
      {x+4*width, y+height-width},
      {x+4*width, y},
      {x+5*width, y},
      {x+5*width, y+height},
      {x+4*width, y+height},
      {x+width, y+width},
      {x+width, y+height},
      {x,y+height},
    };
  DrawLetter(display,window,gc,Npoints,10,color,frameColor,x,y,width,height);
}

int Inicjaly(Display* display, Window window, GC gc, int letterOrder, struct LetterVar Kvar, struct LetterVar Nvar) {
  if(letterOrder) {
    DrawK(display,window,gc,blue, yellow, Kvar.x,Kvar.y,Kvar.w,Kvar.h);
    DrawN(display,window,gc,yellow, blue, Nvar.x, Nvar.y, Nvar.w,Nvar.h);
  } else {
    DrawN(display,window,gc,yellow, blue, Nvar.x, Nvar.y, Nvar.w,Nvar.h);
    DrawK(display,window,gc,blue, yellow, Kvar.x,Kvar.y,Kvar.w,Kvar.h);
  }
}

//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
  white=AllocNamedColor("white", display, colormap);
  black=AllocNamedColor("black", display, colormap);
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 500;
  info.height = 300;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

  // określa która litera będzie ryzowana pierwsza
  int letterOrder;
  letterOrder = 1;
  struct LetterVar K = bigK, N = smallN;

  Bool isTransforming = TRUE;
 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
          
          Tarcza(display,window,gc,colormap);
          Inicjaly(display,window,gc,letterOrder, K,N);
          XFlush(display);
        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
          Tarcza(display,window,gc,colormap);
          isTransforming = !isTransforming;
          TransformLetter(&letterOrder, &isTransforming, &K, &N);

          Inicjaly(display,window,gc,letterOrder, K,N);
          XFlush(display);
        }
        break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
