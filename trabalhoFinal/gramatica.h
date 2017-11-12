#include <cstdlib>
#include <iostream>
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

#endif /* GRAMATICA_H */

