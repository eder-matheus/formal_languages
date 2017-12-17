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

int encontraReducao(std::string const &variavel, std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di) {
	int marcador;
	int s = 0; // Variavel usada para indicar o ponteiro da producao reduzida

	for (int i = 0; i < Dr.size(); i++)
		for (int j = 0; j < Dr[i].size(); j++)
			s++;

	for (int k = 0; k < Di.size(); k++) {
		if (variavel.compare(Di[k].variavel) == 0) {
			marcador = encontraMarcador(Di[k].cadeia_simbolos);
			if ((int) Di[k].cadeia_simbolos[marcador + 1][0] == 47) {
				return s;
			}
		}
		s++;
	}
}

// -----------------------------------------------------------------------------

int encontraReducaoAnterior(std::string const &variavel, std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di) {
	int marcador;
	int s = 0;

	for (int i = 0; i < Dr.size(); i++) {
		for (int j = 0; j < Dr[i].size(); j++) {
			if (variavel.compare(Dr[i][j].variavel) == 0) {
				marcador = encontraMarcador(Dr[i][j].cadeia_simbolos);
				if ((int) Dr[i][j].cadeia_simbolos[marcador + 1][0] == 47) {
					return s;
				}
			}
			s++;
		}
	}

	for (int k = 0; k < Di.size(); k++) {
		if (variavel.compare(Di[k].variavel) == 0) {
			marcador = encontraMarcador(Di[k].cadeia_simbolos);
			if ((int) Di[k].cadeia_simbolos[marcador + 1][0] == 47) {
				return s;
			}
		}
		s++;
	}
}

// -----------------------------------------------------------------------------

void encontraPonteiros(std::vector<std::vector<REGRA>> const &Dr, std::vector<REGRA> const &Di, REGRA const &producao, std::vector<std::vector<int>> &ponteiros) {
	int marcador;
	int p, p1, p2;
	std::vector<int> ponteiro;

	marcador = encontraMarcador(producao.cadeia_simbolos);
	if (marcador - 1 >= 0 && (producao.cadeia_simbolos.size() == 3 || marcador + 1 < producao.cadeia_simbolos.size() - 1)) { // Se o marcador da producao estiver entre duas variaveis
		p = encontraReducao(producao.cadeia_simbolos[marcador - 1], Dr, Di);
		ponteiro.push_back(p);
		ponteiros.push_back(ponteiro);
	} else if (marcador == 2 && producao.cadeia_simbolos.size() == 4) {
		std::string var1, var2;

		marcador = encontraMarcador(producao.cadeia_simbolos);
		var1 = producao.cadeia_simbolos[marcador - 2];
		var2 = producao.cadeia_simbolos[marcador - 1];

		p1 = encontraReducaoAnterior(var1, Dr, Di);

		p2 = encontraReducao(var2, Dr, Di);

		ponteiro.push_back(p1);
		ponteiro.push_back(p2);
		ponteiros.push_back(ponteiro);
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

void constroiDr(GRAMATICA const &G, std::vector<std::vector<REGRA>> &Dr, std::vector<std::string> const &sentenca, std::vector<std::vector<int>> &ponteiros) {
	int simboloMarcado;
	std::vector<REGRA> Di;
	std::vector<int> ponteiro;
	ponteiro.push_back(-1);

	for (int i = 0; i < Dr[0].size(); i++) {
		ponteiros.push_back(ponteiro);
	}

	for (int i = 1; i <= sentenca.size(); i++) {
		Di.clear();

		for (int j = 0; j < Dr[i - 1].size(); j++) { // Producoes que geram o simbolo ar
			simboloMarcado = encontraMarcador(Dr[i - 1][j].cadeia_simbolos) + 1;
			if (Dr[i - 1][j].cadeia_simbolos[simboloMarcado].compare(sentenca[i - 1]) == 0) {
				Di.push_back(Dr[i - 1][j]);
				ponteiros.push_back(ponteiro);
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
							ponteiros.push_back(ponteiro);
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
								encontraPonteiros(Dr, Di, regraAux, ponteiros);
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

void arvoreDeDerivacao(std::vector<std::vector<REGRA>> const &Dr, std::vector<std::vector<int>> &ponteiros, GRAMATICA const &G) {
	int idRegra = 0;
	int idRegraAux;
	int simboloMarcado;
	std::vector<REGRA> regras;
	std::vector<int> idsRegras;
	std::vector<std::string> terminais;
	std::vector<int> reducoes;

	for (int i = 0; i < Dr.size(); i++)
		for (int j = 0; j < Dr[i].size(); j++)
			regras.push_back(Dr[i][j]);

	for (int i = 0; i < Dr.size() - 1; i++)
		for (int j = 0; j < Dr[i].size(); j++)
			idRegra++;

	for (int i = 0; i < Dr[Dr.size() - 1].size(); i++) {
		simboloMarcado = encontraMarcador(Dr[Dr.size() - 1][i].cadeia_simbolos) + 1;
		if (Dr[Dr.size() - 1][i].variavel.compare(G.inicial) == 0 && Dr[Dr.size() - 1][i].cadeia_simbolos[simboloMarcado].compare("/0") == 0) {
			reducoes.push_back(idRegra);
		} else {
			idRegra++;
		}
	}

	for (int l = 0; l < reducoes.size(); l++) {
		idsRegras.push_back(reducoes[l]);
		std::cout << regras[idsRegras[0]].variavel << "(S" << idsRegras[0] << ")\n";

		int j = 0;
		while (j < idsRegras.size()) {
			int i = j;
			int numRegras = idsRegras.size();

			while (i < numRegras) {
				if (ponteiros[idsRegras[i]].size() == 1) {
					idRegraAux = ponteiros[idsRegras[i]][0];
					if (idRegraAux != -1) {
						std::cout << regras[idRegraAux].variavel << "(S" << idRegraAux << ") |";
						idsRegras.push_back(idRegraAux);
						if (encontraTerminal(regras[idRegraAux].cadeia_simbolos[0], G.terminais) && regras[idRegraAux].cadeia_simbolos.size() == 3)
							terminais.push_back(regras[idRegraAux].cadeia_simbolos[0]);
					} else {
						std::cout << terminais[0] << "| ";
						terminais.erase(terminais.begin() + 0);
					}
				} else {
					idRegraAux = ponteiros[idsRegras[i]][0];
					if (idRegraAux != -1) {
						std::cout << regras[idRegraAux].variavel << "(S" << idRegraAux << ")    ";
						idsRegras.push_back(idRegraAux);
						if (encontraTerminal(regras[idRegraAux].cadeia_simbolos[0], G.terminais) && regras[idRegraAux].cadeia_simbolos.size() == 3)
							terminais.push_back(regras[idRegraAux].cadeia_simbolos[0]);
					} else {
						std::cout << terminais[0] << "| ";
						terminais.erase(terminais.begin() + 0);
					}

					idRegraAux = ponteiros[idsRegras[i]][1];
					if (idRegraAux != -1) {
						std::cout << regras[idRegraAux].variavel << "(S" << idRegraAux << ") |";
						idsRegras.push_back(idRegraAux);
						if (encontraTerminal(regras[idRegraAux].cadeia_simbolos[0], G.terminais) && regras[idRegraAux].cadeia_simbolos.size() == 3)
							terminais.push_back(regras[idRegraAux].cadeia_simbolos[0]);
					} else {
						std::cout << terminais[0] << "| ";
						terminais.erase(terminais.begin() + 0);
					}
				}
				i++;
				j++;
			}
			std::cout << "\n";
		}
	}

	std::cout << "\n";
}

// -----------------------------------------------------------------------------

void early(std::string const &entrada, GRAMATICA const &G) {
	std::vector<REGRA> D0;
	std::vector<std::vector < REGRA>> Dr;
	std::vector<std::string> sentenca;
	std::vector<std::vector<int>> ponteiros;

	constroiD0(G, D0);
	Dr.push_back(D0);

	analisaSentenca(entrada, G, sentenca);
	constroiDr(G, Dr, sentenca, ponteiros);

	int s = 0;
	int p = 0;
	for (int k = 0; k < Dr.size(); k++) {
		std::cout << "D" << k << "\n";
		for (int i = 0; i < Dr[k].size(); i++) {
			std::cout << "S" << s << " " << Dr[k][i].variavel << " -> ";
			for (int j = 0; j < Dr[k][i].cadeia_simbolos.size(); j++)
				std::cout << "'" << Dr[k][i].cadeia_simbolos[j] << "' ";
			if (ponteiros[p].size() == 1) {
				std::cout << ponteiros[p][0];
			} else {
				for (int l = 0; l < ponteiros[p].size(); l++)
					std::cout << ponteiros[p][l] << ", ";
			}
			std::cout << "\n";
			s++;
			p++;
		}
		std::cout << "\n";
	}

	if (aceitaSentenca(Dr.back(), G)) {
		std::cout << "Sentenca reconhecida\n";
		std::cout << "\n";
	} else {
		std::cout << "Sentenca rejeitada\n";
	}

	std::cout << "Arvore de Derivacao: \n";
	arvoreDeDerivacao(Dr, ponteiros, G);
}

// -----------------------------------------------------------------------------