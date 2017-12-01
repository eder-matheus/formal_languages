#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "simplificacao.h"
#include "fnc.h"
#include "earley.h"

using namespace std;

int main(int argc, char** argv) {
	bool continuar;
	int operacao;
	int sucesso;

	do {
		GRAMATICA g; // Gramatica original
		GRAMATICA g1; // Gramatica simplificada
		GRAMATICA g2; // Gramatica na forma normal

		std::string arquivo;
		std::cout << "Escreva o nome do arquivo que contém a gramatica (com a extensao do arquivo):\n";
		std::cout << "Nome do arquivo: ";
		std::cin >> arquivo;

		sucesso = le_gramatica(arquivo, g);
		std::cout << "\n";

		if (sucesso) {
			std::cout << "Escolha a operação a ser feita sobre a gramatica lida:\n";
			do {
				std::cout << "|1 - Mostrar Original| |2 -  Simplificacao| |3 - FNC | |4 - Sair|\n";
				std::cout << "Operacao: ";
				std::cin >> operacao;
				std::cout << "\n";
				switch (operacao) {
					case 1:
						std::cout << "Gramatica Original\n";
						imprimeGramatica(g);
						break;
					case 2:
						std::cout << "Simplificacao\n";
						simplificaGramatica(g, g1);
						imprimeGramatica(g1);
						break;
					case 3:
						std::cout << "Forma Normal de Chomsky\n";
						simplificaGramatica(g, g2);
						fnc(g2);
						imprimeGramatica(g2);
						break;
					case 4:
						std::cout << "Saindo.......\n";
						break;
					default:
						std::cout << "Operacao invalida!!!\n";
						break;
				}
			} while (operacao != 4);
		}

		std::cout << "Algoritmo de Early - Etapa 1:\n";
		std::vector<REGRA> D0;
		std::vector<std::vector<REGRA>> Dr;
		std::string entrada = "aba abbba abbbb";
		std::vector<std::string> sentenca;
		constroiD0(g2, D0);
		Dr.push_back(D0);
		
		
		analisaSentenca(entrada, g2, sentenca);
		
		
		constroiDr(g2, Dr, sentenca);

		std::cout << "Abrir outro arquivo?\n |1 - Sim| |0 - Nao|\n";
		std::cin >> continuar;
	} while (continuar);

	return 0;
}