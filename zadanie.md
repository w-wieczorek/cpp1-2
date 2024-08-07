## Zadanie 1. (na ocenę dostateczną)

Zaprojektuj klasę `Int` w taki sposób, żeby poniższy kod się wykonał zgodnie z komentarzem.

```cpp
int main(int argc, char* argv[]) 
{ 
    Int x = 3; 
    Int y = 4; 
    cout << x + y << endl; // 34 
    x = 15; 
    y = 634; 
    cout << x + y << endl; // 15634 
    return 0; 
} 
```

## Zadanie 2. (na ocenę dobrą)

Zaprojektuj klasy `Int` oraz `Expr` w taki sposób, żeby poniższy kod się wykonał zgodnie z komentarzem.

```cpp
int main(int argc, char* argv[]) 
{ 
    Int x = 3; 
    Int y = 4; 
    Expr z(add, 5, multiply, subtract, y, 1, x);
    cout << z.compute() << endl; // 14
    return 0; 
} 
```

## Zadanie 3. (na ocenę bardzo dobrą)

Zaprojektuj klasy `Int` oraz `Expr` w taki sposób, żeby poniższy kod się wykonał zgodnie z komentarzem.

```cpp
int main(int argc, char* argv[]) 
{ 
    Int x = 3; 
    Int y = 4; 
    Expr z = x + y; 
    cout << z.value << endl; // 7
    x = 5; 
    y = 6; 
    cout << z.value << endl; // 11
    return 0; 
} 
```