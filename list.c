/// @file list.c

/**
 * El struct de nodo de las listas.
 */
typedef struct structNodoLista {
	void * valor; /**< el puntero al valor asociado */
	struct structNodoLista * hijo; /**< el nodo siguiente */
} NodoLista;

/**
 * El struct de lista.
 */
typedef struct structLista {
	NodoLista * inicio; /**< el primer nodo */
	int longitud; /**< la longitud de la lista */
} Lista;

/**
 * Crea un nodo de una lista.
 * 
 * @param el puntero al valor respectivo
 * @return el puntero al nodo
 */
static NodoLista * crearNodoLista(void * valor) {
	NodoLista * nodo = calloc(1, sizeof(NodoLista));
	nodo->valor = valor;
	return nodo;
}

/**
 * Crea una lista.
 *
 * @return el puntero a una lista nueva
 */
Lista * crearLista() {
	return calloc(1, sizeof(Lista));
}

/**
 * Inserta el puntero de un valor en una lista.
 * 
 * @param edd la lista
 * @param valor el puntero de un valor
 */
void insertarValorLista(Lista * edd, void * valor) {
	if (!(edd->inicio)) {
		edd->inicio = crearNodoLista(valor);
		edd->longitud++;
		return;
	}
	NodoLista * act = edd->inicio;
	while (act->hijo) act = act->hijo;
	act->hijo = crearNodoLista(valor);
	edd->longitud++;
}

/**
 * Inserta un texto en una lista.
 * 
 * @param edd la lista
 * @param texto el texto
 */
void insertarTextoLista(Lista * edd, char * texto) {
	insertarValorLista(edd, moverTexto(texto));
}

/**
 * Insertar un valor en una lista si este no está.
 *
 * @param edd la lista
 * @param valor el puntero al valor
 * @param comparar la función de comparación entre dos elementos para determinar igualdad (debería retornar 0 si son diferentes)
 * @return el código de éxito (1 si se insertó y 0 si no)
 */
char insertarNoRepetidoLista(Lista * edd, void * valor, char (*comparar)(void *, void *)) {
	if (!(edd->inicio)) {
		edd->inicio = crearNodoLista(valor);
		edd->longitud++;
		return 1;
	}
	NodoLista * act = edd->inicio;
	while (act->hijo && !comparar(act->valor, valor)) act = act->hijo;
	if (!comparar(act->valor, valor)) {
		act->hijo = crearNodoLista(valor);
		edd->longitud++;
		return 1;
	}
	return 0;
}

/**
 * Insertar un texto en una lista si este no está.
 *
 * @param edd la lista
 * @param texto el texto
 */
void insertarTextoNoRepetidoLista(Lista * edd, char * texto) {
	if (!insertarNoRepetidoLista(edd, (texto = moverTexto(texto)), compararStrings)) free(texto);
}

/**
 * Mapea (aplica) una función a cada valor de una lista con una estructura extra.
 *
 * @param varas dios sabe qué
 * @param edd la lista
 * @param aplicar la función a aplicar
 */
void mapListaConAdicional(void * varas, Lista * edd, void (*aplicar)(void *, char *)) {
	if (!(edd->inicio)) return;
	NodoLista * act = edd->inicio;
	while (act) {
		aplicar(varas, (char *)(act->valor));
		act = act->hijo;
	}
}

/**
 * Mapea (aplica) una función a cada valor de una lista.
 *
 * @param edd la lista
 * @param aplicar la función a aplicar
 */
void mapLista(Lista * edd, void (*aplicar)(void *)) {
	if (!(edd->inicio)) return;
	NodoLista * act = edd->inicio;
	while (act) {
		aplicar(act->valor);
		act = act->hijo;
	}
}

/**
 * Elimina los nodos de una lista.
 * 
 * @param nodo el nodo actual
 */
static void eliminarNodoLista(NodoLista * nodo) {
	if (nodo->hijo) eliminarNodoLista(nodo->hijo);
	free(nodo);
}

/**
 * Elimina una lista.
 * 
 * @param edd la lista
 */
void eliminarLista(Lista * edd) {
	if (edd->inicio) {
		eliminarNodoLista(edd->inicio);
	}
	free(edd);
}

/**
 * Determina la intersección entre dos listas (sobreescriba la primera).
 *
 * @param edd1 lista 1
 * @param edd2 lista 2
 * @param comparar la función de comparación
 * @return la lista de intersecciones
 */
Lista * intersecLista(Lista * edd1, Lista * edd2, char (*comparar)(void *, void *)) {
	Lista * intersec = crearLista();
	if (!(edd1->inicio && edd2->inicio)) return intersec;
	NodoLista * act1 = edd1->inicio;
	while (act1) {
		NodoLista * act2 = edd2->inicio;
		while (act2) {
			if (comparar(act1->valor, act2->valor)) insertarValorLista(intersec, act1->valor);
			act2 = act2->hijo;
		}
		act1 = act1->hijo;
	}
	return intersec;
}


/**
 * Determina la intersección entre dos listas de texto (sobreescriba la primera).
 *
 * @param edd1 lista 1
 * @param edd2 lista 2
 * @return la lista de intersecciones
 */
Lista * intersecListaTexto(Lista * edd1, Lista * edd2) {
	return intersecLista(edd1, edd2, compararStrings);
}
