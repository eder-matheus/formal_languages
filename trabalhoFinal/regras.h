#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#ifndef REGRAS_H
#define REGRAS_H

typedef struct {
    std::string variavel; // A variavel poderia ser um char, mas ainda não temos certeza se o nome da variavel possui somente um caractere
    std::vector<std::vector<std::string>> cadeia_simbolos; // A cadeia de simbolos representa as produções que uma variável tem
                                                          // Como uma produção pode ter mais de um simbolo, cada produção também é um vector
} REGRA;

#endif /* REGRAS_H */

