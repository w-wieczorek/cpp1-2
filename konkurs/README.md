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

Serwer napisany jest w języku [Scala](https://www.scala-lang.org/), a jego kod źródłowy można podejrzeć 
na [githubie](https://github.com/w-wieczorek/serwer).

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

### Scala

```scala
import java.net.Socket
import java.io._
import scalax.collection.GraphEdge.UnDiEdge
import scalax.collection.mutable
import scalax.collection.GraphPredef._
import scala.annotation.tailrec
import scala.language.postfixOps
import scala.util.Random

object Klient {
  val help: String = """
  |Usage: java -jar klient.jar arguments
  |Where the allowed arguments are:
  |  -h | --help          Show help
  |  -i | --ip address    IP address of a server (required)
  |  -p | --port number   Port to connect (required)
  |""".stripMargin

  def chooseOne[A](seq: List[A]): A = {
    val n = seq.size
    val r = Random.nextInt(n)
    seq(r)
  }

  def quit(status: Int = 0, message: String = ""): Nothing = {
    if (message.nonEmpty) println(s"ERROR: $message")
    println(help)
    sys.exit(status)
  }

  case class Args(ipAddress: Option[String], portNumber: Option[String])

  def parseArgList(params: Array[String]): Args = {
    @tailrec
    def pa(params2: Seq[String], args: Args): Args = params2 match {
      case Nil => args
      case ("-h" | "--help") +: Nil => quit()
      case ("-i" | "--ip") +: address +: tail =>
        pa(tail, args.copy(ipAddress = Some(address)))
      case ("-p" | "--port") +: number +: tail =>
        pa(tail, args.copy(portNumber = Some(number)))
      case _ => quit(1, s"Unrecognized argument ${params2.head}")
    }

    val argz = pa(params.toList, Args(None, None))
    if (argz.ipAddress.isEmpty || argz.portNumber.isEmpty)
      quit(1, "Must specify IP address and a port number.")
    argz
  }

  def main(params: Array[String]): Unit = {
    val argz = parseArgList(params)
    val connection = new Socket(argz.ipAddress.get, argz.portNumber.get.toInt)
    val outStream = connection.getOutputStream
    val out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(outStream)))
    val inStream = new InputStreamReader(connection.getInputStream)
    val in = new BufferedReader(inStream)
    val regex = """^(\d\d\d)\s*((?<=\s).*)?\s*$"""r
    var line = in.readLine
    println(s"Received: $line")
    var arr: Array[Int] = null
    line match {
      case regex(num, txt) =>
        if(num == "200") {
          arr = line.split(' ').map(_.toInt)
        } else {
          println("The game cannot be continued.")
          sys.exit(1)
        }
      case _ =>
        println("Unrecognized command.")
        sys.exit(1)
    }
    val responseCode = arr(0)
    var token = arr(2)
    var counter = arr(3)
    var j = 3
    val graph = mutable.Graph[Int, UnDiEdge]()
    for (n <- 0 until arr(1)) graph += n
    while (counter > 0) {
      j += 2
      graph += arr(j - 1) ~ arr(j)
      counter -= 1
    }
    def n(outer: Int): graph.NodeT = graph get outer
    var responseOpt = Option((responseCode, ""))
    while(responseOpt.isDefined && responseOpt.get._1 < 230) {
      val neighbours = n(token).neighbors.toList
      val nextNode: Int = chooseOne(neighbours).value
      graph -= token ~ nextNode
      token = nextNode
      out.println(s"210 $nextNode")
      out.flush()
      println(s"Sent: 210 $nextNode")
      line = in.readLine
      println(s"Received: $line")
      responseOpt = line match {
        case regex(num, txt) => Option((num.toInt, txt))
        case _ => None
      }
      if(responseOpt.isDefined) {
        responseOpt.get._1 match {
          case 220 =>
            val opponentMove = responseOpt.get._2.trim.toInt
            graph -= token ~ opponentMove
            token = opponentMove
          case 230 => println("Wygrałem wg zasad.")
          case 231 => println("Wygrałem przez przekroczenie czasu (przeciwnika).")
          case 240 => println("Przegrałem wg zasad.")
          case 241 => println("Przegrałem przez przekroczenie czasu.")
          case 999 => println(s"Błąd: ${responseOpt.get._2}")
        }
      } else {
        println(s"Nierozpoznana komenda: $line")
      }
    }
  }
}
```

### C#

Czekam na chętnego do napisania i podzielenia się z innymi...