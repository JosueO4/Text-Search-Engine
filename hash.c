/// @file hash.c

//      SECCION STRUCTS      //
////////////////////////////////

/**
 * El struct que maneja los nodos contenedores.
 */
typedef struct structNodoTablaHash{

    int llave; /**< Llave ( Valor ASCII del nombre del archivo */

    char * ruta_archivo; /**< Valor ( Ruta del archivo ) */

    char * nombre_archivo; /**< Nombre del archivo */

    struct structNodoTablaHash * sigt; /**< Siguiente */
} NodoTablaHash;

/**
 * El struct que maneja cada parte individual de la tabla de hash.
 */
typedef struct structTablaHash {
    NodoTablaHash * inicio; /**< Inicio */
} TablaHash;

/**
 * El struct de HashTable.
 */
typedef struct structHashTable {
	TablaHash ** tabla; /**< La tabla */
	int tam; /**< el tamaño */
	int a; /**< constante 1 */
	int b; /**< constante 2 */
	int c; /**< constante 3 */
} HashTable;

//////////////////////////////////////////////

// SECCION PARA ENCONTRAR EL NUEVO TAMAÑO DEL HASH //

/////////////////////////////////////////////////

/**
 * Encuentra el primo más cercano a la entrada.
 *
 * @param tamano el tamaño
 * @return el primo mayor más cercano
 */
int encontrarPrimoCercano(int tamano){

    for(int i = 2; i*i < tamano ;i++){ // i va desde 2 hasta tamano //

            if(tamano%i == 0){ // Si i es divisor //

                return encontrarPrimoCercano(tamano+1); // Se avanza al siguiente numero //
            }
    }

    // Si ningun i es divisor es porque es primo //

    return tamano;
}

////////////////////////////////


// SECCION LLAVE Y POS EN EL HASH //

///////////////////////////////

/**
 * Calcula un hash a partir de entradas.
 *
 * @param llave la llave
 * @param a primera constante
 * @param b segunda constante
 * @param c tercera constante
 * @return el hash generado
 */
static unsigned long int funcionHash(int llave, int a, int b, int c){
    return (a*llave + b)%c; // Se le hace la funcion hash a la llave //
}

/**
 * Calcula un posición en la tablaHash a partir de entradas.
 *
 * @param llave la llave
 * @param tam el tamaño de la tabla
 * @param a primera constante
 * @param b segunda constante
 * @param c tercera constante
 * @return la posición determinada
 */
static int encontrarEspacioHashTable(int llave, int tam, int a, int b, int c){
    return (int)(funcionHash(llave, a, b, c) % tam); // Se retorna la posicion en la tabla //
}

////////////////////////////////////////////////


//          SECCION CREAR TABLA Y NODOS            //
/////////////////////////////////////////////////////

/**
 * Crea un nodo para las tablas de hash.
 *
 * @param nombre_archivo el nombre del archivo
 * @param ruta la ruta al archivo
 * @return un puntero al nodo creado
 */
static NodoTablaHash * crearNodoHashTable(char * nombre_archivo, char * ruta){

    NodoTablaHash * NuevoNodo = calloc(1, sizeof(NodoTablaHash)); // Se crea el nuevo nodo //

    NuevoNodo->llave = stringToHash(nombre_archivo); // Se le asigna la llave al nodo //

    NuevoNodo->ruta_archivo = moverTexto(ruta); // Se le asigna el valor al nodo //

    NuevoNodo->nombre_archivo = moverTexto(nombre_archivo); // Se le asigna el nombre del archivo al nodo //

    return NuevoNodo; // Se retorna el nodo //
}

/**
 * Crea una tabla de nodos.
 *
 * @param tam el tamaño
 * @return un arreglo que constituye la tabla
 */
static TablaHash ** crearTablaHash(int tam){

    return calloc(tam, sizeof(TablaHash *)); // Se le da memoria a la tabla hash //
}

/**
 * Crea el HashTable.
 *
 * @param tam el tamaño
 * @return un nuevo HashTable
 */
HashTable * crearHashTable(int tam) {
	HashTable * table = calloc(1, sizeof(HashTable));
	table->tam = encontrarPrimoCercano(tam);
	table->b = encontrarPrimoCercano(table->tam + 1);
	table->c = encontrarPrimoCercano(table->b + 10);
	table->a = encontrarPrimoCercano(table->c + 100);
	
	table->tabla = crearTablaHash(table->tam);
	return table;
}

//////     SECCION INSERTAR, ACCEDER Y MANEJO DE COLISION    ////
////////////////////////////////////////////////////////

/**
 * Maneja las colisiones durante inserción.
 *
 * @param tabla_hash el arreglo de hashes
 * @param nodo_colision el nodo a insertar
 * @param pos la posición
 */
static void colision(TablaHash ** tabla_hash, NodoTablaHash * nodo_colision, int pos ){

    NodoTablaHash * act = tabla_hash[pos]->inicio; // Se define act como el primer nodo en esa posision de la tabla //

    while(act->sigt != NULL){ // Si no hemos llegado hasta el ultimo nodo en esa posicion de la tabla de hash //

        act = act->sigt; // Recorremos //
    }

    // Una vez llegamos al final //

    act->sigt = nodo_colision; // Se inserta el nodo al final //
}


////////////////////////////////////////////////////////////

/**
 * Inserta un nodo de la tabla de hash en la tabla.
 * 
 * @param tablaHash la tabla de hash
 * @param el nodo a insertar
 */
static void insertarNodoHashTable(HashTable * tablaHash, NodoTablaHash * nodo_insertar){

    int pos = encontrarEspacioHashTable(nodo_insertar->llave, tablaHash->tam, tablaHash->a, tablaHash->b, tablaHash->c); // Se encuentra la posicion en la tabla de hash //
    if(tablaHash->tabla[pos] != NULL){ // Si hay una colision //

        colision(tablaHash->tabla,nodo_insertar,pos); // Se inserta en el mismo espacio pero en un arreglo de nodos //
    }
    else{ // Si no esta colisionado //

        tablaHash->tabla[pos] = calloc(1, sizeof(TablaHash)); // Se le da memoria ese espacio //

        tablaHash->tabla[pos]->inicio = nodo_insertar; // Se define que en ese espacio apuntando a inicio estara la ciudad //

    }
}

/**
 * Inserta un texto en una tabla de hash.
 *
 * @param tablaHash la tabla de hash
 * @param nombre_archivo el nombre del archivo
 * @param ruta el texto de la ruta absoluta
 */
void insertarTextoHashTable(HashTable * tablaHash, char * nombre_archivo, char * ruta) {
    insertarNodoHashTable(tablaHash, crearNodoHashTable(nombre_archivo, ruta));
}

///////////////////////////////////////////////////////

/**
 * Accede al texto guardado bajo una llave.
 *
 * @param tablaHash la tabla de hash
 * @param nombre_archivo la llave en
 * @return el texto asociado (NULL si no lo encontró)
 */
char * accederTextoHashTable(HashTable * tablaHash, char * nombre_archivo){

    int llave = stringToHash(nombre_archivo); // Se genera la llave en base al nombre de la ciudad //

    int pos = encontrarEspacioHashTable(llave, tablaHash->tam, tablaHash->a, tablaHash->b, tablaHash->c); // Se encuentra la posicion en la tabla de hash //

    if (!(tablaHash->tabla[pos]))
         return NULL;

    NodoTablaHash * nodo = tablaHash->tabla[pos]->inicio; // Se define nodo como el primer nodo en ese espacio //


    while(nodo != NULL){ // Mientras no se haya acabado la lista de los nodos en esa posicion

        if(compararStrings(nodo->nombre_archivo, nombre_archivo)){ // Si el identificador unico del nodo actual es igual a la llave del nodo a accesar //

            return nodo->ruta_archivo;  // Se retorna el nodo encontrado //
        }
        nodo = nodo->sigt; // Se avanza en la lista //
    }

    // Si no esta colisionado significa que solo hay uno y es el del inicio

    return NULL; // Se retorna el nodo encontrado //
}


////////////////////////////////////////////////////////////

// SECCION REDIMENCIONAR //

////////////////////////////////////////////////////////////

/**
 * Redimenciona una tabla de hash.
 *
 * @param tablaHash la tabla de hash
 * @param tam el nuevo tamaño
 * @deprecated
 */
void redimencionar(HashTable * tablaHash, int tam){
    tablaHash->tam = encontrarPrimoCercano(tam);
    tablaHash->b = encontrarPrimoCercano(tablaHash->tam + 1);
    tablaHash->c = encontrarPrimoCercano(tablaHash->b + 10);
    tablaHash->a = encontrarPrimoCercano(tablaHash->c + 100);
    tablaHash->tabla = realloc(tablaHash->tabla, tablaHash->tam);
}

////////////////////////////////////////////////////////////

// SECCION ELIMINAR //

////////////////////////////////////////////////////////////

/**
 * Elimina los nodos bajo una posición en una tabla de hash.
 *
 * @param el nodo a eliminar
 */
static void eliminarNodoHashTable(NodoTablaHash * nodo) {
	free(nodo->ruta_archivo);
	free(nodo->nombre_archivo);
	if (nodo->sigt != NULL) {
		eliminarNodoHashTable(nodo->sigt);
	}
	free(nodo);
}

/**
 * Elimina una tabla de hash.
 *
 * @param tablaHash la tabla de hash a eliminar
 */
void eliminarHashTable(HashTable * tablaHash) {
	for (int i = 0; i < tablaHash->tam; i++) {
		if (tablaHash->tabla[i] != NULL) {
			eliminarNodoHashTable(tablaHash->tabla[i]->inicio);
			free(tablaHash->tabla[i]);
		}
	}
	free(tablaHash->tabla);
	free(tablaHash);
}
