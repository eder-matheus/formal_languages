#include "simplificacao.h"

void removeProducoesVazias(GRAMATICA const &G, GRAMATICA &G1) {
	std::vector<std::string> varProdVazias;
	std::vector<REGRA> p1;

	for (int i = 0; i < G.regras.size(); i++) { // Encontra as variaveis que produzem vazio diretamente
		if (G.regras[i].cadeia_simbolos[0].compare("V") == 0)
			if (!encontraVariavel(G.regras[i].variavel, varProdVazias))
				varProdVazias.push_back(G.regras[i].variavel);
	}

	for (int i = 0; i < G.regras.size(); i++) { // Encontra as variaveis que produzem vazio indiretamente
		if (encontraVariavel(G.regras[i].cadeia_simbolos[0], varProdVazias) && G.regras[i].cadeia_simbolos.size() == 1)
			if (!encontraVariavel(G.regras[i].variavel, varProdVazias))
				varProdVazias.push_back(G.regras[i].variavel);
	}

	for (int i = 0; i < G.regras.size(); i++) // Exclusao de producoes vazias
		if (G.regras[i].cadeia_simbolos[0].compare("V") != 0) {
			p1.push_back(G.regras[i]);
		}

	int i = 0;
	REGRA novaRegra;

	// Producoes adicionais que simulam producoes vazias
	int p1_tamanho_atual = 0, p1_tam_anterior = 0;
	do { // percorre o vetor até  seu final. Esse final muda a cada nova produção inserida
		p1_tam_anterior = p1_tamanho_atual;
		for (int j = 0; j < p1[i].cadeia_simbolos.size(); j++) {//
			if (encontraTerminal(p1[i].cadeia_simbolos[j], varProdVazias)) {
				std::vector<std::string> novaProducao;
				for (int k = 0; k < p1[i].cadeia_simbolos.size(); k++){
					if (k != j){
						novaProducao.push_back(p1[i].cadeia_simbolos[k]);

					}
				}
				novaRegra.variavel = p1[i].variavel;
				novaRegra.cadeia_simbolos = novaProducao;
				p1_tamanho_atual++;
				/*------------------
				std::cout << "hi \n";
				std::cout << novaRegra.variavel << " -> ";
				for (int j = 0; j < novaRegra.cadeia_simbolos.size(); j++)
					std::cout << "'" << novaRegra.cadeia_simbolos[j] << "' ";
				std::cout << "\n";
				//-------------------*/
				if (novaProducao.size() > 0 && (!encontraProducao(novaRegra, p1))) {
					p1.push_back(novaRegra);
				}
			}
		}
		i++;
	} while (i < p1.size());
	
	//removeProducoesRepetidas(p1);

	G1.inicial = G.inicial;
	G1.terminais = G.terminais;
	G1.variaveis = G.variaveis;
	G1.regras = p1;
}

// -----------------------------------------------------------------------------

void removeSimbolosInuteis(GRAMATICA const &G, GRAMATICA &G1) {
	for (int i = 0; i < G.regras.size(); i++) { // Etapa 1: qualquer variavel gera terminais
		for (int j = 0; j < G.regras[i].cadeia_simbolos.size(); j++) {
			if (encontraTerminal(G.regras[i].cadeia_simbolos[j], G.terminais) ||
				encontraVariavel(G.regras[i].cadeia_simbolos[j], G1.variaveis)) {
				if (!encontraVariavel(G.regras[i].variavel, G1.variaveis))
					G1.variaveis.push_back(G.regras[i].variavel);
				if(!encontraProducao(G.regras[i], G1.regras))
					G1.regras.push_back(G.regras[i]);
			}
		}
	}
	
	//p1 = G1.regras
	//Etapa 2: qualquer símbolo é atingível a partir do símbolo inicial
	std::vector<std::string> t2;
	std::vector<std::string> v2;
	v2.push_back(G.inicial);
	
	int l = 0;
	do{
		for(int i = 0; i < v2.size(); i++){
			for(int j = 0; j < G1.regras.size(); j++){
				if(G1.regras[j].variavel == v2[i]){
					for(int k = 0; k < G1.regras[j].cadeia_simbolos.size(); k++){
						if(!encontraVariavel(G1.regras[j].cadeia_simbolos[k], v2) 
							&& encontraVariavel(G1.regras[j].cadeia_simbolos[k], G1.variaveis)){
							v2.push_back(G1.regras[j].cadeia_simbolos[k]);
						}
						if(!encontraTerminal(G1.regras[j].cadeia_simbolos[k], t2)
							&& encontraTerminal(G1.regras[j].cadeia_simbolos[k], G.terminais)){
							t2.push_back(G1.regras[j].cadeia_simbolos[k]);
						}	
					}
				}
			}
		}	
		l++;
	}while(l < t2.size() || l < v2.size());
	
	std::cout << "v2:";
	for(int i = 0; i < v2.size(); i++){
		std::cout << v2[i];
	}
	std::cout << "\n";
	std::cout << "t2:";
	for(int i = 0; i < t2.size(); i++){
		std::cout << t2[i];
	}
	std::cout << "\n";
	
	//p2 contem apenas produções que referenciam v2 e t2
	std::vector<REGRA> p2;
	int producao_valida = 1;
	for(int i = 0; i < G1.regras.size(); i++){
		producao_valida = 1;
		for(int j = 0; j < G1.regras[i].cadeia_simbolos.size(); j++){
			if((!encontraVariavel(G1.regras[i].cadeia_simbolos[j], v2) 
				&& !encontraTerminal(G1.regras[i].cadeia_simbolos[j], t2))
				|| !encontraVariavel(G1.regras[i].variavel, v2)){
				producao_valida = 0;
			}
		}
		if(producao_valida == 1 && !encontraProducao(G1.regras[i], p2)) 
			p2.push_back(G1.regras[i]);
	}
	G1.regras = p2;
	G1.terminais = t2;
}

// -----------------------------------------------------------------------------

void removeProducoesUnitarias(GRAMATICA const &G, GRAMATICA &G1) {
	// fecho transitivo das variaveis
	std::vector<FECHO> fechos;
	std::vector<std::string> variaveis_inclusas;
	FECHO temp;

	for (int i = 0; i < G.regras.size(); i++) { //inclui todas as variávis na lista de fechos
		if (!encontraVariavel(G.regras[i].variavel, variaveis_inclusas)) {
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

	for (int i = 0; i < variaveis_inclusas.size(); i++) {
		for (int j = 0; j < G.regras.size(); j++) {
			if ((variaveis_inclusas[i] == G.regras[j].variavel)
				&& (G.regras[j].cadeia_simbolos.size() < 2)
				&& (encontraVariavel(G.regras[j].cadeia_simbolos[0], G.variaveis))) {
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
	do {
		dimensao_fecho_anterior = dimensao_fecho_atual;
		for (int i = 0; i < variaveis_inclusas.size(); i++) {// para cada variavel da gramatica
			for (int j = 0; j < fechos[i].cadeia_simbolos.size(); j++) { //percorre todas as variaveis no seu fecho
				for (int k = 0; k < variaveis_inclusas.size(); k++) {
					if (fechos[i].cadeia_simbolos[j] == fechos[k].variavel) {
						for (int l = 0; l < fechos[k].cadeia_simbolos.size(); l++) {
							if (!encontraVariavel(fechos[k].cadeia_simbolos[l], fechos[i].cadeia_simbolos)
								&& fechos[i].variavel != fechos[k].cadeia_simbolos[l]) {
								fechos[i].cadeia_simbolos.push_back(fechos[k].cadeia_simbolos[l]);
								dimensao_fecho_atual++;
							}
						}
					}
				}
			}
		}
	} while (dimensao_fecho_atual != dimensao_fecho_anterior);
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

	std::vector <REGRA> P1;
	REGRA regra_temp;

	for (int i = 0; i < G.regras.size(); i++) {
		if (G.regras[i].cadeia_simbolos.size() >= 2 || encontraTerminal(G.regras[i].cadeia_simbolos[0], G.terminais)) {
			P1.push_back(G.regras[i]);
		}
	}
	
	for (int i = 0; i < G.variaveis.size(); i++) {
		for (int j = 0; j < fechos[i].cadeia_simbolos.size(); j++) {
			for (int k = 0; k < G.regras.size(); k++) {
				if (G.regras[k].variavel == fechos[i].cadeia_simbolos[j]) {
					if (G.regras[k].cadeia_simbolos.size() >= 2 || encontraTerminal(G.regras[k].cadeia_simbolos[0], G.terminais)) {
						regra_temp.variavel = fechos[i].variavel;
						regra_temp.cadeia_simbolos = G.regras[k].cadeia_simbolos;
						if (!encontraProducao(regra_temp, P1))
							P1.push_back(regra_temp);
					}
				}
			}
		}
	}
	
	G1.regras = P1;


}

// -----------------------------------------------------------------------------

void simplificaGramatica(GRAMATICA const &G, GRAMATICA &G1, GRAMATICA &G2) {
	removeProducoesVazias(G, G1);
	removeProducoesUnitarias(G1, G1);
	removeSimbolosInuteis(G1, G2);
}
