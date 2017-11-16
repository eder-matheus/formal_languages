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

bool encontraVariavel(std::string const &variavel, std::vector<std::string> const &variaveis) {
	for (int i = 0; i < variaveis.size(); i++)
		if (variavel.compare(variaveis[i]) == 0)
			return true;
	return false;
}

bool encontraTerminal(std::string const &terminal, std::vector<std::string> const &terminais) {
	for (int i = 0; i < terminais.size(); i++)
		if (terminal.compare(terminais[i]) == 0)
			return true;
	return false;
}

void removeProducoesVazias(GRAMATICA const &G, GRAMATICA &G1) {
	std::vector<std::string> prodVazias;
	std::vector<REGRA> p1;

	for (int i = 0; i < G.regras.size(); i++) { // Encontra as variaveis que produzem vazio diretamente
		if (G.regras[i].cadeia_simbolos[0].compare("V") == 0)
			if (!encontraVariavel(G.regras[i].variavel, prodVazias))
				prodVazias.push_back(G.regras[i].variavel);
	}

	for (int i = 0; i < G.regras.size(); i++) { // Encontra as variaveis que produzem vazio indiretamente
		for (int j = 0; j < G.regras[i].cadeia_simbolos.size(); j++)
			if (encontraVariavel(G.regras[i].cadeia_simbolos[j], prodVazias))
				if (!encontraVariavel(G.regras[i].variavel, prodVazias))
					prodVazias.push_back(G.regras[i].variavel);
	}

	for (int i = 0; i < G.regras.size(); i++)
		if (G.regras[i].cadeia_simbolos[0].compare("V") != 0)
			p1.push_back(G.regras[i]);

	
}

void removeSimbolosInuteis(GRAMATICA const &G, GRAMATICA &G1) {
	for (int i = 0; i < G.regras.size(); i++) { // Etapa 1: qualquer variavel gera terminais
		for (int j = 0; j < G.regras[i].cadeia_simbolos.size(); j++) {
			if (encontraTerminal(G.regras[i].cadeia_simbolos[j], G.terminais) ||
				encontraVariavel(G.regras[i].cadeia_simbolos[j], G1.variaveis)) {
				if (!encontraVariavel(G.regras[i].variavel, G1.variaveis))
					G1.variaveis.push_back(G.regras[i].variavel);
				G1.regras.push_back(G.regras[i]);
			}
		}
	}


}




























