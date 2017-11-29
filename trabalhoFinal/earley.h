#include "regras.h"
#include "gramatica.h"

#ifndef EARLY_H
#define EARLY_H

int encontraMarcador(std::vector<std::string> const &producao);

// -----------------------------------------------------------------------------

void avancaMarcador(std::vector<std::string> &producao, int conjunto);

// -----------------------------------------------------------------------------

void constroiD0 (GRAMATICA const &G, std::vector<REGRA> &D0);

// -----------------------------------------------------------------------------

void constroiDr (GRAMATICA const &G, std::vector<std::vector<REGRA>> &Dr, std::string sentenca);

// -----------------------------------------------------------------------------

#endif /* EARLY_H */

