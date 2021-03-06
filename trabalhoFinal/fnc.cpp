#include <stdio.h>
#include <stdlib.h>
#include "fnc.h"

void transformaMaiorDois(GRAMATICA &G) {
	std::vector<std::string> V2 = G.variaveis;
	std::vector<REGRA> P2 = G.regras;
	int tamanhoP2 = P2.size();

	for (int i = 0; i < tamanhoP2; i++) { // Passa por cada producao verificando se existe uma com mais de dois simbolos
		if (P2[i].cadeia_simbolos.size() >= 2) {
			for (int j = 0; j < P2[i].cadeia_simbolos.size(); j++) { // Se existir, verifica cada simbolo procurando por um terminal
				if (encontraTerminal(P2[i].cadeia_simbolos[j], G.terminais)) { // Se encontrar algum terminal:
					std::string novaVariavel = "T" + P2[i].cadeia_simbolos[j]; // Cria uma nova variavel, que vai produzir o terminal encontrado
					if (!encontraVariavel(novaVariavel, V2)) // Se essa variavel ainda nao foi criada, adiciona a V2
						V2.push_back(novaVariavel);
					REGRA novaRegra; // Variavel novaRegra contem a producao adicional que da origem ao terminal encontrado
					novaRegra.variavel = novaVariavel;
					novaRegra.cadeia_simbolos.push_back(P2[i].cadeia_simbolos[j]);
					if (!encontraProducao(novaRegra, P2)) // Se essa regra de producao ainda nao tiver sido criada, adiciona a P2
						P2.push_back(novaRegra);
					P2[i].cadeia_simbolos[j] = novaVariavel; // Troca o terminal encontrado na producao pela variavel adicional criada anteriormente
				}
			}
		}
	}

	G.variaveis = V2;
	G.regras = P2;
}

// -----------------------------------------------------------------------------

void transformaMaiorTres(GRAMATICA &G) {
	std::vector<std::string> V3 = G.variaveis;
	std::vector<REGRA> P3 = G.regras;
	int i = 0;
	int numVar = 1;

	do { // Passa por todas as producoes verificando se ela tem mais de tres simbolos
		if (P3[i].cadeia_simbolos.size() >= 3) {
			REGRA regra; // Variavel regra usada para guardar a regra que vai ser modificada
			regra.variavel = P3[i].variavel;
			regra.cadeia_simbolos = P3[i].cadeia_simbolos;

			P3.erase(P3.begin() + i); // Exclui a regra a ser modificada
			i--;

			std::string buffer = std::to_string(numVar);
			std::string novaVariavel = "V" + buffer; // Nomenclatura da variavel adicional 

			std::vector<std::string> novaProducao;
			for (int j = 1; j < regra.cadeia_simbolos.size(); j++) {
				novaProducao.push_back(regra.cadeia_simbolos[j]);
			} // Criacao da producao adicional

			REGRA novaRegra; // Variavel novaRegra usada para guardar a producao adicional criada pelo algoritmo
			novaRegra.variavel = novaVariavel;
			novaRegra.cadeia_simbolos = novaProducao;

			regra.cadeia_simbolos.erase(regra.cadeia_simbolos.begin() + 1, regra.cadeia_simbolos.end());
			regra.cadeia_simbolos.push_back(novaVariavel); // Alteracao da regra original para ela conter somente duas variaveis no seu corpo

			P3.push_back(regra);
			P3.push_back(novaRegra);
			V3.push_back(novaVariavel);

			numVar++;
		}

		i++;
	} while (i < P3.size());

	G.regras = P3;
	G.variaveis = V3;
}

void fnc(GRAMATICA &G) {
	transformaMaiorDois(G);
	transformaMaiorTres(G);
}

// -----------------------------------------------------------------------------