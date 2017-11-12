#include <cstdlib>
#include "regras.h"
#include "gramatica.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	GRAMATICA g;
	le_gramatica("exemplo-gramatica.txt", g);
	
	return 0;
}

