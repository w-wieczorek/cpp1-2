# Opis konkursu gry w krążki

- [Opis konkursu gry w krążki](#opis-konkursu-gry-w-krążki)
  - [Instalacja serwera](#instalacja-serwera)
  - [Uruchomienie serwera](#uruchomienie-serwera)
  - [Turniej](#turniej)
  - [Zasady gry](#zasady-gry)
  - [Protokół komunikacyjny](#protokół-komunikacyjny)
    - [Żądania klienta (wysyłają programy grające w turnieju)](#żądania-klienta-wysyłają-programy-grające-w-turnieju)
    - [Odpowiedzi serwera](#odpowiedzi-serwera)
    - [Przykładowa rozgrywka](#przykładowa-rozgrywka)
  - [Randomowe programy klienckie](#randomowe-programy-klienckie)
    - [C++](#c)
  - [Warunki zaliczenia](#warunki-zaliczenia)

Repozytorium zawiera program serwera służącego do rozegrania gier turniejowych w grze _Krążki_.
Niniejszy opis zawiera zasady tej gry oraz protokół komunikacyjny, wg którego powinny działać programy klienckie.

## Instalacja serwera

Serwer jest prostą aplikacją okienkową działającą w Windowsie i nie wymaga specjalnej instalacji. Wystarczy pobrać plik z repozytorium (z katalogu
[`serwer`](https://w-wieczorek.github.io/cpp1-2/konkurs2025/serwer/serwer.7z))
oraz rozpakować na komputerze w dowolnym katalogu. Serwer jest gotowy do uruchomienia.

Serwer napisany jest w języku C# z wykorzystaniem [Avalonia UI](https://avaloniaui.net/), a jego kod źródłowy można podejrzeć na [githubie](https://github.com/w-wieczorek/SerwerKrazki).

## Uruchomienie serwera

Najpierw w katalogu, gdzie mamy program `SerwerKrazki.exe` tworzymy podkatalog `Programy` oraz plik `gracze.jsonl`. W katalogu `Programy` umieszczamy pliki wykonywalne (`.exe`) programów, które wezmą udział w turnieju oraz wymagane dla ich działania pliki `.dll` (o ile jakieś są potrzebne). Następnie tworzymy plik `gracze.jsonl` wg poniższego schematu:
```
{"Name": "Adam", "Surname": "Kowalski", "Program": "s12345.exe"}
{"Name": "Ewa", "Surname": "Gołąb", "Program": "s20006.exe"}
{"Name": "Bartłomiej", "Surname": "Kluska", "Program": "s32347.exe"}
{"Name": "Magdalena", "Surname": "Piekarska", "Program": "s42347.exe"}
{"Name": "Rafał", "Surname": "Żywczok", "Program": "s92399.exe"}
```

Po czym można uruchomić aplikację i rozpocząć turniej odpowiednim przyciskiem. Serwer w oddzielnych wątkach będzie uruchamiał poszczególne pary programów i przekazywał ich komunikaty.

## Turniej

Turniej składa się z rund, których liczba wyliczana jest w zależności od liczby zawodników i preferencji uczestników. Rundy składają się z gier. Kolejne gry oraz rundy uruchamiane są automatycznie.
Turniej zostanie rozegrany systemem, który da się przedstawić na przykładzie z siedmioma zawodnikami i trzema rundami:

|z1|z2|z3|z4|z5|z6|z7|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|2|3|4|5|6|7|1|
|3|4|5|6|7|1|2|
|4|5|6|7|1|2|3|

Numer zawodnika (`z1`, `z2` itd.) losowany jest przed rozpoczęciem turnieju.

## Zasady gry

Rekwizytami są są białe i czarne krążki ułożone w dziesięciu stertach. Liczba krążków w stercie nie przekracza 40. Ruch polega na wybraniu jednego z krążków własnego koloru z dowolnej niepustej sterty, a następnie zdjęciu go wraz ze wszystkimi krążkami leżącymi nad nim. Zdjęte krążki nie uczestniczą już w dalszej grze. Przegrywa ten, kto nie może wykonać ruchu.

Na poniższym przykładzie mamy tylko dwie sterty, które posłużą do zilustrowania przebiegu gry oraz przyjętej notacji zapisywania ruchów. 

![Rysunek](rysunek.svg)

Taki stan początkowy byłby reprezentowany za pomocą łańcucha `|cbbcb|bcc|`. Sterty numerujemy od zera, a ruch zapisujemy za pomocą pary numer stery oraz nowa pozycja, która tam wystąpiła po zagraniu ruchu. Na przykład, jeśli rozgrywkę rozpoczynają czarne, a ruch jaki wybrały polega na zdjęciu jednego czarnego krążka z drugiej sterty, to zapiszemy to jako `1 bc` (oznacza to stertę o indeksie 1, na której zostały dwa krążki, na spodzie biały, na górze czarny). Dalsza rozgrywka mogłaby przebiegać następująco: białe grają `0 cb`, czarne grają `0` (po tym zagraniu na stercie pierwszej, o indeksie 0, nie ma już krążków), białe grają `1` i czarne nie mają możliwości wykonania ruchu, więc przegrywają.

## Protokół komunikacyjny

Protokół komunikacyjny oparty jest na odczytywaniu i zapisywaniu łańcuchów ze standardowego we/wy; każda komenda wysyłana z lub do serwera (za pośrednictwem `cin` i `cout`) powinna kończyć się znakiem nowej linii (`endl`). Używany w protokole stan początkowy krążków jest zgodny z losowym ich uporządkowaniem generowanym każdorazowo przed rozpoczęciem gry.

### Żądania klienta (wysyłają programy grające w turnieju)

```
210 [numer sterty] [stan sterty]   // Wyślij ruch, gdzie "numer stery" określa skąd
                                   // zdejmujemy krążki.
```

Przykład:
```
210 7 bbbccbcbcbccb
```

### Odpowiedzi serwera

```
200 [opis gry]     // Rozpoczęcie gry; teraz Twoja kolej, wyślij komunikat 210.
220 [nr] [stan]    // Ruch wykonany przez przeciwnika, serwer oczekuje na Twój ruch.
230                // Wygrałeś wg zasad.
231                // Wygrałeś przez przekroczenie czasu (przeciwnika).
240                // Przegrałeś wg zasad.
241                // Przegrałeś przez przekroczenie czasu.
999 [opis]         // Błąd komendy, opis powinien wyjaśnić przyczynę.
```

Przykłady:
```
200 c |ccb|bcbc|bbbb|cbcccb|bbbccc|bcb|ccccb|bcbcbbccb||cbcbcccb|
220 3 cbccc
230
```

Opis gry składa się z dwóch części. Pierwsza część to znak (`b` lub `c`) informujący, jakim kolorem grasz (białe lub czarne). Druga część opisuje kolejność ułożenia krążków na poszczególnych stertach.

Limit czasowy na wysłanie swojego pierwszego i każdego kolejnego ruchu wynosi 200ms na ruch.

Po otrzymaniu odpowiedzi 230 lub wyższej, klient powinien zakończyć działanie.

### Przykładowa rozgrywka

```
Do z2: 200 b |cbcb|bbbb|bcbcb|cbc|bbbbbb|cccbb|bcbbbcc|cbcbc|ccc|cbcb|
Od z2: 210 6
Do z1: 200 c |cbcb|bbbb|bcbcb|cbc|bbbbbb|cccbb||cbcbc|ccc|cbcb|
Od z1: 210 2 bcb
Do z2: 220 2 bcb
Od z2: 210 4
Do z1: 220 4
Od z1: 210 7 cbcb
Do z2: 220 7 cbcb
Od z2: 210 5 cccb
Do z1: 220 5 cccb
Od z1: 210 0 cb
Do z2: 220 0 cb
Od z2: 210 5 ccc
Do z1: 220 5 ccc
Od z1: 210 7 cb
Do z2: 220 7 cb
Od z2: 210 0 c
Do z1: 220 0 c
Od z1: 210 9 cb
Do z2: 220 9 cb
Od z2: 210 1 bb
Do z1: 220 1 bb
Od z1: 210 2 b
Do z2: 220 2 b
Od z2: 210 7 c
Do z1: 220 7 c
Od z1: 210 3 cb
Do z2: 220 3 cb
Od z2: 210 3 c
Do z1: 220 3 c
Od z1: 210 9
Do z2: 220 9
Od z2: 210 2
Do z1: 220 2
Od z1: 210 0
Do z2: 220 0
Od z2: 210 1 b
Do z1: 220 1 b
Od z1: 210 3
Do z2: 220 3
Od z2: 210 1
Do z1: 220 1
Od z1: 210 5 cc
Do z2: 240
Do z1: 230
```

## Randomowe programy klienckie

### C++

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>

using namespace std;

char moj_kolor;
string sterty[10];

mt19937 gen32;  // generator liczb pseudolosowych

typedef struct {
    int nr;
    string sterta;
} ruch;

void losowy_ruch(ruch &r) {
	int i, j, licznik = 0;
	for (i = 0; i < 10; i++) {
        const int num_items = count(sterty[i].cbegin(), sterty[i].cend(), moj_kolor);
		licznik += num_items;
	}
	int wylosowany = gen32() % licznik;
	licznik = 0;
	for (i = 0; i < 10; i++) {
		const int num_items = count(sterty[i].cbegin(), sterty[i].cend(), moj_kolor);
		licznik += num_items;
		if (licznik > wylosowany) {
			r.nr = i;
			licznik -= num_items;
            j = 0;
			while (licznik <= wylosowany) {
				if (sterty[i][j++] == moj_kolor)
				    licznik++;
            }
			r.sterta = sterty[i].substr(0, j-1);
			break;
		}
	}
}

int main(int argc, char* argv[]) {
    auto seed_value = chrono::system_clock::now().time_since_epoch().count();
    gen32.seed(seed_value);
    string komunikat;
    getline(cin, komunikat);
    stringstream ss(komunikat);
    int kod;
    ss >> kod;
    if (kod != 200) {
        return 1;
    }
	ss >> moj_kolor;
    string stan;
	ss >> stan;
    size_t pos = 0;
    while ((pos = stan.find("||", pos)) != string::npos) {
        stan.replace(pos, 2, "|_|");
        pos += 2;
    }
    auto input = make_unique<char[]>(stan.length() + 1); // +1 for null terminator
    strncpy_s(input.get(), stan.length() + 1, stan.c_str(), stan.length());
    const char* delim = "|";
    char* next_token;
    char* token = strtok_s(input.get(), delim, &next_token);
    int i = 0;
    while (token && i < 10) {
		sterty[i++] = (token[0] != '_' ? string(token) : string(""));
        token = strtok_s(NULL, delim, &next_token);
    }
    while (kod < 230) {
        ruch odp;
        losowy_ruch(odp);
        cout << string("210 ") << to_string(odp.nr) 
            << (odp.sterta.length() > 0 ? " " : "") << odp.sterta << endl;
		cout.flush();
		sterty[odp.nr] = odp.sterta;
        ss.clear();
        getline(cin, komunikat);
        ss.str(komunikat);
        ss >> kod;
        if (kod == 220) {
            ss >> odp.nr;
			odp.sterta.clear();
			ss >> odp.sterta;
            sterty[odp.nr] = odp.sterta;
        }
    }
    return 0;
}
```

## Warunki zaliczenia

Programy grające w krążki powinny być napisane w języku C++. Ocena zaliczeniowa tego projektu zależy od liczby zdobytych punktów w turnieju. Autor programu, który uzyskał największą liczbę punktów (takich osób może być kilka) uzyskuje ocenę bardzo dobrą. Autor programu, który uzyskał najmniejszą liczbę punktów (takich osób może być kilka) uzyskuje ocenę dostateczną. Dla reszty osób oceny wyznaczone zostaną proporcjonalnie do liczby zdobytych punktów.