#include "earley.h"

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

		std::string s = "/" + std::to_string(conjunto);
		producao.push_back(s);
	}
}

// -----------------------------------------------------------------------------

void constroiD0(GRAMATICA const &G, std::vector<REGRA> &D0) {
	std::vector<std::string> producaoAux;
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

void constroiDr(GRAMATICA const &G, std::vector<std::vector<REGRA>> &Dr, std::string sentenca) {
	int simbolo;
	std::vector<REGRA> Daux;
	for (int i = 1; i <= sentenca.size(); i++) {
		for (int j = 0; j < Dr[i - 1].size(); j++) { // Producoes que geram o simbolo ar
			simbolo = encontraMarcador(Dr[i - 1][j].cadeia_simbolos) + 1;
			if (Dr[i - 1][j].cadeia_simbolos[simbolo][0] == sentenca[i - 1] && Dr[i - 1][j].cadeia_simbolos[simbolo].size() == 1) {
				Daux.push_back(Dr[i - 1][j]);
				//avancaMarcador(Daux.back().cadeia_simbolos);
			}
		}

		for (int k = 0; k < Daux.size(); k++) {
			avancaMarcador(Daux[k].cadeia_simbolos, (i-1));
		}

		Dr.push_back(Daux);
		Daux.clear();

		int sizeAux;

		do {
			for (int k = 0; k < Dr[i].size(); k++) {
				simbolo = encontraMarcador(Dr[i][k].cadeia_simbolos) + 1;
				for (int l = 0; l < G.regras.size(); l++) { // Producoes que podem gerar o proximo simbolo
					if (Dr[i][k].cadeia_simbolos[simbolo].compare(G.regras[l].variavel) == 0) {
						if (!encontraProducao(G.regras[l], Daux)) {
							Daux.push_back(G.regras[l]);
							//avancaMarcador(Dr[i].back().cadeia_simbolos);
						}
					}
				}

				for (int l = 0; l < Dr[i].size(); l++) {
					if (Dr[i][l].cadeia_simbolos[simbolo].compare("/0") == 0) {
						for (int m = 0; m < Dr[i].size(); m++) {
							//if (Dr[i])
							}
					}
				}
			}

			sizeAux++;
		} while (sizeAux < Dr[i].size());

		for (int k = 0; k < Daux.size(); k++)
			avancaMarcador(Daux[k].cadeia_simbolos, i);
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