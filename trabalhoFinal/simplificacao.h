#include "gramatica.h"

#ifndef SIMPLIFICACAO_H
#define SIMPLIFICACAO_H

void removeProducoesVazias(GRAMATICA const &G, GRAMATICA &G1);

// -----------------------------------------------------------------------------

void removeSimbolosInuteis(GRAMATICA const &G, GRAMATICA &G1);

// -----------------------------------------------------------------------------

void removeProducoesUnitarias(GRAMATICA const &G, GRAMATICA &G1);

// -----------------------------------------------------------------------------

void simplificaGramatica(GRAMATICA const &G, GRAMATICA &G1, GRAMATICA &G2);

// -----------------------------------------------------------------------------

#endif /* SIMPLIFICACAO_H */

