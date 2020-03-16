//tworzone z pomoca https://learn.adafruit.com/adafruit-gfx-graphics-library?view=all
//Dziekuje tworcom za stworzenie bibliotek oraz poradnika

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);  //ustalenie pinow
                                                             // pin 7 - Serial clock out (SCLK)
                                                             // pin 6 - Serial data out (DIN)
                                                             // pin 5 - Data/Command select (D/C)
                                                             // pin 4 - LCD chip select (CS)
                                                             // pin 3 - LCD reset (RST)



int kb1 = A1;            //zmienne
int kb2 = A2;            //odpowiedzialne
int kb3 = A3;            //za
int kb4 = A4;            //siwtche
                         //
int v1, v2, v3, v4;      //

int z = 0;  //zmienna pomocnicza do a[1] i b[1]

int a1 = 1;  //zmienna pomocnicza do sprawdzanie()
int b1 = 1;  //zmienna pomocnicza do sprawdzanie()
int a[220];  //zmienna do funkcji rysowanie()
int b[220];  //zmienna do funkcji rysowanie() 
int popa[220];  //zmienna do funkcji rysowanie() 
int popb[220];  //zmienna do funkcji rysowanie() 

int s = 4;  //wielkosc jednego segmentu weza //mozna ja zmieniac dowlonie, poniewaz wszystkie zmienne sa od niej zalezne

int dlu = 3;  //dlugosc weza
int kier[220];  //zmienna pomocnicza do funkcji loop() i rysowanie() (kierunek)
int popkier[220];  //zmienna pomocnicza do funkcji loop() i rysowanie() (poprzedni kierunek)
int kierw = 0;  //zmienna uzywana do ustalania braku kierunku
int losa;  //zmienna x do losowego miejsca jedzenia
int losb;  //zmienna y do losowego miejsca jedzenia
boolean wylos = 0;  //zmienna okreslajaca czy wylosowano miejsce czy nie
boolean dead = 0;  //zmienna okreslajaca czy waz umarl





void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);    //uruchamianie
  digitalWrite(9, HIGH); //diody LED
  display.begin();        //wlaczanie ekranu
  display.setContrast(55);  //ustawianie kontrastu (55)
  display.clearDisplay();
  display.drawRect(0, 0, 84 - (84 % (s+1)) - (s+1-2)+s, 48 - (48 % (s+1)) - (s-2+1)+s, BLACK);  //rysowanie obramowki planszy //check
  randomSeed(analogRead(2));
  a[0] = 1 + (84 - 84%(s+1))/(s+1)/2*(s+1);
  b[0] = 1 + (48 - 48%(s+1))/(s+1)/2*(s+1);  //idealnie policzone wzory na miejsce rozpoczecia
  popa[0] = 1;            //ustalanie poprzednich
  popb[0] = 1;            //zmiennych a[0] i b[0] by segemnty weza mogly z nich skorzystac 
  for(int i = 0; i < 220; i++)  //ustawianie wszystkich segmentow na jednym polu za mapa
  {
    if(i==0);
    else
    {
    a[i] = -s-1;
    b[i] = -s-1;
    }
  }
  
}
int sprawdzaniekierw()
{
  v1 = analogRead(kb1);  //ustalenie wartosci zmiennych odpowiedzialnych za
  v2 = analogRead(kb2);  //wykrywanie przyciskow
  v3 = analogRead(kb3);  //16 przyciskow wykrywanych jest przez 4 piny, ktore rozrozniaja
  v4 = analogRead(kb4);  //rozne napiecia
  
  if (v4 > 300 && v4 < 550 && kierw != 3)    //gora 1  //switch 2 odpowiedzialny za ruch w gore
  {
    kierw = 1;
    
  }
  if (v3 > 100 && v3 < 300 && kierw != 4)    //lewo 2  //switch 5 odpowiedzialny za ruch w lewo
  {
    kierw = 2;
    
  }
  if (v3 > 300 && v3 < 550 && kierw != 1)    //dol 3  //switch 6 odpowiedzialny za ruch w dol
  {
    kierw = 3;
    
  }
  if (v3 > 550 && v3 < 800 && kierw != 2)    //prawo 4  //switch 7 odpowiedzialny za ruch w prawo
  {
    kierw = 4;
    
  }



  return kierw;  //zwraca kierunek
}

void losowanie()
{

   losa = ((((84-(84%(s+1))-(s-2+1)-2)/(s+1)-1)-random(0, (84-(84%(s+1))-(s+1-2)-2)/(s+1))))*(s+1)+1;   //wzor na polozenie x jedzenia
   losb = ((((48-(48%(s+1))-(s-2+1)-2)/(s+1)-1)-random(0, (48-(48%(s+1))-(s-2+1)-2)/(s+1))))*(s+1)+1;   //wzor na polozenie y jedzenia 
 wylos=1;  //okreslenie, ze liczba zostala wylosowana (by nie losowalo w kolko)
 dlu++;  //zwiekszenie dlugosci weza (poniewaz losowanie odbywa sie zawsze po zjedzeniu kulki)
 for(int i = 0; i<=dlu; i++)  //sprawdzanie czy kulka nie pojawi sie w wezu i losowanie nowego miejsca, gdy tak jest
 {
   if((losa==a[i] && losb==b[i]) || (losa==a[i]+1 && losb==b[i]+1) || (losa==a[i]-1 && losb==b[i]-1) || (losa==a[i]+1 && losb==b[i]-1) || (losa==a[i]-1 && losb==b[i]+1) || (losa==a[i] && losb==b[i]+1) || (losa==a[i] && losb==b[i]-1) || (losa==a[i]+1 && losb==b[i]) || (losa==a[i]-1 && losb==b[i]))
   {//warunek jest bardzo dlugi, poniewaz polozenie punktow wzgledem siebie moze byc rozne
   losa = ((((84-(84%(s+1))-(s-2+1)-2)/(s+1)-1)-random(0, (84-(84%(s+1))-(s+1-2)-2)/(s+1))))*(s+1)+1;
   losb = ((((48-(48%(s+1))-(s-2+1)-2)/(s+1)-1)-random(0, (48-(48%(s+1))-(s-2+1)-2)/(s+1))))*(s+1)+1;  
   }
 }
}

void przegrana()    //funkcja informujaca o przegranej
{

  for(int i=0; i<3; i++)  //trzykrotne mrugniecie
  {
  rysowanie();
  delay(500);
  display.clearDisplay();
  display.drawRect(0, 0, 84 - (84 % (s+1)) - (s+1-2)+s, 48 - (48 % (s+1)) - (s-2+1)+s, BLACK);
  display.display();
  delay(500);
  }
  
  a[0] = 1 + (84 - 84%(s+1))/(s+1)/2*(s+1);  //ustawianie pierwszego segmentu na odpowiednich wspolrzednych
  b[0] = 1 + (48 - 48%(s+1))/(s+1)/2*(s+1);  
    for(int i = 0; i < 220; i++)  //ustawianie wszystkich segmentow na jednym polu
  {
    if(i==0);
    else
    {
    a[i] = -s-1;
    b[i] = -s-1;
    }
  }
  kierw = 0;    //zerowanie zmiennych
  kier[0] = 0;  //
  dlu = 1;      //
  losowanie();  //ponowne losowanie miejsca polozenia kulki

}

void sprawdzanie()  //funkcja sprawdzajaca czy nie wyjechano poza plansze lub czy nie uderzono w siebie
{
    if (a[0] < a1)  //lewa sciana
    {
    dead=1;
    przegrana();

    }
  if (a[0] >= (84 - (84 % s) - (s-2)) - s) //prawa sciana
    {
    dead=1;
    przegrana();

    }
  if (b[0] < b1)  //gorna sciana
   {
    dead=1;
    przegrana();

   }
  if (b[0] >= (48 - (48 % s) - (s-2)) - s)  //dolna sciana
    {
    dead=1;
    przegrana();

    }
  for(int i=2; i<220; i++)  //sprawdzanie czy nie wjechano samemu w siebie
 {
  if((a[0]==a[i]+1 && b[0]==b[i]+1) || (a[0]==a[i]-1 && b[0]==b[i]-1) || (a[0]==a[i]+1 && b[0]==b[i]-1) || (a[0]==a[i]-1 && b[0]==b[i]+1) || (a[0]==a[i] && b[0]==b[i]+1) || (a[0]==a[i] && b[0]==b[i]-1) || (a[0]==a[i]+1 && b[0]==b[i]) || (a[0]==a[i]-1 && b[0]==b[i]) || (a[0]==a[i] && b[0]==b[i]))
  if(i<dlu)
  przegrana();

 } 
}
void rysowanie()  //funkcja odpowiedzialna za rysowanie segmentow
{
  
  

  display.fillRect(losa, losb, s, s , BLACK);
  display.drawPixel(losa, losb, WHITE);
  display.drawPixel(losa+s-1, losb, WHITE);
  display.drawPixel(losa, losb+s-1, WHITE);
  display.drawPixel(losa+s-1, losb+s-1, WHITE);



    for (int i = 0; i <= dlu; i++)  //rysowanie tylu segmentow zaleznie jaka jest wartosc dlu (dlugosc weza)
    {
      if (i == 0)  //rysowanie segmentu glownego (glowy)
  {
  popa[i] = a[i];
  popb[i] = b[i];  
  kier[0] = kierw;
    //tutaj skonczylem ----------------------------____________________________________________________________chce zmienic to nizej na tutaj by oszczedzic sobie nerwoow
//    display.fillRect(a[0], b[0], s, s, BLACK);
      if(kierw == 1)//gora
  display.fillRect(a[i], b[i]+s, s, -s-1, BLACK);
      if(kierw == 3)  //dol
  display.fillRect(a[i], b[i], s, s+1, BLACK);
      if(kierw == 2)//lewo
  display.fillRect(a[i]+s, b[i], -s-1, s, BLACK);
      if(kierw == 4)  //prawo
  display.fillRect(a[i], b[i], s+1, s, BLACK);


  }

   
if(i > 0)  //reszta segmentow
{
  popa[i] = a[i];
  popb[i] = b[i];  
  a[i] = popa[i-1];
  b[i] = popb[i-1];

//      if(a[i] == a[i-1] && b[i] >b[i-1])//gora
//  display.fillRect(a[i], b[i]-s-1, s, 2*s+1, BLACK);
//      if(a[i] == a[i-1] && b[i] < b[i-1])  //dol
//  display.fillRect(a[i], b[i], s, s+1, BLACK);
//      if(a[i] > a[i-1] && b[i] == b[i-1])//lewo
//  display.fillRect(a[i]-s-1, b[i], 2*s+1, s, BLACK);
//      if(a[i] < a[i-1] && b[i] == b[i-1])  //prawo
//  display.fillRect(a[i], b[i], s+1, s, BLACK);

}
if(i==dlu && dlu != 0)
{
  a[dlu] = popa[dlu-1];
  b[dlu] = popb[dlu-1];
  if(kier[i-1] == 1)
  display.fillRect(a[i], b[i]+s, s, -s-1, WHITE);
    if(kier[i-1] == 3)
  display.fillRect(a[i], b[i], s, s+1, WHITE);
    if(kier[i-1] == 2)
  display.fillRect(a[i]+s, b[i], -s-1, s, WHITE);
      if(kier[i-1] == 4)
  display.fillRect(a[i], b[i], s+1, s, WHITE);
    kier[i] = kier[i-1];
}


      
    //1 gora, 2 lewo, 3 dol‚, 4 prawo

  }
    


  display.display();  //wyswietlenie wszystkiego


}

void loop()
{

  if(kierw==0)  //jezeli nie ma ustalonego kierunku to waz ma miec takie wspolrzedne polozenia
  {
  a[0] = 1 + (84 - 84%(s+1))/(s+1)/2*(s+1);  
  b[0] = 1 + (48 - 48%(s+1))/(s+1)/2*(s+1);  
  popkier[0] = 0;
  }
  if(wylos == 1 && a[0] == losa && b[0] == losb)  //jezeli miejsce polozenia jedzenia jest wylosowane, a waz jest na tej samej pozycji
  wylos = 0;                                      //to zmienna ma okreslic, ze nalezy wylosowac nastepne miejsce
  if(wylos == 0)                                  //
  losowanie();                                    //
  kierw = sprawdzaniekierw();                     //ustalanie kierunku (ktory przycisk zostal wcisniety
  rysowanie();  //rysowanie weza, gdy jest w bezruchu

  if (kierw == 1)  //ruch w gore
  { //gora
    if (popkier[0] == 3);  //zablokowanie ruchu w niepoprawna strone
    else
    {

      while (kierw == 1)  //podtrzymywanie ruchu przy spelnionym warunku kierunku
      {

          kierw = sprawdzaniekierw();          //funkcja zmieniajaca lub podtrzymujaca kierunek ruchu
          b[0] = b[0] - s - 1;           //jest tu takze wczesniej wspomniana zmienna z odpowiedzialna za poprawne rysowanie segmentow od 3 do 220
          sprawdzanie();              //sprawdzanie czy nie uderzono w rog planszy lub w siebie
          rysowanie();                //rysowanie weza w ruchu
          delay(s*s*s*s*s);
          if(wylos == 1 && a[0] == losa && b[0] == losb)
          wylos = 0;
          if(wylos == 0)
          losowanie();
          popkier[0] = 1;
      }
    }
  }

  if (kierw == 2)  //ruch w lewo
  { //lewo
    if (popkier[0] == 4);
    else
    {

      while (kierw == 2)
      {

          kierw = sprawdzaniekierw();
          a[0] = a[0] - s - 1;
          sprawdzanie();
          rysowanie();
          delay(s*s*s*s*s);
          if(wylos == 1 && a[0] == losa && b[0] == losb)
          wylos = 0;
          if(wylos == 0)
          losowanie();
          popkier[0] = 2;
      }
    }
  }

  if (kierw == 3) {  //ruch w dol
    //dol‚
    if (popkier[0] == 1);
    else
    {

      while (kierw == 3)
      {


          kierw = sprawdzaniekierw();
          b[0] = b[0] + s + 1;
          sprawdzanie();
          rysowanie();
          delay(s*s*s*s*s);
          if(wylos == 1 && a[0] == losa && b[0] == losb)
          wylos = 0;
          if(wylos == 0)
          losowanie();
          popkier[0] = 3;
      }
    }
  }

  if (kierw == 4) {  //ruch w prawo
    //prawo
    if (popkier[0] == 2);
    else
    {

      while (kierw == 4)
      {

          kierw = sprawdzaniekierw();
          a[0] = a[0] + s + 1;
          sprawdzanie();
          rysowanie();
          delay(s*s*s*s*s);
          if(wylos == 1 && a[0] == losa && b[0] == losb)
          wylos = 0;
          if(wylos == 0)
          losowanie();
          popkier[0] = 4;


      }
    }
  }
  }




