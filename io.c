/// @file io.c

/**
 * Imprima la ruta original de un texto (con la tabla de hash).
 *
 * @param hashTable la tabla de hash
 * @param texto el texto llave
 */
void mostrarRutaOriginal(void * hashTable, char * texto) {
	printf(" --> %s\n", accederTextoHashTable((HashTable *)hashTable, texto));
}

/**
 * Imprimir el valor en un arbol b.
 *
 * @param valor el valor del arbol b
 */
void imprimirValoresBTree(ValorBTree * valor) {
	printf("<-Imprimiendo ValorBTree con llave \"%s\" (hash: %d)->\n", valor->texto, valor->hash);
	mapLista(valor->asociados, imprimirNodoTexto);
	printf("\n");
	if (valor->colision) imprimirValoresBTree(valor->colision);
	printf("<-------->\n");
}

/**
 * Imprimir los nodos del arbol b.
 *
 * @param nodo nodo del arbol b
 * @param nivel nivel en el que se encuentra
 */
void imprimirNodosBTree(NodoBTree * nodo, int nivel) {
	printf("<--Imprimiendo NodoBTree en nivel %d-->\n", nivel);
	for (int i = 0; i < nodo->cantidad; i++)
		imprimirValoresBTree(nodo->valores[i]);
	if (nodo->hijos) 
		for (int i = 0; i < nodo->cantidad + 1; i++)
			imprimirNodosBTree(nodo->hijos[i], nivel + 1);
	printf("<------------>\n");
}

/**
 * Imprimir el arbol.
 *
 * @param tree el arbol que se imprime
 */
void imprimirBTree(BTree * tree) {
	printf("<---Imprimiendo BTree en %p--->\n", (void *)tree);
	if (tree->tree) imprimirNodosBTree(tree->tree, 0);
	printf("<---------------->\n");
}

/**
 * Hacer el recorrido en preorden de un chaflaneado (Auxiliar).
 *
 * @param nodo_arbol el nodo raiz por donde se empieza el recorrido
 */
static void recorridoPreordenChaflaneadoAux(NodoSplayTree * raiz){
	//imprime raiz, izquierdo, derecho
	if(raiz != NULL){
		printf("<--Imprimiendo siguiente nodo con hash: %d-->\n", raiz->hash);
		SubNodoSplayTree * act = raiz->colision;
		while (act) {
			printf("<-Imprimiendo contenidos del subnodo \"%s\"->\n", act->texto);
			mapLista(act->valores, imprimirNodoTexto);
			act = act->colision;
			printf("\n<-------->\n");
			}
		recorridoPreordenChaflaneadoAux(raiz->hijo_izq); //va por todos los izquierdos
		recorridoPreordenChaflaneadoAux(raiz->hijo_der); //va por todos los derechos
		printf("<------------>\n");
		}
	}

/**
 * Hacer el recorrido en preorden de un chaflaneado.
 *
 * @param edd Estructura de datos, puntero al arbol chaflaneado
 */
void recorridoPreordenChaflaneado(SplayTree * edd) {
	printf("<--Empezando recorrido en preorden del SplayTree en %p-->\n", (void *)edd);
	recorridoPreordenChaflaneadoAux(edd->tree);
	printf("<----------------->\n");
}

/**
 * Imprimir una tabla de hash.
 *
 * @param tablaHash la tabla que se imprime
 */
void imprimirHashTable(HashTable * tablaHash){
    printf("<--Imprimiendo HashTable de rutas en %p-->\n", (void *)tablaHash);

    for(int pos = 0;pos < tablaHash->tam; pos++){ // Se recorre la tabla de hash vieja //

        if(tablaHash->tabla[pos]){ // Si hay algo adentro //

            NodoTablaHash * act = tablaHash->tabla[pos]->inicio; // Se define act como el primer nodo de ese espacio //

            char * nombre = act->nombre_archivo; // Nombre del archivo //

            char * ruta = act->ruta_archivo; // Ruta del archivo //

            if(act->sigt != NULL){ // Si el espacio esta colisionado significa que hay mas de un nodo //

                while(act!= NULL){ // Mientras no se haya acabado la lista de los nodos en esa posicion

                    char* nombre = act->nombre_archivo; // Nombre del archivo //

                    char* ruta = act->ruta_archivo; // Ruta del archivo //

                    printf("=======================================================================\n");
                    printf("\n");
                    printf("Indice: %d\n",pos);
                    printf("\n");
                    printf("Nombre del archivo: %s\n",nombre);
                    printf("\n");
                    printf("Ruta del archivo: %s\n",ruta);
                    printf("\n");
                    printf("=======================================================================\n");
                    printf("\n");
                    printf("\n");
                    act = act->sigt; // Se avanza en la lista //
                }
            }
            else{ // Si solo es un nodo //
                printf("=======================================================================\n");
                printf("\n");
                printf("Indice: %d\n",pos);
                printf("\n");
                printf("Nombre del archivo: %s\n",nombre);
                printf("\n");
                printf("Ruta del archivo: %s\n",ruta);
                printf("\n");
                printf("=======================================================================\n");
                printf("\n");
                printf("\n");
            }
        }
    }
    printf("<----------------->\n");
}

/**
 * Interfaz de línea de comandos, controla todo. 
 */
void menu(char * path) {
	char correr = 1;
	char opcion;
	int cant_palabras;

	char buffer[32];

	void ** salida;

	BTree * tree = crearBTree();
	HashTable * hashTable = crearHashTable(123);
	SplayTree * splay = crearSplayTree();
	while(correr) {
		printf("=====================BUSCADOR DE INFORMACION==================================== \n\n\n\n");
		printf("Elija una opcion: \n\n"
			" 1. Indizar \n 2. Cargar indice \n 3. Guardar Indice \n "
			"4. Buscar \n 5. Mostrar Árbol Chaflaneado \n 6. Mostrar HashTable \n 7. Mostrar BTree \n 8. Salir \n ");
		printf("\n> Opción: \n $ ");
		
		
		do {
			opcion = getchar();	
		} while(opcion == '\n');
		opcion -= '0';
		//printf("%d",opcion);
		if(getchar() != '\n'){
			while(getchar()!='\n');
			printf(" >>>! Opción inválida, intente de nuevo. \n\n\n\n");
			continue;
		}
		printf("\n");
		switch(opcion) {
			case 1:
				salida = indexar(path);
				eliminarHashTable(hashTable);
				hashTable = salida[0];
				eliminarBTree(tree);
				tree = salida[1];
				free(salida);
				eliminarSplayTree(splay);
				splay = crearSplayTree();
				//la va a buscar
				printf(" >>>* Se logró crear el índice!\n\n");
				break;
			case 2:
				//cargar indice de max
				printf("* es su culpa si el programa deja de correr porque le metiste un mal archivo ;D\n");
				printf(" > Ingrese el camino:\n $ ");
				scanf(" %255[^\n]", buffer);
				salida = loadData(buffer);
				if (salida) {
					printf("\n >>>* Se logró cargar!\n\n");
					eliminarHashTable(hashTable);
					hashTable = salida[1];
					eliminarBTree(tree);
					tree = salida[0];
					eliminarSplayTree(splay);
					splay = crearSplayTree();
					free(salida);
				} else printf("\n >>>! No se logró cargar...\n\n");
				break;
			case 3:
				//guardar indice de max
				printf("> Ingrese el camino:\n $ ");
				scanf(" %255[^\n]", buffer);
				saveData(buffer, tree, hashTable);
				printf("\n >>>* Se guardó el BTree y el HashTable!\n\n");
				break;
			case 4:
				//buscar ???
				//cantidad de palabras
				printf("> Ingrese la cantidad de palabras a buscar (entre 1 y 9):\n $ ");
				cant_palabras = getchar() - '0';
				if(getchar() != '\n'){
					while(getchar()!='\n');
					printf(" >>>! Cantidad inválida profe ._. \n\n");
					continue;
				}
				if(cant_palabras<=0 || cant_palabras>9){
					printf(" >>>! Cantidad inválida profe ._. \n\n");
					continue;
				}
				char modificado = 0;
				Lista * palabras = crearLista();
				for(int i = 1; i <= cant_palabras;i++){
					printf("> Ingrese la palabra %d:\n $ ", i);
					scanf(" %31[^\n]", buffer);
					Lista * intento = buscarChaflaneado(splay, buffer);
					if (!intento) {
						intento = buscarBTree(tree, buffer);
						if (!intento) {
							printf("\n >>>! La palabra \"%s\" no existe D;\n\n", buffer);
							if (palabras) eliminarLista(palabras);
							break;
						}
						insertarChaflaneado(splay, buffer, intento);
					}
					if (!modificado) {
						palabras = intento;
						modificado = 1;
					} else if (modificado == 1) {
						modificado = 2;
						palabras = intersecListaTexto(palabras, intento);
					} else {
						intento = intersecListaTexto(palabras, intento);
						free(palabras);
						palabras = intento;
					}
				}
				if (palabras->longitud) printf("\n >>>* Su búsqueda tuvo el siguiente resultado:\n");
				else printf("\n >>>! No habían coincidencias D;\n");
				mapListaConAdicional(hashTable, palabras, mostrarRutaOriginal);
				if (modificado == 2) eliminarLista(palabras);
				printf("\n\n");
				break;
			case 5:
				recorridoPreordenChaflaneado(splay);
				break;
			case 6:
				imprimirHashTable(hashTable);
				break;
			case 7:
				imprimirBTree(tree);
				break;
			case 8:
				printf(" >>> Gracias por usar el sistema. \n\n\n");
				correr = 0;
				break;
			default:
				printf(" >>> Opción inválida, intente de nuevo. \n\n\n");
		}
	}
	eliminarBTree(tree);
	eliminarSplayTree(splay);
	eliminarHashTable(hashTable);
}
