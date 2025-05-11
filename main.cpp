//Techniki programowania projekt 2 zadanie 6. Seria wagonów jest generowana w osobnym wątku
//Autorzy: Tymoteusz Stańczewski 193986, Mikołaj Saczewa
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void generuj_wagony(vector<char>& kolejka, vector<char>& zestawy) {
	vector<char> typy = { 'A', 'B', 'C', 'D' };
	uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	uniform_int_distribution<> dist(0, 3);

	for (int i = 0; i < 4; i++) {
		char litera = typy[dist(gen)];
		zestawy.insert(zestawy.begin(), litera);
		kolejka.push_back(litera);
		kolejka.push_back(litera);
	}
	shuffle(kolejka.begin(), kolejka.end(), gen);
}
void rysuj_tory(vector<char> kolejka, vector<char> zgadniete[5], vector<char> zestawy[6], int punkty) {
	vector<char> kolejka_8(kolejka.end() - 8, kolejka.end());
	int odstep = 38;
	int spacje = 18;
	cout << "liczba punktow: " << punkty << endl;
	for (char i : kolejka_8)
		std::cout << i;
	cout << " " << string(10 + odstep - zgadniete[0].size(), '=');
	for (char j : zgadniete[0])
		std::cout << j;
	cout << " Tor " << 1 << ": " << "[";
	for (char j : zestawy[0])
		std::cout << j;
	cout << "]" << endl << string(spacje, ' ');
	for (int i = 1; i < 5; i++) {
		spacje++;
		cout << "\\\\" << "(" << i + 1 << ")" << endl << string(spacje, ' ') << "\\\\";
		spacje++;
		cout << string(odstep - zgadniete[i].size() - 2 * i, '=');
		for (char j : zgadniete[i])
			std::cout << j;
		cout << " Tor " << i + 1 << ": " << "[";
		for (char j : zestawy[i])
			std::cout << j;
		cout << "]" << endl << string(spacje, ' ');
	}
	cout << endl;
}
int wybierz_tor() {
	int wybranyTor = 0;
	char przycisk;
	while (1) {
		cin >> przycisk;

		switch (przycisk) {
		case '2':
			wybranyTor = 1;
			break;
		case '3':
			wybranyTor = 2;
			break;
		case '4':
			wybranyTor = 3;
			break;
		case '5':
			wybranyTor = 4;
			break;
		case 'w':
			return wybranyTor;
		default:
			wybranyTor = 0;
			break;
		}
	}
}
void aktualizuj(vector<char>& kolejka, vector<char> zgadniete[5], int tor) {
	zgadniete[tor].insert(zgadniete[tor].begin(), kolejka.back());
	kolejka.pop_back();
}
void resetuj_tor(vector<char>& zgadniete, vector<char>& zestawy, vector<char>& kolejka) {
	zgadniete.clear();
	zestawy.clear();
	vector<char> tymczasowy = {};
	thread generuj(generuj_wagony, ref(tymczasowy), ref(zestawy));
	generuj.join();
	kolejka.insert(kolejka.begin(), tymczasowy.begin(), tymczasowy.end());
}
int main()
{
	int punkty = 0;


	vector<char> kolejka = {};
	vector<char> zestawy[5];
	for (int i = 0; i < 5; i++) {
		thread generuj(generuj_wagony, ref(kolejka), ref(zestawy[i]));
		generuj.join();
	}
	vector<char> zgadniete[6];
	rysuj_tory(kolejka, zgadniete, zestawy, punkty);

	int tor = 0;

	cout << endl;
	while (punkty >= 0 && punkty < 50)
	{
		tor = wybierz_tor();

		aktualizuj(kolejka, zgadniete, tor);
		if (zgadniete[tor].size() == zestawy[tor].size()) {
			if (zgadniete[tor] == zestawy[tor]) {
				punkty = punkty + 10;
				resetuj_tor(zgadniete[tor], zestawy[tor], kolejka);
			}
			else {
				punkty = punkty - 10;
				resetuj_tor(zgadniete[tor], zestawy[tor], kolejka);
			}
		}
		rysuj_tory(kolejka, zgadniete, zestawy, punkty);
	}
	if (punkty >= 50)
		cout << "wygrana";
	else
		cout << "przegrana";
}
