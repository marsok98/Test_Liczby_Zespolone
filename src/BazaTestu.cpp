#include <iostream>
#include <cstring>
#include <cassert>
#include <fstream>
#include "BazaTestu.hh"


using namespace std;

/*
 * Tablica, ktora jest widoczna tylko w tym module.
 * Zawiera ona tresc latwego testu.
 */
static WyrazenieZesp  TestLatwy[] =
  { {{2,1}, Op_Dodaj, {1,2}},
    {{1,0}, Op_Odejmij, {0,1}},
    {{3,0}, Op_Mnoz, {0,3}},
    {{4,8}, Op_Dziel, {1,0}},
  };


static WyrazenieZesp TestTrudny[] = 
  { {{2.5,9.5},     Op_Dodaj,   {2.5,7.5}},
    {{8.5,10.5},     Op_Odejmij, {7.5,11.5}},
    {{6.5,45.5},  Op_Mnoz,    {25.5,50}},
    {{25.50,1234}, Op_Dziel,   {25.50,1234}},
  };




/*
 * W bazie testu ustawia wybrany test jako biezacy test i indeks pytania
 * ustawia na pierwsze z nich.
 * Parametry:
 *    wskBazaTestu - wskaznik na zmienna reprezentujaca baze testu,
 *    wskTabTestu  - wskaznik na tablice zawierajaca wyrazenia arytmetyczne
 *                   bedace przedmiotem testu,
 *    IloscElemTab - ilosc pytan w tablicy.
 *   
 * Warunki wstepne:
 *      - Parametr wskBazaTestu nie moze byc pustym wskaznikiem. Musi zawierac adres
 *        zmiennej reprezentujacej baze testu, ktora wczesniej zostal poprawnie
 *        zainicjalizowany poprzez wywolanie funkcji InicjalizujTest.
 *      - Parametr wskTabTestu zawiera wskaznik na istniejaca tablice.
 *      - Parametr IloscPytan zawiera wartosc, ktora nie przekracza ilosci elementow
 *        w tablicy dostepnej poprzez wskTabTestu.
 */
void UstawTest( BazaTestu *wskBazaTestu, WyrazenieZesp *wskTabTestu, unsigned int IloscPytan )
{
  wskBazaTestu->wskTabTestu = wskTabTestu;
  wskBazaTestu->IloscPytan = IloscPytan;
  wskBazaTestu->IndeksPytania = 0;
}




/*
 * Inicjalizuje test kojarzac zmienna dostepna poprzez wskaznik wskBazaTestu
 * z dana tablica wyrazen, ktora reprezentuje jeden z dwoch dopuszczalnych 
 * testow.
 * Parametry:
 *    wskBazaTestu - wskaznik na zmienna reprezentujaca baze testu.
 *    sNazwaTestu  - wskaznik na napis wybranego typu testu.
 *
 * Warunki wstepne:
 *      - Parametr wskBazaTestu nie moze byc pustym wskaznikiem. Musi zawierac adres
 *        zmiennej reprezentujacej baze testu, ktora wczesniej zostal poprawnie
 *        zainicjalizowany poprzez wywolanie funkcji InicjalizujTest.
 *      - Parametr sNazwaTestu musi wskazywac na jedna z nazw tzn. "latwe" lub "trudne" lub "plik".
 *       
 * Zwraca:
 *       true - gdy operacja sie powiedzie i test zostanie poprawnie
 *              zainicjalizowany,
 *       false - w przypadku przeciwnym.
 */
bool InicjalizujTest( BazaTestu  *wskBazaTestu, const char *sNazwaTestu )
{
  if (!strcmp(sNazwaTestu,"latwy")) 
  {
    UstawTest(wskBazaTestu,TestLatwy,sizeof(TestLatwy)/sizeof(WyrazenieZesp));
    return true;
  }
  else if(!strcmp(sNazwaTestu, "trudny"))
  {
    UstawTest(wskBazaTestu,TestTrudny,sizeof(TestTrudny)/sizeof(WyrazenieZesp));
    return true;
  }
  else if (!strcmp(sNazwaTestu, "plik"))
  {
    return true;
  }
  else
  {
    cerr << "Otwarcie testu '" << sNazwaTestu << "' nie powiodlo sie." << endl;
    return false;
  }  
}

/*!
 * Funkcja udostepnia nastepne pytania o ile jest odpowiednia ich ilosc.
 * Parametry:
 *       wskBazaTestu - wskaznik na zmienna reprezentujaca baze testu.
 *       wskWyrazenie - wskaznik na zmienna, do ktorej zostanie wpisane
 *                      kolejne wyrazenie z bazy testu.
 *
 * Warunki wstepne:
 *      - Parametr wskBazaTestu nie moze byc pustym wskaznikiem. Musi zawierac adres
 *        zmiennej reprezentujacej baze testu, ktora wczesniej zostal poprawnie
 *        zainicjalizowany poprzez wywolanie funkcji InicjalizujTest.
 *      - Parametr wskWyrazenie nie moze byc pustym wskaznikiem. Musi wskazywac na
 *        istniejaca zmienna.
 *
 * Zwraca:
 *       true - gdy operacja sie powiedzie i parametrowi *wskWyrazenie zostanie
 *              przypisane nowe wyrazenie zespolone z bazy,
 *       false - w przypadku przeciwnym.
 */
bool PobierzNastepnePytanie( BazaTestu  *wskBazaTestu, WyrazenieZesp *wskWyrazenie )
{
  if (wskBazaTestu->IndeksPytania >= wskBazaTestu->IloscPytan) return false;

  *wskWyrazenie = wskBazaTestu->wskTabTestu[wskBazaTestu->IndeksPytania];
  ++wskBazaTestu->IndeksPytania;
  return true;
}




/*
 * Procedura realizujaca obsluge pytan testowych
 * Wyswietla pytanie testowe
 * Oblicza
 * Wczytuje odpowiedz uzytkownika
 * Jesli jest ona nie poprawna skladniowo to daje 3 krotna szanse na wpisanie odpowiedzi
 * Jesli jest poprawna skladniowo to nalezy sprawdzic czy zgadza sie z poprawna odpowiedzia obliczona wczesniej
 *    Jesli odpowiedz jest poprawna to uzytkownikowi zostaje przyznany jeden punkt
      Jesli nie, informujemy uzytkownika o blednej odpowiedzi i wyswietlamy prawidlowa 


 * Warunki wstepne:
        -Referencja do struktury Statystyka_odpowiedzi
        -Kopia WyrazenieZesp bo oryginal jest nam niepotrzebny
*/
void ObslugaPytan(Statystyka_odpowiedzi & Statystyka, WyrazenieZesp WyrZ_PytanieTestowe)
{
  
  LZespolona Poprawny_Wynik;
  LZespolona Odp_Uzytkownika;
  Statystyka.ilosc_dzialan++;

  cout<<"\n:? Podaj wynik operacji: ";
  cout<<WyrZ_PytanieTestowe;

  Poprawny_Wynik = Oblicz(WyrZ_PytanieTestowe);

  cout<<"\n   Twoja odpowiedz: ";
  cin>>Odp_Uzytkownika;
  cin.ignore(1000,'\n');
  for(int i = 2; i>=1 && cin.fail() ; i--)                //Dopoki sa proby i cin jest w stanie fail
  {
    cin.clear();
    cin.ignore(1000,'\n');
    cout<<"Blad zapisu liczby,";
    cout<<" sprobuj jeszcze raz\n";
    cout<<"Zostalo jeszcze: "<<i<<" prob\n";
    cout<<"Twoja odpowiedz: ";
    cin>>Odp_Uzytkownika;
  }
    cin.clear();
    if(Poprawny_Wynik == Odp_Uzytkownika)
    {
      Statystyka.ilosc_poprawnych++;
      cout<<":) Odpowiedz poprawna\n";
    }
    else
    { 
      cout<<":( Blad. Poprawny Wynik to: ";
      cout<<Poprawny_Wynik;
      cout<<endl;
    }
}

/*Procedura realizujaca czytanie wyrazen zespolonych z pliku 
 * Uzytkownik musi podac nazwe pliku do wczytania
 * Sprawdzenie poprawnosci otwarcia
 * Kontrola poprawnosci wczytywania wyrazen z pliku
 * Zadawanie pytan testowych zgodnie z procedura ObslugaPytan
 
 * Warunki wstepne:
      -Referencja do struktury Statystyka_odpowiedzi
 */

void Test_z_Pliku(Statystyka_odpowiedzi &Statystyka)
{
  fstream plik;
  string nazwa_pliku;
  WyrazenieZesp WyrZ_PytanieTestowe;
  cout<<"Podaj nazwe pliku do odczytania: ";
  cin>>nazwa_pliku;
  cout<<endl;
  plik.open(nazwa_pliku,ios::in);
  if(!plik.good())
  {
    cerr<<"Blad otwarcia pliku\n";
    exit(0);
  }

  while(!plik.eof())                              //jesli plik jeszcze sie nie skonczyl
  {
    plik>>WyrZ_PytanieTestowe;
      if(plik.fail())
      {
        cerr<<"\nNapotkano bledne wyrazenie, zostalo ono pominiete\n";
        plik.clear();
        plik.ignore(1000,'\n');
      }
      else
      {
        ObslugaPytan(Statystyka,WyrZ_PytanieTestowe);
      }
  } 
  plik.close();
}
