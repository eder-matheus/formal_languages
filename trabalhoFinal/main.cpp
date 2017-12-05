#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "simplificacao.h"
#include "fnc.h"
#include "early.h"

using namespace std;

int main(int argc, char** argv) {
	bool continuar;
	int operacao;
	int sucesso;

	do {
		GRAMATICA g; // Gramatica original
		GRAMATICA g1; // Gramatica simplificada
		GRAMATICA g2; // Gramatica na forma normal
		GRAMATICA g3; // Gramatica usada para o algoritmo de Early

		std::string arquivo;
		std::string entrada;
		std::cout << "Escreva o nome do arquivo que contém a gramatica (com a extensao do arquivo):\n";
		std::cout << "Nome do arquivo: ";
		std::cin >> arquivo;

		sucesso = le_gramatica(arquivo, g);
		std::cout << "\n";

		if (sucesso) {
			std::cout << "Escolha a operação a ser feita sobre a gramatica lida:\n";
			do {
				std::cout << "|1 - Mostrar Original| |2 -  Simplificacao| |3 - FNC | |4 - Early| |5 - Sair|\n";
				std::cout << "Operacao: ";
				std::cin >> operacao;
				std::cout << "\n";
				if (operacao < 1 || operacao > 5) {
					std::cout << "Operacao invalida!!!\n";
					continue;
				}
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
						std::cout << "Algoritmo de Early\n";
						std::cout << "\n---- Digite a senteca a ser analisada: ";
						std::cin >> entrada;
						std::cout << "\n";
						simplificaGramatica(g, g3);
						fnc(g3);
						early(entrada, g3);
						break;
					case 5:
						std::cout << "Saindo.......\n";
						break;
				}
			} while (operacao != 5);
		}

		std::cout << "Abrir outro arquivo?\n |1 - Sim| |0 - Nao|\n";
		std::cin >> continuar;
	} while (continuar);

	return 0;
}