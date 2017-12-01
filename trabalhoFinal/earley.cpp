#include "earley.h"

void analisaSentenca(std::string const &sentenca, GRAMATICA const &G, std::vector<std::string> &terminaisSentenca) {
	int i = 0;
	int simboloNaoPertence = 0;
	std::string potencialTerminal;
	std::vector<std::string> potenciaisTerminais;

	do {
		while ((int) sentenca[i] != 32 && i < sentenca.size()) {
			std::string simbolo;
			simbolo.push_back(sentenca[i]);
			if (!encontraTerminal(simbolo, G.terminais))
				simboloNaoPertence = 1;
			potencialTerminal.push_back(sentenca[i]);
			potenciaisTerminais.push_back(simbolo);
			i++;
		}

		if (encontraTerminal(potencialTerminal, G.terminais)) {
			terminaisSentenca.push_back(potencialTerminal);
		} else {
			if (!simboloNaoPertence) {
				for (int k = 0; k < potenciaisTerminais.size(); k++) {
					terminaisSentenca.push_back(potenciaisTerminais[k]);
				}
			}
		}

		potenciaisTerminais.clear();
		potencialTerminal.clear();
		i++;
	} while (i < sentenca.size());
}

// -----------------------------------------------------------------------------

int encontraMarcador(std::vector<std::string> const &producao) {
	int i = 0;

	while (i < producao.size() && producao[i].compare("*") != 0)
		i++;

	if (i < producao.size())
		return i; // Retorna o indice do marcador numa producao
	else
		return -1;
}

// -----------------------------------------------------------------------------

void avancaMarcador(std::vector<std::string> &producao, int conjunto) {
	int marcador = encontraMarcador(producao);
	std::string cjto = producao.back();

	if (marcador != -1) {
		producao[marcador] = producao[marcador + 1];
		producao[marcador + 1] = "*";
	} else {
		std::vector<std::string> producaoAux;

		producaoAux = producao;
		producao.clear();
		producao.push_back("*");
		for (int k = 0; k < producaoAux.size(); k++) {
			producao.push_back(producaoAux[k]);
		}
		if (conjunto != -1) {
			std::string s = "/" + std::to_string(conjunto);
			producao.push_back(s);
		} else {
			producao.push_back(cjto);
		}
	}
}

// -----------------------------------------------------------------------------

void constroiD0(GRAMATICA const &G, std::vector<REGRA> &D0) {
	//std::vector<std::string> producaoAux;
	D0 = {};

	for (int i = 0; i < G.regras.size(); i++) { // Adiciona a D0 todas as producoes de S
		if (G.regras[i].variavel.compare(G.inicial) == 0) {
			D0.push_back(G.regras[i]);
		}
	}

	int j = 0;

	do {
		for (int i = 0; i < G.regras.size(); i++) {
			if (D0[j].cadeia_simbolos[0].compare(G.regras[i].variavel) == 0) {
				if (!encontraProducao(G.regras[i], D0)) {
					D0.push_back(G.regras[i]);
				}
			}
		}
		j++;
	} while (j < D0.size());

	for (int i = 0; i < D0.size(); i++) {
		avancaMarcador(D0[i].cadeia_simbolos, 0);
	}
}

// -----------------------------------------------------------------------------

void constroiDr(GRAMATICA const &G, std::vector<std::vector<REGRA>> &Dr, std::vector<std::string> const &sentenca) {
	int simboloMarcado;
	std::vector<REGRA> Daux, Daux2;
	std::cout << "Tamanho sentenca: " << sentenca.size() << "\n";

	for (int i = 1; i < sentenca.size(); i++) {
		for (int j = 0; j < Dr[i - 1].size(); j++) { // Producoes que geram o simbolo ar
			simboloMarcado = encontraMarcador(Dr[i - 1][j].cadeia_simbolos) + 1;
			if (Dr[i - 1][j].cadeia_simbolos[simboloMarcado].compare(sentenca[i - 1]) == 0) {
				Daux.push_back(Dr[i - 1][j]);
			}
		}

		for (int k = 0; k < Daux.size(); k++) {
			avancaMarcador(Daux[k].cadeia_simbolos, -1);
		}

		Dr.push_back(Daux);
		Daux.clear();

		int sizeAux = 0;

		do {
			for (int k = 0; k < Dr[i].size(); k++) {
				simboloMarcado = encontraMarcador(Dr[i][k].cadeia_simbolos) + 1;
				for (int l = 0; l < G.regras.size(); l++) { // Producoes que podem gerar o proximo simbolo
					if (Dr[i][k].cadeia_simbolos[simboloMarcado].compare(G.regras[l].variavel) == 0) {
						if (!encontraProducao(G.regras[l], Daux) && !encontraProducao(G.regras[l], Daux2)) {
							Daux2.push_back(G.regras[l]);
							//avancaMarcador(Dr[i].back().cadeia_simbolos);
						}
					}
				}

				std::cout << "Inicio\n";
				for (int l = 0; l < Daux2.size(); l++) { // Coloca o marcador nas producoes adicionadas a Dr que vieram das producoes originais
					std::cout << "Ok1\n";
					std::cout << "Producao com marcador sendo movido: " << Daux2[l].variavel << " -> ";
					for (int z = 0; z < Daux2[l].cadeia_simbolos.size(); z++)
						std::cout << Daux2[l].cadeia_simbolos[z] << " ";
					std::cout << "\n";
					avancaMarcador(Daux2[l].cadeia_simbolos, i);
					std::cout << "Ok2\n";
					Dr[i].push_back(Daux2[l]);
				}
			}
			std::cout << "Inicio2\n";

			for (int k = 0; k < Dr[i].size(); k++) { // Uma subpalavra w foi reduzida a variavel A
				simboloMarcado = encontraMarcador(Dr[i][k].cadeia_simbolos) + 1;
				if ((int) Dr[i][k].cadeia_simbolos[simboloMarcado][0] == 47) {
					std::string conjunto;
					for (int l = 1; l < Dr[i][k].cadeia_simbolos[simboloMarcado].size(); l++) {
						conjunto.push_back(Dr[i][k].cadeia_simbolos[simboloMarcado][l]);
					}

					int cjto = 0;
					for (int l = 0; l < conjunto.size(); l++) { // Identifica o conjunto de producoes cujo a producao esta inserida
						cjto += (int) conjunto[conjunto.size() - 1 - l] - 48;
					}

					for (int l = 0; l < Dr[cjto].size(); l++) {
						simboloMarcado = encontraMarcador(Dr[cjto][l].cadeia_simbolos) + 1;
						if (Dr[i][k].variavel.compare(Dr[cjto][l].cadeia_simbolos[simboloMarcado]) == 0) {
							if (!encontraProducao(Dr[cjto][l], Daux2) && !encontraProducao(Dr[cjto][l], Daux))
								Daux2.push_back(Dr[cjto][l]);
						}
					}
				}
			}

			std::cout << "Loop " << sizeAux << "\n";
			std::cout << "Tamanho de Dr[i]: " << Dr[i].size() << "\n";
			sizeAux++;
		} while (sizeAux < Dr[i].size());

		std::cout << "Saiu das comparacoes\n";
		
		for (int k = 0; k < Daux2.size(); k++)
			avancaMarcador(Daux2[k].cadeia_simbolos, i);
	}




	std::cout << "Dr size: " << Dr.size() << "\n";
	for (int k = 0; k < 2; k++) {
		std::cout << "D" << k << "\n";
		for (int i = 0; i < Dr[k].size(); i++) {
			std::cout << Dr[k][i].variavel << " -> ";
			for (int j = 0; j < Dr[k][i].cadeia_simbolos.size(); j++)
				std::cout << "'" << Dr[k][i].cadeia_simbolos[j] << "' ";
			std::cout << "\n";
		}
	}
}

// -----------------------------------------------------------------------------