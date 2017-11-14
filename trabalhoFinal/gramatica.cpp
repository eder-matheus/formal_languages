#include "gramatica.h"

void le_gramatica(std::string arquivo, GRAMATICA& gramatica) {
	std::ifstream entrada;

	entrada.open(arquivo);

	std::string linha;
	std::string leitura_atual;
	std::string terminal;
	std::string variavel;
	std::string inicial;
	std::string regra_var;
	std::string regra_prod;

	int contador;

	while (!entrada.eof()) {
		std::getline(entrada, linha);

		if (entrada.eof()) {
			continue;
		}

		if (linha.find("#Terminais", 0) == 0) {
			leitura_atual = "terminais";
			continue;
		}

		if (linha.find("#Variaveis", 0) == 0) {
			leitura_atual = "variaveis";
			continue;
		}

		if (linha.find("#Inicial", 0) == 0) {
			leitura_atual = "inicial";
			continue;
		}

		if (linha.find("#Regras", 0) == 0) {
			leitura_atual = "regras";
			continue;
		}

		if (leitura_atual.compare("terminais") == 0) {
			terminal = linha[linha.find("[", 0) + 2];
			std::cout << terminal << "\n";
		}

		if (leitura_atual.compare("variaveis") == 0) {
			variavel = linha[linha.find("[", 0) + 2];
			std::cout << variavel << "\n";
		}

		if (leitura_atual.compare("inicial") == 0) {
			inicial = linha[linha.find("[", 0) + 2];
			std::cout << inicial << "\n";
		}

		if (leitura_atual.compare("regras") == 0) {
			regra_var = linha[linha.find("[", 0) + 2];
			std::cout << regra_var << "\n";
		}
	}

	entrada.close();
}