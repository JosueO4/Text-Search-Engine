/// @file btree.c

static const int MAXT = 4; /**< la máxima cantidad de valores en un nodo */

/**
 * El struct de valor de los nodos de los árboles B.
 */
typedef struct structValorBTree {
	int hash; /**< la llave */
	char * texto; /**< el texto */
	Lista * asociados; /**< los archivos asociados */
	struct structValorBTree * colision; /**< la colisión */
} ValorBTree;

/**
 * El struct de nodo de los árboles B. 
 */
typedef struct structNodoBTree {
	int cantidad; /**< la cantidad de valores */
	ValorBTree ** valores; /**< los valores */
	struct structNodoBTree ** hijos; /**< los hijos */
} NodoBTree;

/**
 * El struct de árbol B.
 */
typedef struct structBTree {
	NodoBTree * tree; /**< el nodo base */
} BTree;

/**
 * Crea un campo de valor de un nodo de un árbol B.
 *
 * @return el struct de valor
 */
static ValorBTree * crearValorBTree(char * texto) {
	ValorBTree * nodo = calloc(1, sizeof(ValorBTree));
	nodo->texto = moverTexto(texto);
	nodo->hash = stringToHash(texto);
	nodo->asociados = crearLista();
	return nodo;
}

/**
 * Crea los hijos de un nodo de un árbol B.
 *
 * @return un arreglo de punteros a hijos
 */
static NodoBTree ** crearHijosNodoBTree() {
	return calloc(MAXT + 2, sizeof(NodoBTree *));
}

/**
 * Crea un nodo de un árbol B.
 *
 * @return el struct de un nodo
 */
static NodoBTree * crearNodoBTree() {
	NodoBTree * edd = calloc(1, sizeof(NodoBTree));
	edd->valores = calloc(MAXT + 1, sizeof(ValorBTree *));
	return edd;
}

/**
 * Crea un árbol B.
 *
 * @return el struct de un árbol B
 */
BTree * crearBTree() {
	return calloc(1, sizeof(BTree));
}

/**
 * Intenta insertar una palabra con un camino asociado en un valor de
 * un nodo de un árbol B.
 *
 * @param act el valor del nodo del árbol B donde se insertará el texto
 * @param texto la palabra
 * @param camino el camino asociado
 */
static void insertarEnIgual(ValorBTree * act, char * texto, char * camino) {
	char found = 0;
	if (compararStrings(act->texto, texto)) {	
		insertarTextoNoRepetidoLista(act->asociados, camino);
		found = 1;
	}
	while (act->colision && !found) {
		act = act->colision;
		if (compararStrings(act->texto, texto)) {
			insertarTextoNoRepetidoLista(act->asociados, camino);
			found = 1;
		}
	}
	if (!found) {
		act->colision = crearValorBTree(texto);
		insertarTextoNoRepetidoLista(act->colision->asociados, camino);
	}
}

/**
 * Intenta insertar una palabra con un camino asociado de forma ordenada
 * en los valores de un nodo de un árbol B.
 *
 * @param edd el nodo del árbol B
 * @param texto la palabra
 * @param camino el camino asociado
 * @return 0 si no se creó un nuevo nodo
 */
static char insertarArregloOrdenado(NodoBTree * edd, char * texto, char * camino) {
	int inf = 0, sup = edd->cantidad, prevmid = -1, hash = stringToHash(texto);
	int mid = (inf + sup) >> 1;
	if (!(edd->cantidad)) {
		edd->valores[0] = crearValorBTree(texto);
		insertarTextoNoRepetidoLista(edd->valores[0]->asociados, camino);
		return 1;
	}
	while (prevmid != mid && edd->valores[mid]->hash != hash) {
		if (edd->valores[mid]->hash < hash) inf = mid;
		else sup = mid;
		prevmid = mid;
		mid = (inf + sup) >> 1;
	}
	ValorBTree * act = edd->valores[mid];
	if (act->hash == hash) {
		insertarEnIgual(act, texto, camino);
		return 0;
	}
	if (act->hash < hash) mid++;
	for (int i = MAXT - 1; i >= mid; i--) {
		edd->valores[i + 1] = edd->valores[i];
	}
	edd->valores[mid] = crearValorBTree(texto);
	insertarTextoNoRepetidoLista(edd->valores[mid]->asociados, camino);
	return 1;
}

/**
 * Separa un nodo de un árbol B (para subir el valor central).
 *
 * @param edd el nodo del árbol B
 * @return el trio valor a subir, load izquierdo, lado derecho
 */
static void ** splitBTree(NodoBTree * edd) {
	void ** resp = calloc(3, sizeof(void *));
	resp[0] = edd->valores[MAXT/2];
	resp[1] = edd;
	edd->cantidad = MAXT/2;
	NodoBTree * nodo = crearNodoBTree();
	resp[2] = nodo;
	nodo->cantidad = MAXT/2;
	for (int i = 0; i < MAXT/2; i++)
		nodo->valores[i] = edd->valores[i + MAXT/2 + 1];
	if (edd->hijos) {
		nodo->hijos = calloc(MAXT + 2, sizeof(NodoBTree));
		for (int i = 0; i < MAXT/2 + 1; i++)
			nodo->hijos[i] = edd->hijos[i + MAXT/2 + 1];
	}
	return resp;
}

/**
 * Intenta insertar una palabra con un camino asociado en un nodo de un árbol B.
 *
 * @param edd el nodo del árbol B
 * @param texto la palabra
 * @param camino el camino asociado
 * @return el trio valor a subir, lado izquierdo, lado derecho
 */
static void ** insertarBTreeAux(NodoBTree * edd, char * texto, char * camino) {
	if (edd->hijos) {
		int hash = stringToHash(texto);
		NodoBTree * subtree = edd->hijos[0];
		int i = 0;
		while (i < edd->cantidad) {
			if (edd->valores[i]->hash == hash) {
				insertarEnIgual(edd->valores[i], texto, camino);
				return 0;
			}
			if (edd->valores[i]->hash > hash) break;
			i++;
			subtree = edd->hijos[i];
		}
		void ** intento = insertarBTreeAux(subtree, texto, camino);
		if (!intento) return 0;
		ValorBTree * nodo = intento[0];
		NodoBTree * sub1 = intento[1];
		NodoBTree * sub2 = intento[2];
		free(intento);
		edd->hijos[i] = sub1;
		for (int j = MAXT - 1; j >= i; j--) edd->valores[j + 1] = edd->valores[j];
		edd->valores[i] = nodo;
		i++;
		for (int j = MAXT; j >= i; j--) edd->hijos[j + 1] = edd->hijos[j];
		edd->hijos[i] = sub2;
	} else {
		if (!insertarArregloOrdenado(edd, texto, camino)) return 0;
	}
	edd->cantidad++;
	if (edd->cantidad <= MAXT) return 0;
	return splitBTree(edd);
}

/**
 * Intenta insertar una palabra con un camino asociado en un árbol B.
 *
 * @param edd el árbol B
 * @param texto la palabra
 * @param camino el camino asociado
 */
void insertarBTree(BTree * edd, char * texto, char * camino) {
	if (!(edd->tree)) edd->tree = crearNodoBTree();
	void ** intento = insertarBTreeAux(edd->tree, texto, camino);
	if (!intento) return;
	ValorBTree * nodo = intento[0];
	NodoBTree * sub1 = intento[1];
	NodoBTree * sub2 = intento[2];
	free(intento);
	NodoBTree * newTree = crearNodoBTree();
	newTree->cantidad = 1;
	newTree->valores[0] = nodo;
	newTree->hijos = crearHijosNodoBTree();
	newTree->hijos[0] = sub1;
	newTree->hijos[1] = sub2;
	edd->tree = newTree;
}

/**
 * Busca un texto dentro de un árbol B.
 *
 * @param edd el árbol B
 * @param texto el texto a buscar
 * @return la lista de archivos asociados
 */
Lista * buscarBTree(BTree * edd, char * texto) {
	NodoBTree * act = edd->tree;
	int hash = stringToHash(texto);
	while (act) {
		int i = 0;
		while (i < act->cantidad) {
			if (act->valores[i]->hash == hash) {
				ValorBTree * data = act->valores[i];
				while (data) {
					if (compararStrings(texto, data->texto)) return data->asociados;
					data = data->colision;
				}
				return 0;
			}
			if (act->valores[i]->hash > hash) break;
			i++;
		}
		if (act->hijos) act = act->hijos[i];
		else act = 0;
	}
	return 0;
}

/**
 * Elimina los valores de un nodo en un árbol B.
 *
 * @param valor el valor a eliminar
 */
static void eliminarValoresBTree(ValorBTree * valor) {
	free(valor->texto);
	mapLista(valor->asociados, eliminarNodoTexto);
	eliminarLista(valor->asociados);
	if (valor->colision) eliminarValoresBTree(valor->colision);
	free(valor);
}

/**
 * Elimina los nodos de un árbol B.
 *
 * @param nodo el nodo a eliminar
 */
static void eliminarNodosBTree(NodoBTree * nodo) {
	for (int i = 0; i < nodo->cantidad; i++) eliminarValoresBTree(nodo->valores[i]);
	free(nodo->valores);
	if (nodo->hijos) {
		for (int i = 0; i < nodo->cantidad + 1; i++) eliminarNodosBTree(nodo->hijos[i]);
		free(nodo->hijos);
	}
	free(nodo);
}

/**
 * Elimina un árbol B.
 *
 * @param tree el arbol b
 */
void eliminarBTree(BTree * tree) {
	if (tree->tree) eliminarNodosBTree(tree->tree);
	free(tree);
}
