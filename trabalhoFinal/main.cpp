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
		GRAMATICA g1;
		std::string arquivo;
		std::cout << "Escreva o nome do arquivo que contém a gramatica (com a extensao do arquivo):\n";
		std::cout << "Nome do arquivo: ";
		std::cin >> arquivo;

		sucesso = le_gramatica(arquivo, g);
		removeProducoesVazias(g, g1);
		//removeSimbolosInuteis(g1, g);
		

		if (sucesso) {
			imprimeGramatica(g);
			//removeProducoesUnitarias(g, g1);
		}
		
		std::cout << "Abrir outro arquivo?\n |1 - Sim| |0 - Nao|\n";
		std::cin >> continuar;
	} while (continuar);

	return 0;
}