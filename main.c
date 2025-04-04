/// @file main.c

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "text.c"
#include "hash.c"
#include "list.c"
#include "btree.c"
#include "splay.c"
#include "index.c"
#include "io.c"

#ifndef MAX_BUF
#define MAX_BUF 96
#endif

/**
 * La función main (nada más activa el menú).
 *
 * @return el código de éxito
 */
int main() {
	char addText[16] = "/hoy_en_el_tec/";
	char path[MAX_BUF] = "";
	getcwd(path, MAX_BUF);
	int pt1 = 0, pt2 = 0;
	while (path[pt1++]);
	pt1--;
	while (addText[pt2]) path[pt1++] = addText[pt2++];
	path[pt1] = 0;
	menu(path);
	return 0;
}
