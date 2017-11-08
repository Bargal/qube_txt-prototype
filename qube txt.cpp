// qube txt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>

struct qube					// struktura opisujaca ekcje gry
{
	std::string infoQ;	//opis tekstowy boxa
	int hpQ;			//obrazenia lub leczenie (hp+ hp-)
	bool exitQ[4];		//tablica wyjsc | 0-W | 1-D | 2-S | 3-A
	int timeQ;			//zmienna rund zabieranych przez box
	int beInsideQ;		//licznik ilosci pobytow w boxie
};


const int xM = 10 ;			//
const int yM = xM;			// deklaracja rozmiaru gry

// zmienne globalne

qube matrix[xM][yM];		//tblica struktur boxow wraz z inicjacja

// prototypy funkcji

void defineQ();											//inicjacja i wprowadzanie danych do tablicy struktur planszy gry
int mainGame(int, int, int, int, int&, int&, int);					//glowny modul gry (tabela boxow, start X Y, end X Y, hpq
int move(int&, int&, int, int, int);					//analiza ruchu (polozenie gracza X Y )
void opisQ(int, int, int, int, int, int, int, int);		//opis pomieszczenia po wejscu
int menuQ(int, int, int, int);
int gameOver(int, int, int, int);							//rozliczenie końcowe gry (endgame, hp, losc ruchow, timer)

int main()
{
	using std::cin;
	using std::cout;
	using std::endl;

	srand(time(NULL));

	int end = 0; // !=0 - koniec petli pomocniczej

	const int timer{ 50 };			// deklaracja ilosci rund

	do  //petla pomocnicza
	{
		int hpq = 20;		//zdrowie gracza
		int mov = 0;		//ilosc skończonych rund
		int rndX = (xM / 3);
		int rndY = (yM / 3);

		const int startX = (std::rand() % rndX);					   //pozycja startowa
		const int startY = (std::rand() % rndY);					   //pozycja startowa
		const int endX = (std::rand() % rndX) + (rndX * 2);			   //wyjscie
		const int endY = (std::rand() % rndY) + (rndY * 2);			   //wyjscie

		int endgame = 0;		//znacznik końca gry

		system("cls");

		cout << "Witam w qube txt" << endl;
		cout << "\nzaraz wejdziesz do losowego boxu " << endl;
		cout << "twoim zadaniem bedzie wydostanie sie na zewnatrz" << endl;
		cout << "w tym celu uzyj klawiszy WSAD :" << endl;
		cout << "\n W - polnoc\n E - poludnie\n A - zachod\n D - wschod" << endl;
		cout << "spokojnie - nie musisz tego zapamietac - bedzie to podawane podczas gry :)" << endl;
		cout << "\nNa poczatek masz 10 punktow zycia.\nCzesc pokojow zadaje obrazena a odpoczynek w czesci pokojow leczy rany" << endl;
		cout << "odpoczywasz uzywajac klawisza Q" << endl;
		cout << "masz " << timer << " jednostek czasu a kazdy pokoj zabiera pewna ilosc czasu na ture" << endl;
		cout << "\nzycze milej zabawy, nacisnij dowolny klawisz aby zaczac." << endl;
		cout << "koncowy wynik zalezy od czasu przejscia i pozostalgo zdrowia" << endl;
		cout << "\n\nNacisnij dowolny klawisz" << endl;
		_getch();

		defineQ();			//uzupełnianie danych
		endgame = mainGame(startX, startY, endX, endY, hpq, mov, timer); //pętla główna gry
		end = gameOver(endgame, hpq, mov, timer); //koniec gry
	} while (end == 0);

	return 0;
}

void defineQ()
{
	int  p = 1; //numerator nazwy pokoju

	for (int i = 0; i < xM; i++)
		for (int j = 0; j < yM; j++)
		{
			matrix[i][j].beInsideQ = 1;
			matrix[i][j].infoQ = "pokoj numer  ";				//opis pokoju - moze z pliku
			matrix[i][j].infoQ += std::to_string(p); p++;		//numer pokoju
			matrix[i][j].hpQ = (std::rand() % 5) - 2;			//losowe okreslenie obrazem lub leczenia w zakresie +/-2
			matrix[i][j].timeQ = (std::rand() % 4) + matrix[i][j].hpQ;			//Im bezpieczniej, tym czas szybciej idzie
			matrix[i][j].exitQ[0] = true;
			matrix[i][j].exitQ[1] = true;
			matrix[i][j].exitQ[2] = true;
			matrix[i][j].exitQ[3] = true;
			if (i == 0) matrix[i][j].exitQ[0] = false;			//dla blokady w
			if (j == (yM - 1)) matrix[i][j].exitQ[1] = false;	//dla blokady d
			if (i == (xM - 1)) matrix[i][j].exitQ[2] = false;	//dla blokady s
			if (j == 0) matrix[i][j].exitQ[3] = false;			//dla blokady a
		}
}

int mainGame(int x, int y, int endX, int endY, int &hpq, int &mov, int timer)
{
	const int startX = x;
	const int startY = y;
	int endgame = 0;	//znacznik końca gry 0-gramy | 1-zdrowie | 2-ucieczka | 3-wygrana! | 4-koniec czasu
	int door = 5;		//znacznik pokonanych ostatnio drzwi (odwrotny wdsa) 5 - pierwsze wejscie
	int moveI = 9;		//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit | 9 - wartosc startowa

	system("cls");
	std::cout << "\n\n\n\nZaraz wejdziesz do gry!!!" << std::endl;
	std::cout << "\nzaczniesz w miejscu oznaczonym jako " << matrix[x][y].infoQ << std::endl;
	std::cout << "wyjscie znajduje sie w pokoju oznaczonym jako " << matrix[endX][endY].infoQ << std::endl;
	std::cout << "\nzapamietaj to, bo od teraz podawany bedzie tylko numer pokoju w ktorym aktualnie jestes!" << std::endl;
	std::cout << "\n\nWspinasz sie po drabince do pomieszczenia od ktorego zaczniesz gre,\ncelem jest dotrzec jak najszybciej do pomieszczenia z taka sama klapa w podlodze i wyjscie" << std::endl;
	std::cout << "\n\nNacisnij dowolny klawisz" << std::endl;
	_getch();

	do					//głowna pętla gry
	{
		system("cls");

		hpq += matrix[x][y].hpQ;  //zmiana HP gracza w nowym pokoju
		if (hpq <= 0) return 1;
		mov += matrix[x][y].timeQ;  //zmiana rundy gracza w nowym pokoju
		if (mov >= timer) return 4;

		std::cout << "    HP  " << hpq << "   zakonczone rundy  " << mov << "    Pozostalo czasu  " << timer - mov << std::endl << std::endl;

		opisQ(x, y, hpq, startX, startY, endX, endY, moveI);
		moveI = menuQ(x, y, endX, endY);
		endgame = move(x, y, moveI, endX, endY);
	} while (endgame == 0);

	return endgame;
}

void opisQ(int x, int y, int hpq, int startX, int startY, int endX, int endY, int moveI)
{
	using std::cin;
	using std::cout;
	using std::endl;

	cout << "wchodzisz do pomieszczenia przez ";
	if (moveI == 9) cout << "klape w podlodze " << endl;
	if (moveI == 0) cout << "poludniowe drzwi " << endl;
	if (moveI == 1) cout << "zachodnie drzwi " << endl;
	if (moveI == 2) cout << "polnocne drzwi " << endl;
	if (moveI == 3) cout << "wschodnie drzwi " << endl;
	cout << "w jednolicie bialym pokoju widzisz tylko na kazdej scianie napis \n\n    \"" << matrix[x][y].infoQ << "\"" << endl;
	cout << "\no ile pamietasz jestes w tym pokoju po raz " << matrix[x][y].beInsideQ << endl;
	matrix[x][y].beInsideQ++;     //dodanie liczby odwiedzin w pokoju
	cout << "\nwychodza z tad drzwi na ";
	if (matrix[x][y].exitQ[0] == true) cout << "polnoc ";
	if (matrix[x][y].exitQ[1] == true) cout << "wschod ";
	if (matrix[x][y].exitQ[2] == true) cout << "poludnie ";
	if (matrix[x][y].exitQ[3] == true) cout << "zachod ";
	if (x == startX && y == startY) cout << "\nw podloze widzisz klape przez ktora sie tu dostales" << endl;
	if (x == endX && y == endY)cout << "\n w podlodze widzisz nieznana klape, czyzby upragnione wyjscie??" << endl;
	cout << "\nJednoczesnie czujesz ze w pokoju panuje ";
	if (matrix[x][y].hpQ == 0) cout << "neutralna atmosfera " << endl;
	if (matrix[x][y].hpQ > 0) cout << "przyjazna atmosfera i orzymujesz " << matrix[x][y].hpQ << " HP" << endl;
	if (matrix[x][y].hpQ < 0) cout << "szkodliwa atmosfera i orzymujesz " << matrix[x][y].hpQ << " HP" << endl;
	cout << "obecnie twoje zdrowie wynosi " << hpq << endl;
	cout << "\nPobyt w tym pokoju kosztuje " << matrix[x][y].timeQ << " rund" << endl;
}

int menuQ(int x, int y, int endX, int endY)
{
	std::cout << std::endl;
	std::cout << "nacisnij E zeby odpoczac w pokoju jedna runde\n" << std::endl;
	if (matrix[x][y].exitQ[0])  std::cout << "nacisnij W zeby isc na polnoc" << std::endl;
	if (matrix[x][y].exitQ[1])  std::cout << "nacisnij D zeby isc na wschod" << std::endl;
	if (matrix[x][y].exitQ[2])  std::cout << "nacisnij S zeby isc na poludie" << std::endl;
	if (matrix[x][y].exitQ[3])  std::cout << "nacisnij A zeby isc na zachod" << std::endl;
	if (x == endX && y == endY)  std::cout << "\n\a\a****** nacisnij Q zeby skorzystac z klapy w podlodze *****" << std::endl;
	else std::cout << "\n *** nacisnij R - zrezygnuj i wyjdz z gry teraz ***" << std::endl;

	int tst = 0;
	char moveC;				//znak z klawiatury
	int moveI = 9;			//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit

	do
	{
		tst = 0;
		moveC = _getch(); //pobieranie znaku z klawiatury

		if ((moveC == 'w' || moveC == 'W') && matrix[x][y].exitQ[0]) moveI = 0;
		else if ((moveC == 'd' || moveC == 'D') && matrix[x][y].exitQ[1]) moveI = 1;
		else if ((moveC == 's' || moveC == 'S') && matrix[x][y].exitQ[2]) moveI = 2;
		else if ((moveC == 'a' || moveC == 'A') && matrix[x][y].exitQ[3]) moveI = 3;
		else if ((moveC == 'q' || moveC == 'Q') && (x == endX && y == endY)) moveI = 4;
		else if (moveC == 'e' || moveC == 'E') moveI = 5;
		else if (moveC == 'r' || moveC == 'R') moveI = 6;
		else tst = 1;
	} while (tst == 1);
	return moveI;
}

int move(int &x, int &y, int moveI, int endX, int endY)
{
	switch (moveI)
	{
	case 0: x--; break;
	case 1: y++; break;
	case 2: x++; break;
	case 3: y--; break;
	case 4: return 3;
	case 5: return 0;
	case 6: return 2;
	default: menuQ(x, y, endX, endY);
	}
	return 0;
}

int gameOver(int endgame, int hpq, int mov, int timer)
{
	using std::cout;
	using std::endl;

	system("cls");
	cout << "    HP  " << hpq << "   zakonczone rundy  " << mov << "    Pozostalo czasu  " << timer - mov << endl << endl;
	if (endgame == 1) cout << "\nNiestety twoje zdrowie spadlo do zera... niezyjesz :) ";
	if (endgame == 2) cout << "\nUciekasz wyjsciem bezpieczenstwa przed zakonczeniem zadania.";
	if (endgame == 4) cout << "\nNiestety koniec czasu - przegrales.";
	if (endgame == 3)
	{
		cout << "\n\nGRATULACJE udalo ci sie wygrac !!!!";
		cout << "\nuzyskales " << hpq / 2 + (timer - mov) << " punktow" << endl;
	}
	cout << "\nKoniec gry nacisnij Q\nponowna gra nacisnij E" << endl;
	int tst = 0;
	do
	{
		char a = 0;
		a = _getch();
		if (a == 'e' || a == 'E')return 0;
		else if (a == 'q' || a == 'Q')return 1;
		else tst = 1;
	} while (tst == 1);
	return 1;
}