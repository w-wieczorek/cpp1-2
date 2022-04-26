# Opis konkursu gry na grafie

Repozytorium zawiera program serwera służącego do rozegrania gier turniejowych w grze _Undirected edge geography_.
Niniejszy opis zawiera zasady tej gry oraz protokół komunikacyjny, wg którego powinny działać programy klienckie.

## Instalacja serwera

Do instalacji serwera oraz jego uruchomienia potrzebna jest wirtualna maszyna Javy
[JVM](https://pl.wikipedia.org/wiki/Wirtualna_maszyna_Javy). W czasie testów korzystaliśmy
z __OpenJDK Runtime Environment__ w wersji "18".

Aby zainstalować serwer należy pobrać plik z repozytorium (z katalogu
[`serwer`](https://w-wieczorek.github.io/cpp1-2/konkurs/serwer/serwer.jar))
oraz zapisać na komputerze w dowolnym katalogu. Serwer jest gotowy do uruchomienia.

## Uruchomienie serwera

```
Usage: java -jar serwer.jar arguments
Where the allowed arguments are:
  -h | --help          Show help
  -i | --ip address    IP address of this server (required)
  -p | --port number   Port on listening (required)
  -g | --gsize number  The size of a game graph (default = 100 nodes)
  -t | --time seconds  Time limit for a single move (default = 2 seconds)
```

Przykład:
```
>java -jar serwer.jar --ip 192.168.0.15 --port 8080
Server started on /192.168.0.15:8080
```

Uruchomiony serwer nasłuchuje na uczestników turnieju. Aby rozpocząć grę, z serwerem muszą się
połączyć co najmniej dwa programy klienckie. Pierwszy zalogowany klient zagra z drugim zalogowanym
klientem, trzeci z czwartym itd.

## Turniej

Turniej składa się z rund, których liczba wyliczana jest w zależności od liczby zawodników i systemu. Rundy
składają się z gier. Kolejne rundy uruchamiane są ręcznie (za pomocą ponownego logowania klientów).
Turniej zostanie rozegrany [Systemem szwajcarskim](https://pl.wikipedia.org/wiki/System_szwajcarski)
z wykorzystaniem webowej aplikacji ([SwissSystem.org](https://swisssystem.org/)).

## Zasady gry

Rekwizytami są graf nieskierowany (bez pętli i bez krawędzi wielokrotnych) i żeton,
który na początku gry znajduje się w losowo wybranym wierzchołku grafu.

Gracze A i B naprzemiennie wykonują ruchy, gdzie ruch polega na przesunięciu żetonu
do jednego z sąsiednich wierzchołków, a następnie usunięciu krawędzi przez którą dokonano
przesunięcia. Przegrywa gracz, który nie może wykonać przesunięcia, gdy na niego przypada
ruch.

## Protokół komunikacyjny

Protokół komunikacyjny oparty jest na standardowych gniazdkach sieciowych; każda komenda wysyłana z lub
do serwera powinna kończyć się znakiem nowej linii. Używane w protokole numery wierzchołków są zgodne
z losowym grafem generowanym każdorazowo przed rozpoczęciem gry.

### Żądania klienta (wysyłają programy grające w turnieju)

```
210 [wierzchołek]   // Wyślij ruch, gdzie "wierzchołek" to numer kolejnego wierzchołka, na który
                    // należy przesunąć żeton.
```

Przykład:
```
210 17
```

### Odpowiedzi serwera

```
200 [opis gry]     // Rozpoczęcie gry; teraz Twoja kolej, wyślij komunikat 210.
220 [wierzchołek]  // Wierzchołek wybrany przez przeciwnika, serwer oczekuje na Twój ruch.
230                // Wygrałeś wg zasad.
231                // Wygrałeś przez przekroczenie czasu (przeciwnika).
240                // Przegrałeś wg zasad.
241                // Przegrałeś przez przekroczenie czasu.
999 [opis]         // Błąd komendy, opis powinien wyjaśnić przyczynę.
```

Przykłady:
```
200 6 2 5 3 5 2 5 0 2 1 3 4 5
220 3
230
```

Opis gry składa się z czterech części. Pierwsza część to liczba informująca, ile jest wierzchołków
w grafie (w ww. przykładzie 6). Druga część to numer wierzchołka, w którym znajduje się żeton
(w przykładzie 2). Trzecia część -- też jedna wartość -- to liczba równa mocy zbioru krawędzi
(w przykładzie 5). Czwarta część zawiera odpowiednią liczbę krawędzi zapisanych parami.
W przykładzie tymi pięcioma krawędziami są: (3, 5), (2, 5), (0, 2), (1, 3), (4, 5). Zakładamy,
że wierzchołki są numerowane od 0 kolejnymi liczbami naturalnymi.

Po otrzymaniu odpowiedzi 230 lub wyższej, klient powinien się rozłączyć.

### Przykładowa rozgrywka

<table>
<tr>
<th> Klient 1 </th>
<th> Klient 2 </th>
</tr>
<tr>
<td>
>java -jar klient.jar --ip 127.0.0.1 --port 8080<br/>
Received: 200 32 2 72 0 15 0 17 0 1 1 25 1 15 1 29<br/>
1 24 2 11 2 15 2 18 2 29 2 31 2 27 2 30 3 8 3 29<br/>
3 18 3 12 4 6 4 28 5 8 5 17 5 9 5 30 5 26 5 20 5 21<br/>
6 24 6 29 6 23 6 26 6 21 7 23 7 13 8 11 8 31 8 14<br/>
9 27 9 30 9 29 10 28 10 11 11 31 11 24 12 24 13 20<br/>
13 18 13 26 14 17 14 25 14 15 15 30 15 16 15 31<br/>
16 30 17 20 17 18 18 24 18 19 18 23 19 25 19 27<br/>
20 22 20 31 20 25 21 22 21 31 22 26 23 29 24 25<br/>
26 28 28 31<br/>
Sent: 210 29<br/>
Received: 220 6<br/>
Sent: 210 4<br/>
Received: 220 28<br/>
Sent: 210 10<br/>
Received: 220 11<br/>
Sent: 210 8<br/>
Received: 220 5<br/>
Sent: 210 26<br/>
Received: 220 13<br/>
Sent: 210 7<br/>
Received: 220 23<br/>
Sent: 210 6<br/>
Received: 220 26<br/>
Sent: 210 28<br/>
Received: 220 31<br/>
Sent: 210 8<br/>
Received: 220 14<br/>
Sent: 210 17<br/>
Received: 220 18<br/>
Sent: 210 2<br/>
Received: 220 11<br/>
Sent: 210 24<br/>
Received: 220 18<br/>
Sent: 210 23<br/>
Received: 220 29<br/>
Sent: 210 1<br/>
Received: 220 25<br/>
Sent: 210 24<br/>
Received: 220 6<br/>
Sent: 210 21<br/>
Received: 220 31<br/>
Sent: 210 11<br/>
Received: 230<br/>
Wygrałem wg zasad.
</td>
<td>
>java -jar klient.jar --ip 127.0.0.1 --port 8080<br/>
Received: 200 32 29 71 0 15 0 17 0 1 1 25 1 15<br/>
1 29 1 24 2 11 2 15 2 18 2 31 2 27 2 30 3 8 3 29<br/>
3 18 3 12 4 6 4 28 5 8 5 17 5 9 5 30 5 26 5 20<br/>
5 21 6 24 6 29 6 23 6 26 6 21 7 23 7 13 8 11<br/>
8 31 8 14 9 27 9 30 9 29 10 28 10 11 11 31 11 24<br/>
12 24 13 20 13 18 13 26 14 17 14 25 14 15 15 30<br/>
15 16 15 31 16 30 17 20 17 18 18 24 18 19 18 23<br/>
19 25 19 27 20 22 20 31 20 25 21 22 21 31 22 26<br/>
23 29 24 25 26 28 28 31<br/>
Sent: 210 6<br/>
Received: 220 4<br/>
Sent: 210 28<br/>
Received: 220 10<br/>
Sent: 210 11<br/>
Received: 220 8<br/>
Sent: 210 5<br/>
Received: 220 26<br/>
Sent: 210 13<br/>
Received: 220 7<br/>
Sent: 210 23<br/>
Received: 220 6<br/>
Sent: 210 26<br/>
Received: 220 28<br/>
Sent: 210 31<br/>
Received: 220 8<br/>
Sent: 210 14<br/>
Received: 220 17<br/>
Sent: 210 18<br/>
Received: 220 2<br/>
Sent: 210 11<br/>
Received: 220 24<br/>
Sent: 210 18<br/>
Received: 220 23<br/>
Sent: 210 29<br/>
Received: 220 1<br/>
Sent: 210 25<br/>
Received: 220 24<br/>
Sent: 210 6<br/>
Received: 220 21<br/>
Sent: 210 31<br/>
Received: 240<br/>
Przegrałem wg zasad.
</td>
</tr>
</table>
