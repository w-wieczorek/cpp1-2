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
232                // Wygrałeś przez rozłączenie się przeciwnika.
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
