#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#ifndef REGRAS_H
#define REGRAS_H

typedef struct {
    std::string variavel; // A variavel poderia ser um char, mas ainda não temos certeza se o nome da variavel possui somente um caractere
    std::vector<std::string> cadeia_simbolos; // A cadeia de simbolos representa uma das possiveis produções que uma variável tem
} REGRA;

typedef struct {
    std::string variavel; // A variavel poderia ser um char, mas ainda não temos certeza se o nome da variavel possui somente um caractere
    std::vector<std::string> cadeia_simbolos; // A cadeia de simbolos representa uma das possiveis produções que uma variável tem
} FECHO;

#endif /* REGRAS_H */

