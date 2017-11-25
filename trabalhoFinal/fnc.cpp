#include "fnc.h"

void transformaMaiorDois(GRAMATICA &G) {
	std::vector<std::string> V2 = G.variaveis;
	std::vector<REGRA> P2 = G.regras;
	int i = 0;
	int tamanhoP2 = P2.size();

	for (int i = 0; i < tamanhoP2; i++) {
		if (P2[i].cadeia_simbolos.size() >= 2) {
			for (int j = 0; j < P2[i].cadeia_simbolos.size(); j++) {
				if (encontraTerminal(P2[i].cadeia_simbolos[j], G.terminais)) {
					std::string novaVariavel = "T" + P2[i].cadeia_simbolos[j];
					V2.push_back(novaVariavel);
					REGRA novaRegra;
					novaRegra.variavel = novaVariavel;
					novaRegra.cadeia_simbolos.push_back(P2[i].cadeia_simbolos[j]);
					if (!encontraProducao(novaRegra, P2))
						P2.push_back(novaRegra);
					P2[i].cadeia_simbolos[j] = novaVariavel;
				}
			}
		}
	}

	G.variaveis = V2;
	G.regras = P2;
}

// -----------------------------------------------------------------------------

void transformaMaiorTres(GRAMATICA &G) {
	
}