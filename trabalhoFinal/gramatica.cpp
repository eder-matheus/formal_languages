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

void removeProducoesUnitarias(GRAMATICA const &G, GRAMATICA &G1){
	// fecho transitivo das variaveis
	std::vector<FECHO> fechos;
	std::vector<std::string> variaveis_inclusas;
	FECHO temp;
	
	for(int i = 0; i < G.regras.size(); i++){ //inclui todas as variávis na lista de fechos
		if(!encontraVariavel(G.regras[i].variavel, variaveis_inclusas)){
			temp.variavel = G.regras[i].variavel;
			fechos.push_back(temp);
			variaveis_inclusas.push_back(G.regras[i].variavel);
		}
	}
	/*	
	for(int k = 0; k < variaveis_inclusas.size(); k++){
		std::cout << variaveis_inclusas[k] + "\n";	
	}		
	*/
	
	for(int i = 0; i < variaveis_inclusas.size(); i++){
		for(int j = 0; j < G.regras.size(); j++){
			if((variaveis_inclusas[i] == G.regras[j].variavel) 
			&& (G.regras[j].cadeia_simbolos.size() < 2) 
			&& (encontraVariavel(G.regras[j].cadeia_simbolos[0], G.variaveis))){
				fechos[i].cadeia_simbolos.push_back(G.regras[j].cadeia_simbolos[0]);
			}
		}
	}
/*
	std::cout << "Fecho de cada variável: \n";
	for(int i = 0; i < variaveis_inclusas.size(); i++){		
		std::cout << fechos[i].variavel + " = { ";
		for(int j = 0; j < fechos[i].cadeia_simbolos.size(); j++){
			std::cout << fechos[i].cadeia_simbolos[j] + ", ";
		}
		std::cout << "}\n";
	}	
*/	
	int dimensao_fecho_atual = 0, dimensao_fecho_anterior = 0;
	do{
		dimensao_fecho_anterior = dimensao_fecho_atual;
		for(int i = 0; i < variaveis_inclusas.size(); i++){// para cada variavel da gramatica
			for(int j = 0; j < fechos[i].cadeia_simbolos.size(); j++){ //percorre todas as variaveis no seu fecho
				for(int k = 0; k < variaveis_inclusas.size(); k++){ 
					if(fechos[i].cadeia_simbolos[j] == fechos[k].variavel){
						for(int l = 0; l < fechos[k].cadeia_simbolos.size(); l++){
							if(!encontraVariavel(fechos[k].cadeia_simbolos[l], fechos[i].cadeia_simbolos)
								&& fechos[i].variavel != fechos[k].cadeia_simbolos[l]){
								fechos[i].cadeia_simbolos.push_back(fechos[k].cadeia_simbolos[l]);
								dimensao_fecho_atual++;
							}
						}
					}
				}
			}
		}
	}while(dimensao_fecho_atual != dimensao_fecho_anterior);
/*	
	std::cout << "Fecho de cada variável(final): \n";
	for(int i = 0; i < variaveis_inclusas.size(); i++){		
		std::cout << fechos[i].variavel + " = { ";
		for(int j = 0; j < fechos[i].cadeia_simbolos.size(); j++){
			std::cout << fechos[i].cadeia_simbolos[j] + ", ";
		}
		std::cout << "}\n";
	}	
*/
}




void imprimeGramatica(GRAMATICA const &g){
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

























