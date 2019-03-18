#include "LZespolona.hh"
#include <iostream>
#include <iomanip>
using namespace std;

																//Przeciazenie operatora przesuniecia bitowego pelniacego funkcje
																//wyswietlenia liczby zespolonej
ostream & operator << (ostream & wyj, LZespolona Skl1)
{
	wyj<<"("<<Skl1.re<<showpos<<Skl1.im<<"i)"<<noshowpos;
	return wyj;
}

/*Obowiazujacy format liczby zespolonej:
'('Czesc rzeczywista Czesc urojona'i'')''*/
															//Wczytanie liczby zespolonej z strumienia wejsciowego i zapisanie
istream & operator >> (istream & wej, LZespolona &Skl1)		// jej w strukturze podanej przez uzytkownika
{

	wczytaj_i_sprawdz_znak(wej,'(');                        //Sprawdzenie czy wczytany znak jest '('
										
	wczytaj_sprawdz_zapisz_liczbe(wej,Skl1.re);             //wczytanie czesci rzeczywistej

	wczytaj_sprawdz_zapisz_liczbe(wej,Skl1.im);
	
	wczytaj_i_sprawdz_znak(wej,'i');

	wczytaj_i_sprawdz_znak(wej,')');


	return wej;
}


double Modul2(LZespolona Skl1)             					//Funkcja wyliczajaca modul do kwadratu z danej liczby zespolonej
{
	double Wynik = (Skl1.re * Skl1.re + Skl1.im * Skl1.im);
	return Wynik;
}
															//Wylicza sprzezenie liczby zespolonej
LZespolona Sprzezenie(LZespolona Skl1)    
{
	LZespolona Wynik;

	Wynik.re = Skl1.re;
	Wynik.im =-Skl1.im;
	return Wynik;
}

LZespolona operator / (LZespolona Skl1, double dzielnik)  	//Realizuje dzielenie liczby zespolonej przez typ double
{
	LZespolona Wynik;
	if(dzielnik){                     						//Sprawdzenie czy dzielnik aby napewno jest rozny od zera
		Wynik.re = Skl1.re / dzielnik;
		Wynik.im = Skl1.im / dzielnik;
		return Wynik;
	}
	else{
		cerr<<"Nie mozna dzielic przez zero\n";
		exit(0);
	}
}
															//Przeciazenie operatorow dzialan arytmetycznych
LZespolona operator + (LZespolona Skl1, LZespolona Skl2)
{
  	LZespolona  Wynik;

  	Wynik.re = Skl1.re + Skl2.re;
  	Wynik.im = Skl1.im + Skl2.im;
  	return Wynik;
}

LZespolona operator - (LZespolona Skl1, LZespolona Skl2)
{
	LZespolona Wynik;

	Wynik.re = Skl1.re - Skl2.re;
	Wynik.im = Skl1.im - Skl2.im;
	return Wynik;
}

LZespolona operator * (LZespolona Skl1, LZespolona Skl2)
{
	LZespolona Wynik;

	Wynik.re=((Skl1.re * Skl2.re) - (Skl1.im * Skl2.im));
	Wynik.im=((Skl2.im * Skl1.re) + (Skl1.im * Skl2.re));
	return Wynik;
}

LZespolona operator / (LZespolona Skl1, LZespolona Skl2)
{
	LZespolona Wynik;
	LZespolona Licznik;
	double Mianownik;

	Licznik = Skl1 * Sprzezenie(Skl2);
	Mianownik = Modul2(Skl2);

	Wynik = Licznik / Mianownik;          					 //wykorzystanie przeciazenie dla Lzespolona i liczby typu double

	return Wynik;

}

bool operator == (LZespolona Prawidlowa, LZespolona Do_Porownania)                  //Przeciazenie dla operatora porownania
{
	if(Prawidlowa.re == Do_Porownania.re && Prawidlowa.im == Do_Porownania.im) return true;
	else return false;
}

/*Funkcja wczytujaca znak ze strumienia wej i sprawdzajaca czy znak jest znakiem prawidlowym
Jesli nie to ustawiamy na strumieniu wejsciowym failbit*/

istream & wczytaj_i_sprawdz_znak(istream &wej,char znak_prawidlowy)
{
	char znak;
	wej>>znak;
	if(znak != znak_prawidlowy)
	{
		wej.setstate(ios::failbit);
		return wej;
	}

	return wej;
}
//Funkcja wczytujaca  ze strumienia wej liczbe typu double 
istream & wczytaj_sprawdz_zapisz_liczbe(istream &wej, double &liczba)
{
	double temp;
	wej >> temp;
	if(wej.fail())          										//jesli zostalo wczytanego cos nieprawidlowego to zwroc strumien
		return wej;
	else
	{
		liczba = temp;												//jesli wszystko ok to przypisz do liczby wartosc wczytana
		return wej;
	}
}