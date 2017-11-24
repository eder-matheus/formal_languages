#include "gramatica.h"

int le_gramatica(std::string arquivo, GRAMATICA& gramatica) {
	std::ifstream entrada;

	entrada.open(arquivo);

	if (!entrada.is_open()) {
		std::cout << "Arquivo não encontrado!!!\n";
		return 0;
	}

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
		
		if (linha.find("#", 0) == 0)
			continue;

		if (leitura_atual.compare("terminais") == 0) {
			if (linha.find("[", 0) == -1) // Verifica se a linha é vazia ou se não possui elementos da gramatica
				continue;
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				terminal.push_back(linha[i]);
			}
			gramatica.terminais.push_back(terminal);
			terminal.erase();
		}

		if (leitura_atual.compare("variaveis") == 0) {
			if (linha.find("[", 0) == -1) // Verifica se a linha é vazia ou se não possui elementos da gramatica
				continue;
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				variavel.push_back(linha[i]);
			}
			gramatica.variaveis.push_back(variavel);
			variavel.erase();
		}

		if (leitura_atual.compare("inicial") == 0) {
			if (linha.find("[", 0) == -1) // Verifica se a linha é vazia ou se não possui elementos da gramatica
				continue;
			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0) {
					break;
				}
				inicial.push_back(linha[i]);
			}
			gramatica.inicial = inicial;
			inicial.erase();
		}

		if (leitura_atual.compare("regras") == 0) {
			if (linha.find("[", 0) == -1) // Verifica se a linha é vazia ou se não possui elementos da gramatica
				continue;

			for (int i = linha.find("[", 0) + 2; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0)
					break;
				regra_var.push_back(linha[i]);
			}

			std::vector<std::string> producao;

			contador = linha.find(">", 0) + 1;
			for (int i = contador; i < linha.size(); i++) {
				std::string caractere;
				caractere.push_back(linha[i]);
				if (caractere.compare(" ") == 0 || caractere.compare("]") == 0 || caractere.compare("[") == 0) {
					if (regra_prod.size() != 0)
						producao.push_back(regra_prod);
					regra_prod.erase();
					continue;
				}
				if (caractere.compare("#") == 0)
					break;

				regra_prod.push_back(linha[i]);
			}
			regra_prod.erase();

			REGRA regra;

			regra.variavel = regra_var;
			regra.cadeia_simbolos = producao;
			gramatica.regras.push_back(regra);
			regra_var.erase();
		}
	}

	entrada.close();
	return 1;
}

// -----------------------------------------------------------------------------

bool encontraVariavel(std::string const &variavel, std::vector<std::string> const &variaveis) {
	for (int i = 0; i < variaveis.size(); i++)
		if (variavel.compare(variaveis[i]) == 0)
			return true;
	return false;
}

// -----------------------------------------------------------------------------

bool encontraTerminal(std::string const &terminal, std::vector<std::string> const &terminais) {
	for (int i = 0; i < terminais.size(); i++)
		if (terminal.compare(terminais[i]) == 0)
			return true;
	return false;
}

// -----------------------------------------------------------------------------

bool encontraProducao(REGRA const &regra, std::vector<REGRA> const &regras) {
	int encontra = 0;
	
	for (int i = 0; i < regras.size(); i++) {
		if (regra.variavel.compare(regras[i].variavel) == 0) {
			encontra = 1;
			if (regra.cadeia_simbolos.size() != regras[i].cadeia_simbolos.size()) {
				encontra = 0;
			} else {
				for (int j = 0; j < regras[i].cadeia_simbolos.size(); j++) {
					if (regra.cadeia_simbolos[j].compare(regras[i].cadeia_simbolos[j]) != 0)
						encontra = 0;
				}
			}
		} else {
			encontra = 0;
		}
		
		if (encontra == 1)
			return true;
	}

	return false;
}

// -----------------------------------------------------------------------------

void imprimeGramatica(GRAMATICA const &g) {
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

// -----------------------------------------------------------------------------



















