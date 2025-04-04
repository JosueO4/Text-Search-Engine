/// @file text.c

/**
 * Saca la longitud de un texto.
 *
 * @param in el texto
 * @return la longitud
 */
int len(char * in) {
	int l = 0;
	while (*in++) l++;
	return l;
}

/**
 * Elimina un texto en una lista (función para mapeo).
 *
 * @param valor el texto
 */
void eliminarNodoTexto(void * valor) {
	free((char *)valor);
}

/**
 * Guarda un texto en el heap.
 *
 * @param buffer el texto
 * @return un puntero en el heap
 */
char * moverTexto(char * buffer) {	
	int longitud = len(buffer);
	char * texto = calloc(longitud + 1, sizeof(char));
	while (*buffer) *texto++ = *buffer++;
	return texto - longitud;
}

/**
 * Compara dos strings.
 * 
 * @param v1 el texto 1
 * @param v2 el texto 2
 * @return 0 si son diferentes
 */
char compararStrings(void * v1, void * v2) {
	char * str1 = (char *)v1;
	char * str2 = (char *)v2;
	int cnt = 0;
	while (str1[cnt] && str2[cnt] && str1[cnt] == str2[cnt]) cnt++;
	return str1[cnt] == str2[cnt];
}

/**
 * Imprime un texto de una lista (función para mapeo).
 *
 * @param valor el texto
 */
void imprimirNodoTexto(void * valor) {
	printf("%s, ", (char *)valor);
}

/**
 * Imprime un texto de una lista (función para mapeo).
 *
 * @param valor el texto
 */
void imprimirNodoTextoLinea(void * valor) {
	printf("%s\n", (char *)valor);
}

static const int SETA = 79719271, SETB = 34674803, SETC = 49616837; /**< constantes de hasheo */

/**
 * Saca el hash de un entero.
 *
 * @param n el entero
 * @return el hash
 */
static int textHash(int n) {
	return (SETA*n + SETB)%SETC;
}

/**
 * Convierte un texto a un entero.
 *
 * @param texto el texto
 * @return el hash del texto
 */
int stringToHash(char * texto) {
	int llave = 0;
	while (*texto) {
		llave += *texto++;
		llave = textHash(llave);
	}
	return llave;
}
