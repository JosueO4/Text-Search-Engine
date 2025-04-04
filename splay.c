/// @file splay.c

/**
 * Struct de subnodo que contiene el texto asociado a un hash y lo maneja.
 */
typedef struct structSubNodoSplayTree {
	char * texto; /**< texto asociado a hash */
	Lista * valores; /**< lista con nombres de los archivos asociados */
	struct structSubNodoSplayTree * colision; /**< el subnodo de colisión */
} SubNodoSplayTree;

/**
 * Struct de nodo que tiene el hash asociado y lo maneja.
 */
typedef struct structNodoSplayTree{
	int hash; /**< valor del hash */
	SubNodoSplayTree * colision; /**< el subnodo de colisión (contiene el texto asociado) */
	struct structNodoSplayTree * hijo_izq; /**< el hijo izquierdo */
	struct structNodoSplayTree * hijo_der; /**< el hijo derecho */
	} NodoSplayTree;

/**
 * Struct del arbol chaflaneado.
 */
typedef struct structSplayTree {
	NodoSplayTree * tree; /**< la raíz del árbol */
} SplayTree;

/**
 * Crear un nodo y alocarlo en memoria para arbol chaflaneado.
 *
 * @param texto el texto que se asocia a cada nodo
 * @param lista con nombres de archivos
 */
NodoSplayTree * crearNodoChaflaneado(char* texto, Lista * lista){
	NodoSplayTree * nuevo = calloc(1,sizeof(NodoSplayTree));
	nuevo->colision = calloc(1, sizeof(SubNodoSplayTree));
	nuevo->colision->texto = moverTexto(texto);
	nuevo->hash = stringToHash(texto);
	nuevo->colision->valores = lista;
	return nuevo;
	}

/**
 * Crear un arbol chaflaneado.
 *
 * y alocarlo en memoria 
 */
SplayTree * crearSplayTree() {
	return calloc(1, sizeof(SplayTree));
}

/**
 * Inserta un nodo en el arbol chaflaneado (Auxiliar).
 *
 * @param nodo nodo que se quiere insertar
 * @param hash el valor hash del texto
 * @param texto el texto que se quiere ingresar
 * @param lista lista con nombres de archivos
 */
static void insertarChaflaneadoAux(NodoSplayTree * nodo, int hash, char* texto, Lista * lista) {
	if (hash<nodo->hash){
		if (nodo->hijo_izq == NULL) {
			nodo->hijo_izq = crearNodoChaflaneado(texto, lista);
			return;
			}
		insertarChaflaneadoAux(nodo->hijo_izq, hash, texto, lista);
		}
	else if (hash == nodo->hash){
		SubNodoSplayTree * act = nodo->colision;
		while (act->colision != NULL) {
			act = act->colision;
			}
		act->colision = calloc(1, sizeof(SubNodoSplayTree));
		act->colision->texto = moverTexto(texto);
		act->colision->valores = lista;
		}
	else{
		if (nodo->hijo_der == NULL) {
			nodo->hijo_der = crearNodoChaflaneado(texto, lista);
			return;
			}
		insertarChaflaneadoAux(nodo->hijo_der, hash, texto, lista);
		}
	}

/**
 * Inserta un nodo en el arbol chaflaneado.
 *
 * @param raiz raiz del arbol
 * @param texto el texto que se quiere ingresar
 * @param lista la lista de los nombres de archivos
 */
void insertarChaflaneado(SplayTree * raiz, char* texto, Lista * lista){
	if(raiz->tree == NULL){
		raiz->tree = crearNodoChaflaneado(texto, lista);
		return;
		}
	int hash = stringToHash(texto);
	insertarChaflaneadoAux(raiz->tree, hash, texto, lista);
	}

//rotaciones
/**
 * Rotar arbol a la derecha.
 *
 * @param raiz la raiz del arbol que se quiere rotar
 */
static NodoSplayTree * rotar_der(NodoSplayTree * raiz){
	NodoSplayTree * n1 = raiz->hijo_izq;
	raiz->hijo_izq = n1->hijo_der;
	n1->hijo_der = raiz;
	raiz = n1;
	return n1;
	}

/**
 * Rotar arbol a la izquierda.
 *
 * @param raiz la raiz del arbol que se quiere rotar
 */
static NodoSplayTree * rotar_izq(NodoSplayTree * raiz){
	NodoSplayTree * n1 = raiz->hijo_der;
	raiz->hijo_der = n1->hijo_izq;
	n1->hijo_izq = raiz;
	raiz = n1;
	return n1;
	}

//hacer el chaflaneo
/**
 * Chaflanea el arbol dado.
 *
 * Hace las rotaciones segun los casos que existen
 * @param raiz la raiz del arbol 
 * @param hash el valor hash del texto
 */
static NodoSplayTree * chaflanear(NodoSplayTree * raiz, int hash){
	if(raiz== NULL || raiz->hash == hash){
		return raiz;
		}
	//derecho
	if (raiz->hash < hash){
		if(raiz->hijo_der == NULL){
			//caso base
			return raiz;
			}
		if(raiz->hijo_der->hash > hash){
			//caso 1
			raiz->hijo_der->hijo_izq = chaflanear(raiz->hijo_der->hijo_izq, hash); //llamada recursiva
			if(raiz->hijo_der->hijo_izq != NULL){
				raiz->hijo_der = rotar_der(raiz->hijo_der);
				}
			}
		else if(raiz->hijo_der->hash < hash){
			//caso 2
			raiz->hijo_der->hijo_der = chaflanear(raiz->hijo_der->hijo_der, hash); //llamada recursiva
			raiz = rotar_izq(raiz);
			}
		if(raiz->hijo_der == NULL){
			return raiz;
			}
		else{
			return rotar_izq(raiz);
			}
		}
	else{
		//izquierdo
		if(raiz->hijo_izq == NULL){
			return raiz;
			}
		if(raiz->hijo_izq->hash > hash){
			//caso 2
			raiz->hijo_izq->hijo_izq = chaflanear(raiz->hijo_izq->hijo_izq, hash);
			raiz=rotar_der(raiz);
			}
		else if(raiz->hijo_izq->hash < hash){
			//caso 1
			raiz->hijo_izq->hijo_der = chaflanear(raiz->hijo_izq->hijo_der, hash);
			if(raiz->hijo_izq->hijo_der != NULL){
				raiz->hijo_izq = rotar_izq(raiz->hijo_izq);
				}
			}
		if(raiz->hijo_izq == NULL){
			return raiz;
			}
		else{
			return rotar_der(raiz);
			}
		}
	}

//buscar
/**
 * Buscar un nodo.
 *
 * @param raiz la raiz del arbol 
 * @param texto el texto que se busca
 */
Lista * buscarChaflaneado(SplayTree * raiz, char* texto){
	int hash = stringToHash(texto);
	NodoSplayTree * temp = chaflanear(raiz->tree,hash);
	raiz->tree = temp;
	if (temp == NULL) {
		return NULL;
		}
	SubNodoSplayTree * act = temp->colision;
	while (act) {
		if (compararStrings(texto, act->texto)) {
			return act->valores;
			}
		act = act->colision;
		}
	return NULL;
}

/**
 * Eliminar un subnodo.
 *
 * @param subNodo el subnodo que se quiere eliminar
 */
static void eliminarSubNodoSplayTree(SubNodoSplayTree * subNodo) {
	// De esto se encarga el eliminar del BTree...
	free(subNodo->texto);
	//mapLista(sub_nodo->valores, eliminarNodoTexto);
	//eliminarLista(sub_nodo->valores);
	if (subNodo->colision != NULL)
		eliminarSubNodoSplayTree(subNodo->colision);
	free(subNodo);
}

/**
 * Eliminar un nodo.
 *
 * @param nodo el nodo que se quiere eliminar
 */
static void eliminarNodoArbolSplayTree(NodoSplayTree * nodo) {
	eliminarSubNodoSplayTree(nodo->colision);
	if (nodo->hijo_izq != NULL)
		eliminarNodoArbolSplayTree(nodo->hijo_izq);
	if (nodo->hijo_der != NULL)
		eliminarNodoArbolSplayTree(nodo->hijo_der);
	free(nodo);
}

/**
 * Eliminar un arbol chaflaneado.
 *
 * @param raiz la raiz del arbol que se quiere eliminar
 */
void eliminarSplayTree(SplayTree * raiz) {
	if (raiz->tree != NULL) eliminarNodoArbolSplayTree(raiz->tree);
	free(raiz);
}
