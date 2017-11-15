#include <cstdlib>
#include <iostream>
#include <fstream>
#include <regex>
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

int le_gramatica (std::string arquivo, GRAMATICA &gramatica);

#endif /* GRAMATICA_H */

