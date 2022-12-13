/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "primos.h"


void
primos_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	int contar_1_nip;
	int contar_1_min;
	int contar_1_max;
	struct primos  *result_2;
	int buscar_1_nip;
	int buscar_1_min;
	int buscar_1_max;

#ifndef	DEBUG
	clnt = clnt_create (host, PRIMOS, UNO, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = contar_1(contar_1_nip, contar_1_min, contar_1_max, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = buscar_1(buscar_1_nip, buscar_1_min, buscar_1_max, clnt);
	if (result_2 == (struct primos *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	primos_1 (host);
exit (0);
}
