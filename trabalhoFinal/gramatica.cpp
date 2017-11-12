#include "gramatica.h"

void le_gramatica(std::string arquivo, GRAMATICA& gramatica) {
	std::ifstream entrada;

	entrada.open(arquivo);

	for (std::string linha; std::getline(entrada, linha);) {
		std::cout << linha << "\n";
	}

	entrada.close();
}