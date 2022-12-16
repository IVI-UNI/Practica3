/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "calcular_primos.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
contar_1(int nip, int min, int max,  CLIENT *clnt)
{
	contar_1_argument arg;
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.nip = nip;
	arg.min = min;
	arg.max = max;
	if (clnt_call (clnt, contar, (xdrproc_t) xdr_contar_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_int, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct encontrados *
buscar_1(int nip, int min, int max,  CLIENT *clnt)
{
	buscar_1_argument arg;
	static struct encontrados clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.nip = nip;
	arg.min = min;
	arg.max = max;
	if (clnt_call (clnt, buscar, (xdrproc_t) xdr_buscar_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_encontrados, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}