/// @file index.c

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

/**
 * Funcion para indexar palabras en tabla hash y arbol B.
 *
 * @return varias salidas
 */
void ** indexar(char * path) {

	DIR * d;

	struct dirent * dir;


	char ruta[256];

	char buffer[32];

	char last;

	short ptr = 0, pt1 = 0, pt2 = 0;
	
	FILE * archivo;

	if (!(d = opendir("hoy_en_el_tec"))) {
		closedir(d);
		return 0;
	}

	int cnt = 0;
	while ((dir = readdir(d))) cnt++;
	closedir(d);
	d = opendir("hoy_en_el_tec");

	HashTable * tablaHash = crearHashTable(cnt);
	BTree * arbol = crearBTree();

	while ((dir = readdir(d))) {
		if (dir->d_name[0] == '.') continue;

		pt1 = 0;
		pt2 = 0;
		while (path[pt1]) {
			ruta[pt1] = path[pt1];
			pt1++;
		}
		ruta[pt1] = 0;
		while (dir->d_name[pt2]) ruta[pt1++] = dir->d_name[pt2++];
		ruta[pt1] = 0;
		if ((archivo = fopen(ruta, "r"))) {
			insertarTextoHashTable(tablaHash, dir->d_name, ruta);
			ptr = 0;
			last = getc(archivo);
			while (last != EOF) {
				if ((last >= '0' && last <= '9') || (last >= 'a' && last <= 'z')
					|| (last >= 'A' && last <= 'Z') || last == 39 || last < 0) {
					if (ptr == 31) {
						buffer[ptr] = 0;
						insertarBTree(arbol, buffer, dir->d_name);
						ptr = 0;
					} else buffer[ptr++] = last;
				} else if (ptr) {
					buffer[ptr] = 0;
					insertarBTree(arbol, buffer, dir->d_name);
					ptr = 0;
				}
				last = getc(archivo);
			}
   		}
		fclose(archivo);
	}
	closedir(d);

	void ** respuesta = calloc(2, sizeof(void *));
	respuesta[0] = tablaHash;
	respuesta[1] = arbol;
	return respuesta;
}

/**
 * Funcion para guardar texto.
 *
 * @param archivo el archivo que tiene el texto
 * @param texto el texto que se quiere guardar
 */
void saveTexto(void * archivo, char * texto) {
	fprintf((FILE *)archivo, "%s\n", texto);
}

/**
 * Funcion para guardar la lista del texto.
 *
 * @param archivo el archivo que tiene el texto
 * @param lista la lista que contiene el texto
 */
void saveListaTexto(FILE * archivo, Lista * lista) {
	fprintf(archivo, "%d\n", lista->longitud);
	mapListaConAdicional(archivo, lista, saveTexto);
}

/**
 * Funcion para guardar los valores del Btree.
 *
 * @param archivo el archivo que tiene el texto
 * @param valor el valor a guardar
 */
void saveValoresBTree(FILE * archivo, ValorBTree * valor) {
	fprintf(archivo, "%s\n", valor->texto);
	saveListaTexto(archivo, valor->asociados);
	fprintf(archivo, "%d\n", valor->colision ? 1 : 0);
	if (valor->colision) saveValoresBTree(archivo, valor->colision);
}


/**
 * Funcion para guardar los nodos del Btree.
 *
 * @param archivo el archivo que tiene el texto
 * @param nodo el nodo a guardar
 */
void saveNodosBTree(FILE * archivo, NodoBTree * nodo) {
	//fprintf(archivo, "hola locas ");
	fprintf(archivo, "%d\n", nodo->cantidad);
	for (int i = 0; i < nodo->cantidad; i++)
		saveValoresBTree(archivo, nodo->valores[i]);
	fprintf(archivo, "%d\n", nodo->hijos ? 1 : 0);
	if (nodo->hijos) for (int i = 0; i <= nodo->cantidad; i++)
		saveNodosBTree(archivo, nodo->hijos[i]);
}

/**
 * Funcion para guardar los nodos del Btree.
 *
 * @param archivo el archivo que tiene el texto
 * @param tree el arbolB a guardar
 */
void saveBTree(FILE * archivo, BTree * tree) {
	if (tree->tree) saveNodosBTree(archivo, tree->tree);
}


/**
 * Funcion para guardar el Btree.
 *
 * @param archivo el archivo que tiene el texto
 * @param table el HashTable a guardar
 */
void saveHashTable(FILE * archivo, HashTable * table) {
	fprintf(archivo, "%d\n", table->tam);
	for (int i = 0; i < table->tam; i++) {
		if (table->tabla[i]) {
			NodoTablaHash * act = table->tabla[i]->inicio;
			while (act) {
				fprintf(archivo, "%s\n", act->nombre_archivo);
				fprintf(archivo, "%s\n", act->ruta_archivo);
				act = act->sigt;
			}
		}
	}
}

/**
 * Funcion para guardar TODOS los datos.
 *
 * @param path es la ruta donde se guarda
 * @param tree el arbolB a guardar
 * @param table el HashTable a guardar
 */
void saveData(char * path, BTree * tree, HashTable * table) {
	FILE * archivo = fopen(path, "w");
	if (!archivo) {
		fclose(archivo);
		return;
	}
	saveBTree(archivo, tree);
	saveHashTable(archivo, table);
	fclose(archivo);
}


/**
 * Funcion para cargar la lista con texto.
 *
 * @param archivo es el archivo donde estan guardadas
 * @param lista es la lista donde se va a insertar lo cargado
 */
void loadListaTexto(FILE * archivo, Lista * lista) {
	int longitud;
	fscanf(archivo, "%d", &longitud);
	char buffer[256];
	for (int i = 0; i < longitud; i++) {
		fscanf(archivo, " %255[^\n]", buffer);
		insertarTextoLista(lista, buffer);
	}
}

/**
 * Funcion para cargar los valores del BTree.
 *
 * @param archivo es el archivo donde estan guardadas
 * @return es el valor del BTree
 */
ValorBTree * loadValoresBTree(FILE * archivo) {
	char buffer[256];
	fscanf(archivo, " %255[^\n]", buffer);
	ValorBTree * valor = crearValorBTree(buffer);
	loadListaTexto(archivo, valor->asociados);
	int tieneColision;
	fscanf(archivo, "%d", &tieneColision);
	if (tieneColision) {
		valor->colision = loadValoresBTree(archivo);
	}
	return valor;
}

/**
 * Funcion para cargar los nodos del BTree.
 *
 * @param archivo es el archivo donde estan guardadas
 * @return es el nodo del BTree
 */
NodoBTree * loadNodosBTree(FILE * archivo) {
	NodoBTree * nodo = crearNodoBTree();
	fscanf(archivo, "%d", &(nodo->cantidad));
	for (int i = 0; i < nodo->cantidad; i++) {
		nodo->valores[i] = loadValoresBTree(archivo);
	}
	int tieneHijos;
	fscanf(archivo, "%d", &tieneHijos);
	if (tieneHijos) {
		nodo->hijos = crearHijosNodoBTree();
		for (int i = 0; i <= nodo->cantidad; i++) {
			nodo->hijos[i] = loadNodosBTree(archivo);
		}
	}
	return nodo;
}

/**
 * Funcion para cargar el BTree.
 *
 * @param archivo es el archivo donde estan guardadas
 * @return es el BTree
 */
BTree * loadBTree(FILE * archivo) {
	BTree * tree = crearBTree();
	tree->tree = loadNodosBTree(archivo);
	return tree;
}

/**
 * Funcion para cargar la HashTable.
 *
 * @param archivo es el archivo donde estan guardadas
 * @return es la HashTable
 */
HashTable * loadHashTable(FILE * archivo) {
	int tam;
	fscanf(archivo, "%d", &tam);
	HashTable * tabla = crearHashTable(tam);
	char buffer1[128];
	char buffer2[128];
	while (!feof(archivo)) {
		fscanf(archivo, "%s", buffer1);
		fscanf(archivo, "%s", buffer2);
		insertarTextoHashTable(tabla, buffer1, buffer2);
	}
	return tabla;
}

/**
 * Funcion para cargar TODOS lo datos.
 *
 * @param path es la ruta al archivo
 * @return se retornan multiples cosas
 */
void ** loadData(char * path) {
	FILE * archivo = fopen(path, "r");
	if (!archivo) {
		fclose(archivo);
		return 0;
	}
	void ** resp = calloc(2, sizeof(void *));
	resp[0] = loadBTree(archivo);
	resp[1] = loadHashTable(archivo);
	fclose(archivo);
	return resp;
}
