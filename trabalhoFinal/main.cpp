#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "regras.h"
#include "gramatica.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	bool continuar;
	int sucesso;

	do {
		GRAMATICA g;
		std::string arquivo;
		std::cout << "Escreva o nome do arquivo que contém a gramatica (com a extensao do arquivo):\n";
		std::cout << "Nome do arquivo: ";
		std::cin >> arquivo;

		sucesso = le_gramatica(arquivo, g);

		if (sucesso) {
			std::cout << "G = (";
			
			//Variaveis
			std::cout << "{ ";
			for (int i = 0; i < g.variaveis.size(); i++)
				std::cout << "[ " << g.variaveis[i] << " ] ";
			std::cout << "}, ";

			//Terminais
			std::cout << "{ ";
			for (int i = 0; i < g.terminais.size(); i++)
				std::cout << "[ " << g.terminais[i] << " ] ";
			std::cout << "}, ";

			std::cout << "P, ";

			//Simbolo inicial
			std::cout << g.inicial << ")\n";

			std::cout << "onde P:\n";

			//Regras de producao
			for (int i = 0; i < g.regras.size(); i++) {
				std::cout << g.regras[i].variavel << " -> ";
				for (int j = 0; j < g.regras[i].cadeia_simbolos.size(); j++)
					std::cout << "'" << g.regras[i].cadeia_simbolos[j] << "' ";
				std::cout << "\n";
			}
		}

		std::cout << "Abrir outro arquivo?\n |1 - Sim| |0 - Nao|\n";
		std::cin >> continuar;
	} while (continuar);

	return 0;
}