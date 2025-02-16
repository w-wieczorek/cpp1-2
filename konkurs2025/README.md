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
    - [C#](#c-1)

Repozytorium zawiera program serwera służącego do rozegrania gier turniejowych w grze _Krążki_.
Niniejszy opis zawiera zasady tej gry oraz protokół komunikacyjny, wg którego powinny działać programy klienckie.

## Instalacja serwera

Serwer jest prostą aplikacją okienkową działającą w Windowsie i nie wymaga specjalnej instalacji. Wystarczy pobrać plik z repozytorium (z katalogu
[`serwer`](https://w-wieczorek.github.io/cpp1-2/konkurs2025/serwer/serwer.7z))
oraz rozpakować na komputerze w dowolnym katalogu. Serwer jest gotowy do uruchomienia.

Serwer napisany jest w języku C# z wykorzystaniem [Avalonia UI](https://avaloniaui.net/), a jego kod źródłowy można podejrzeć na [githubie](https://github.com/w-wieczorek/SerwerKrazki).

## Uruchomienie serwera

Najpierw w katalogu, gdzie mamy program `SerwerKrazki.exe` tworzymy podkatalog `Programy` oraz plik `gracze.jsonl`. W katalogu `Programy` umieszczamy pliki wykonywalne (`.exe`) programów, które wezmą udział w turnieju. Następnie tworzymy plik `gracze.jsonl` wg poniższego schematu:
```
{"name": "Adam", "surname": "Kowalski", "program": "s12345.exe"}
{"name": "Ewa", "surname": "Gołąb", "program": "s20006.exe"}
{"name": "Bartłomiej", "surname": "Kluska", "program": "s32347.exe"}
{"name": "Magdalena", "surname": "Piekarska", "program": "s42347.exe"}
{"name": "Rafał", "surname": "Żywczok", "program": "s92399.exe"}
```

Po czym można uruchomić aplikację i rozpocząć turniej odpowiednim przyciskiem. Serwer w oddzielnych wątkach będzie uruchamiał poszczególne pary programów i przekazywał ich komunikaty.

## Turniej

Turniej składa się z rund, których liczba wyliczana jest w zależności od liczby zawodników i preferencji uczestników. Rundy składają się z gier. Kolejne gry oraz rundy uruchamiane są automatycznie.
Turniej zostanie rozegrany systemem, który da się przedstawić na przykładzie z siedmioma zawodnikami i czterema rundami:

|z1|z2|z3|z4|z5|z6|z7|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|2|3|4|5|6|7|1|
|3|4|5|6|7|1|2|
|4|5|6|7|1|2|3|
|5|6|7|1|2|3|4|

Numer zawodnika (`z1`, `z2` itd.) losowany jest przed rozpoczęciem turnieju.

## Zasady gry

Rekwizytami są są białe i czarne krążki ułożone w dziesięciu stertach. Liczba krążków w stercie nie przekracza 30. Ruch polega na wybraniu jednego z krążków własnego koloru z dowolnej sterty, a następnie zdjęciu go wraz ze wszystkimi krążkami leżącymi nad nim. Zdjęte krążki nie uczestniczą już w dalszej grze. Przegrywa ten, kto nie może wykonać ruchu.

Na poniższym przykładzie mamy tylko dwie sterty, które posłużą do zilustrowania przebiegu gry oraz przyjętej notacji zapisywania ruchów. 

![Rysunek](rysunek.svg)

Taki stan początkowy byłby reprezentowany za pomocą łańcucha `|cbbcb|bcc|`. Sterty numerujemy od zera, a ruch zapisujemy za pomocą pary numer stery oraz nowa pozycja, która tam wystąpiła po zagraniu ruchu. Na przykład, jeśli rozgrywkę rozpoczynają czarne, a ruch jaki wybrały polega na zdjęciu jednego czarnego krążka z drugiej sterty, to zapiszemy to jako `1 bc` (oznacza to stertę o indeksie 1, na której zostały dwa krążki, na spodzie biały, na górze czarny). Dalsza rozgrywka mogłaby przebiegać następująco: białe grają `0 cb`, czarne grają `0` (po tym zagraniu na stercie pierwszej, o indeksie 0, nie ma już krążków), białe grają `1` i czarne nie mają możliwości wykonania ruchu, więc przegrywają.

## Protokół komunikacyjny

Protokół komunikacyjny oparty jest na odczytywaniu i zapisywaniu łańcuchów ze standardowego we/wy; każda komenda wysyłana z lub do serwera (za pośrednictwem `cin` i `cout`) powinna kończyć się znakiem nowej linii (`endl`). Używany w protokole stan początkowy krążków jest zgodne z losowym ich uporządkowaniem generowanym każdorazowo przed rozpoczęciem gry.

### Żądania klienta (wysyłają programy grające w turnieju)

```
210 [numer sterty] [stan sterty]   // Wyślij ruch, gdzie "numer stery" określa skąd,
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
200 c |ccb|bcbc|bbbb|cbcccb|bbbccc|bcb|ccccb|bcbcbbccb|b|cbcbcccb|
220 3 cbccc
230
```

Opis gry składa się z dwóch części. Pierwsza część to znak (`b` lub `c`) informujący, jakim kolorem grasz (białe lub czarne). Druga część opisuje kolejność ułożenia krążków na poszczególnych stertach

Po otrzymaniu odpowiedzi 230 lub wyższej, klient powinien zakończyć działanie.

### Przykładowa rozgrywka

<table>
<tr>
<th> Klient 1 </th>
<th> Klient 2 </th>
</tr>
<tr>
<td><pre>
>java -jar klient.jar --ip 127.0.0.1 --port 8080
Received: 200 32 2 72 0 15 0 17 0 1 1 25 1 15 1 29
1 24 2 11 2 15 2 18 2 29 2 31 2 27 2 30 3 8 3 29
3 18 3 12 4 6 4 28 5 8 5 17 5 9 5 30 5 26 5 20 5 21
6 24 6 29 6 23 6 26 6 21 7 23 7 13 8 11 8 31 8 14
9 27 9 30 9 29 10 28 10 11 11 31 11 24 12 24 13 20
13 18 13 26 14 17 14 25 14 15 15 30 15 16 15 31
16 30 17 20 17 18 18 24 18 19 18 23 19 25 19 27
20 22 20 31 20 25 21 22 21 31 22 26 23 29 24 25
26 28 28 31
Sent: 210 29
Received: 220 6
Sent: 210 4
Received: 220 28
Sent: 210 10
Received: 220 11
Sent: 210 8
Received: 220 5
Sent: 210 26
Received: 220 13
Sent: 210 7
Received: 220 23
Sent: 210 6
Received: 220 26
Sent: 210 28
Received: 220 31
Sent: 210 8
Received: 220 14
Sent: 210 17
Received: 220 18
Sent: 210 2
Received: 220 11
Sent: 210 24
Received: 220 18
Sent: 210 23
Received: 220 29
Sent: 210 1
Received: 220 25
Sent: 210 24
Received: 220 6
Sent: 210 21
Received: 220 31
Sent: 210 11
Received: 230
Wygrałem wg zasad.
</pre>
</td>
<td><pre>
>java -jar klient.jar --ip 127.0.0.1 --port 8080
Received: 200 32 29 71 0 15 0 17 0 1 1 25 1 15
1 29 1 24 2 11 2 15 2 18 2 31 2 27 2 30 3 8 3 29
3 18 3 12 4 6 4 28 5 8 5 17 5 9 5 30 5 26 5 20
5 21 6 24 6 29 6 23 6 26 6 21 7 23 7 13 8 11
8 31 8 14 9 27 9 30 9 29 10 28 10 11 11 31 11 24
12 24 13 20 13 18 13 26 14 17 14 25 14 15 15 30
15 16 15 31 16 30 17 20 17 18 18 24 18 19 18 23
19 25 19 27 20 22 20 31 20 25 21 22 21 31 22 26
23 29 24 25 26 28 28 31
Sent: 210 6
Received: 220 4
Sent: 210 28
Received: 220 10
Sent: 210 11
Received: 220 8
Sent: 210 5
Received: 220 26
Sent: 210 13
Received: 220 7
Sent: 210 23
Received: 220 6
Sent: 210 26
Received: 220 28
Sent: 210 31
Received: 220 8
Sent: 210 14
Received: 220 17
Sent: 210 18
Received: 220 2
Sent: 210 11
Received: 220 24
Sent: 210 18
Received: 220 23
Sent: 210 29
Received: 220 1
Sent: 210 25
Received: 220 24
Sent: 210 6
Received: 220 21
Sent: 210 31
Received: 240
Przegrałem wg zasad.
</pre>
</td>
</tr>
</table>

## Randomowe programy klienckie

### C++

```c++
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <boost/chrono.hpp>
#include <random>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using boost::asio::ip::tcp;
using namespace boost;

int main(int argc, char* argv[]) {
    unsigned seed = boost::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen32(seed);  // generator liczb pseudolosowych
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    int zeton;
    if (argc != 3) {
        cerr << "Uzycie: prog.exe adres_ip_serwera port" << endl;
        return 1;
    }
    try {
        tcp::iostream serwer(argv[1], argv[2]);  // proba nawiazania polaczenia z serwerem
        if (!serwer) {
            cout << "Polaczenie z serwerem nieudane." << endl;
            return 1;
        }
        string komunikat;
        getline(serwer, komunikat);
        cout << "Odebralem: " << komunikat << endl;
        vector<int> liczby;
        stringstream ss(komunikat);
        int liczba;
        while (ss >> liczba) {
            liczby.push_back(liczba);
        }
        int kod = liczby[0];
        if (kod != 200) {
            return 1;
        }
        Graph g(liczby[1]);
        zeton = liczby[2];
        int liczba_krawedzi = liczby[3];
        for (int i = 0; i < liczba_krawedzi; i++) {
            add_edge(liczby[4 + 2*i], liczby[5 + 2*i], g);
        }
        liczby.clear();
        while (kod < 230) {
            int sasiedzi[7];
            int k = 0;
            typename graph_traits<Graph>::adjacency_iterator ai;
            typename graph_traits<Graph>::adjacency_iterator ai_end;
            for (tie(ai, ai_end) = adjacent_vertices(zeton, g); ai != ai_end; ++ai) {
                sasiedzi[k++] = *ai;
            }
            int odp = sasiedzi[gen32() % k];  // losowy ruch klienta
            serwer << string("210 ") + to_string(odp) + string("\n");
            serwer.flush();
            remove_edge(zeton, odp, g);
            zeton = odp;
            cout << "Wyslalem: 210 " << odp << endl;
            ss.clear();
            getline(serwer, komunikat);
            cout << "Odebralem: " << komunikat << endl;
            ss.str(komunikat);
            ss >> kod;
            if (kod == 220) {
                int ruch;
                ss >> ruch;
                remove_edge(zeton, ruch, g);
                zeton = ruch;
            }
        }
        serwer.close();
    }
    catch (std::exception& e) {
        cout << "Wyjatek: " << e.what() << endl;
    }
    return 0;
}
```

### C#

[github.com/michalmrowiec/Game-on-the-graph](https://github.com/michalmrowiec/Game-on-the-graph)
