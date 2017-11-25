#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "simplificacao.h"
#include "fnc.h"

using namespace std;

int main(int argc, char** argv) {
	bool continuar;
	int sucesso;

	do {
		GRAMATICA g;
		GRAMATICA g1;
		GRAMATICA g2;
		std::string arquivo;
		std::cout << "Escreva o nome do arquivo que contém a gramatica (com a extensao do arquivo):\n";
		std::cout << "Nome do arquivo: ";
		std::cin >> arquivo;

		sucesso = le_gramatica(arquivo, g);
		std::cout << "\n";		

		if (sucesso) {
			imprimeGramatica(g);
			//removeProducoesVazias(g, g1);
			//simplificaGramatica(g, g1, g2);
			//removeSimbolosInuteis(g, g1);
			transformaMaiorDois(g);
			imprimeGramatica(g);	
		}
		
		std::cout << "Abrir outro arquivo?\n |1 - Sim| |0 - Nao|\n";
		std::cin >> continuar;
	} while (continuar);

	return 0;
}