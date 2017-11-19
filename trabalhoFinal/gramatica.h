#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "regras.h"

#ifndef GRAMATICA_H
#define GRAMATICA_H

typedef struct {
    std::vector<std::string> variaveis;
    std::vector<std::string> terminais;
    std::vector<REGRA> regras;
    std::string inicial;
} GRAMATICA;

int le_gramatica(std::string arquivo, GRAMATICA &gramatica);

bool encontraVariavel(std::string const &variavel, std::vector<std::string> const &variaveis);

bool encontraTerminal(std::string const &terminal, std::vector<std::string> const &terminais);

void removeProducoesVazias(GRAMATICA const &G, GRAMATICA &G1);

void removeSimbolosInuteis(GRAMATICA const &G, GRAMATICA &G1);

void imprimeGramatica(GRAMATICA const &g);

void removeProducoesUnitarias(GRAMATICA const &G, GRAMATICA &G1);

#endif /* GRAMATICA_H */

