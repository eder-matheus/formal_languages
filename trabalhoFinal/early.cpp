#include "early.h"

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
			//if (!simboloNaoPertence) {
			for (int k = 0; k < potenciaisTerminais.size(); k++) {
				terminaisSentenca.push_back(potenciaisTerminais[k]);
			}
			//}
		}

		potenciaisTerminais.clear();
		potencialTerminal.clear();
		i++;
	} while (i < sentenca.size());
}

// -----------------------------------------------------------------------------

int encontraMarcador(std::vector<std::string> const &producao) {
	int i = 0;
	std::string marcador;
	marcador.push_back(char(46));

	while (i < producao.size() && producao[i].compare(marcador) != 0)
		i++;

	if (i < producao.size())
		return i; // Retorna o indice do marcador numa producao
	else
		return -1;
}

// -----------------------------------------------------------------------------

void avancaMarcador(std::vector<std::string> &producao, int conjunto) {
	int m = encontraMarcador(producao);
	std::string cjto = producao.back();
	std::string marcador;
	marcador.push_back(char(46));

	if (m != -1) {
		producao[m] = producao[m + 1];
		producao[m + 1] = marcador;
	} else {
		std::vector<std::string> producaoAux;

		producaoAux = producao;
		producao.clear();
		producao.push_back(marcador);
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
	D0.clear();

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
	std::vector<REGRA> Di, Daux2;

	for (int i = 1; i <= sentenca.size(); i++) {
		Di.clear();

		for (int j = 0; j < Dr[i - 1].size(); j++) { // Producoes que geram o simbolo ar
			simboloMarcado = encontraMarcador(Dr[i - 1][j].cadeia_simbolos) + 1;
			if (Dr[i - 1][j].cadeia_simbolos[simboloMarcado].compare(sentenca[i - 1]) == 0) {
				Di.push_back(Dr[i - 1][j]);
			}
		}

		for (int k = 0; k < Di.size(); k++) {
			avancaMarcador(Di[k].cadeia_simbolos, -1);
		}

		int sizeAux = 0;

		do {
			REGRA regraAux;
			for (int k = 0; k < Di.size(); k++) {
				simboloMarcado = encontraMarcador(Di[k].cadeia_simbolos) + 1;
				for (int l = 0; l < G.regras.size(); l++) { // Producoes que podem gerar o proximo simbolo
					if (Di[k].cadeia_simbolos[simboloMarcado].compare(G.regras[l].variavel) == 0) {
						regraAux = G.regras[l];
						avancaMarcador(regraAux.cadeia_simbolos, i);
						if (!encontraProducao(regraAux, Di)) {
							Di.push_back(regraAux);
						}
					}
				}
			}

			for (int k = 0; k < Di.size(); k++) { // Uma subpalavra w foi reduzida a variavel A
				simboloMarcado = encontraMarcador(Di[k].cadeia_simbolos) + 1;
				if ((int) Di[k].cadeia_simbolos[simboloMarcado][0] == 47) {
					std::string variavel = Di[k].variavel;
					std::string conjunto;

					for (int l = 1; l < Di[k].cadeia_simbolos[simboloMarcado].size(); l++) {
						conjunto.push_back(Di[k].cadeia_simbolos[simboloMarcado][l]);
					}
					std::string::size_type sz;
					int s = std::stoi(conjunto, &sz);

					for (int l = 0; l < Dr[s].size(); l++) {
						simboloMarcado = encontraMarcador(Dr[s][l].cadeia_simbolos) + 1;
						if (variavel.compare(Dr[s][l].cadeia_simbolos[simboloMarcado]) == 0) {
							regraAux = Dr[s][l];
							avancaMarcador(regraAux.cadeia_simbolos, -1);
							if (!encontraProducao(regraAux, Di)) {
								Di.push_back(regraAux);
							}
						}
					}
				}
			}

			sizeAux++;
		} while (sizeAux < Di.size());

		Dr.push_back(Di);
	}
}

// -----------------------------------------------------------------------------

bool aceitaSentenca(std::vector<REGRA> const &Dn, GRAMATICA const &G) {
	int simboloMarcado;
	for (int i = 0; i < Dn.size(); i++) {
		simboloMarcado = encontraMarcador(Dn[i].cadeia_simbolos) + 1;
		if (Dn[i].variavel.compare(G.inicial) == 0 && Dn[i].cadeia_simbolos[simboloMarcado].compare("/0") == 0) {
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------

void early(std::string const &entrada, GRAMATICA const &G) {
	std::vector<REGRA> D0;
	std::vector<std::vector < REGRA>> Dr;
	std::vector<std::string> sentenca;

	constroiD0(G, D0);
	Dr.push_back(D0);

	analisaSentenca(entrada, G, sentenca);
	constroiDr(G, Dr, sentenca);

	for (int k = 0; k < Dr.size(); k++) {
		std::cout << "D" << k << "\n";
		for (int i = 0; i < Dr[k].size(); i++) {
			std::cout << Dr[k][i].variavel << " -> ";
			for (int j = 0; j < Dr[k][i].cadeia_simbolos.size(); j++)
				std::cout << "'" << Dr[k][i].cadeia_simbolos[j] << "' ";
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	if (aceitaSentenca(Dr.back(), G)) {
		std::cout << "Sentenca reconhecida\n";
		std::cout << "\n";
	} else {
		std::cout << "Sentenca rejeitada\n";
	}
}

// -----------------------------------------------------------------------------