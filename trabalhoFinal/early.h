#include "regras.h"
#include "gramatica.h"

#ifndef EARLY_H
#define EARLY_H

void analisaSentenca(std::string const &sentenca, GRAMATICA const &G, std::vector<std::string> &terminaisSentenca);

// -----------------------------------------------------------------------------

int encontraMarcador(std::vector<std::string> const &producao);

// -----------------------------------------------------------------------------

void avancaMarcador(std::vector<std::string> &producao, int conjunto);

// -----------------------------------------------------------------------------

int encontraReducao(std::string const &variavel, std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di);

// -----------------------------------------------------------------------------

int encontraReducaoAnterior(std::string const &variavel, std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di);

// -----------------------------------------------------------------------------

void encontraPonteiros(std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di, REGRA const &producao, std::vector<std::vector<int>> &ponteiros);

// -----------------------------------------------------------------------------

void constroiD0(GRAMATICA const &G, std::vector<REGRA> &D0);

// -----------------------------------------------------------------------------

void constroiDr(GRAMATICA const &G, std::vector<std::vector<REGRA>> &Dr, std::vector<std::string> const &sentenca, std::vector<std::vector<int>> &ponteiros);

// -----------------------------------------------------------------------------

bool aceitaSentenca(std::vector<REGRA> const &Dn, GRAMATICA const &G);

// -----------------------------------------------------------------------------

void arvoreDeDerivacao(std::vector<std::vector<REGRA>> const &Dr, std::vector<std::vector<int>> const &ponteiros, GRAMATICA const &G);

// -----------------------------------------------------------------------------

void early(std::string const &entrada, GRAMATICA const &G);

// -----------------------------------------------------------------------------

#endif /* EARLY_H */

