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
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				terminal.push_back(linha[i]);
			}
			std::cout << terminal << "\n";
			terminal.erase();
		}

		if (leitura_atual.compare("variaveis") == 0) {
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				variavel.push_back(linha[i]);
			}
			std::cout << variavel << "\n";
			variavel.erase();
		}

		if (leitura_atual.compare("inicial") == 0) {
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				inicial.push_back(linha[i]);
			}
			std::cout << inicial << "\n";
			inicial.erase();
		}

		if (leitura_atual.compare("regras") == 0) {
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				regra_var.push_back(linha[i]);
			}
			regra_var = linha[linha.find("[", 0) + 2];
			std::cout << regra_var << "\n";
		}
	}

	entrada.close();
}