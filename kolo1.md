## Wprowadzenie do zadań 1 i 2

Liczbę rzeczywistą można reprezentować za pomocą jednego łańcucha *s* i jednej
liczby całkowitej *p*. Sposób ten polega na odpowiednim umiejscowieniu kropki
dziesiętnej: albo przesuwamy ją w lewo (ujemne wartości *p*), albo w prawo
(dodatnie wartości *p*). Na przykład, jeśli `s = "123"` a `p = -1`, to te dwie
zmienne reprezentują wartość `12.3`; jeśli `s = "8"` a `p = 3`, to te dwie
zmienne reprezentują wartość `8000.0`.

---

## Zadanie 1. (na ocenę dostateczną)

Uzupełnij program w taki sposób, aby na ekranie wypisywał wartość liczby
reprezentowanej za pomocą zmiennych `s` i `p`.

```cpp
#include <stdio.h>
#include <stdlib.h>

int main() 
{ 
    char s[] = "71"; 
    int p = -5; 
    // Tu wstaw swój kod
    // Na ekranie powinna pojawić się wartość 0.00071
    return 0; 
} 
```

---

## Zadanie 2. (na ocenę dobrą)

Uzupełnij program w taki sposób, aby wyświetlił wynik dodawania dwóch
liczb rzeczywistych (zakładamy, że zmieszczą się one w typie `double`)
reprezentowanych odpowiednio za pomocą zmiennych `s1` i `p1` (pierwsza
liczba) oraz `s2` i `p2` (druga liczba).

```cpp
#include <stdio.h>
#include <stdlib.h>

int main() 
{ 
    char s1[] = "71", s2[] = "9029"; 
    int p1 = -2, p2 = -3; 
    // Tu wstaw swój kod
    // Na ekranie powinna pojawić się wartość 9.739
    return 0; 
} 
```

---

## Zadanie 3. (na ocenę bardzo dobrą)

Uzupełnij program w języku C, który na ekranie wypisze wszystkie
sufiksy danego łańcucha w porządku alfabetycznym. Długość łańcucha
`s` nie będzie przekraczała 50.

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{ 
    char s[] = "abrakadabra"; 
    // Tu wstaw swój kod
    // Na ekranie powinny się pojawić łańcuchy w podanej kolejności:
    // a, abra, abrakadabra, adabra, akadabra, bra, brakadabra, dabra,
    // kadabra, ra, rakadabra.
    return 0; 
} 
```
