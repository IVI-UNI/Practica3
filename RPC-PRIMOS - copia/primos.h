/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PRIMOS_H_RPCGEN
#define _PRIMOS_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct primos {
	int numero;
	int vector[5761456];
};
typedef struct primos primos;

struct contar_1_argument {
	int nip;
	int min;
	int max;
};
typedef struct contar_1_argument contar_1_argument;

struct buscar_1_argument {
	int nip;
	int min;
	int max;
};
typedef struct buscar_1_argument buscar_1_argument;

#define PRIMOS 11
#define UNO 1

#if defined(__STDC__) || defined(__cplusplus)
#define contar 1
extern  int * contar_1(int , int , int , CLIENT *);
extern  int * contar_1_svc(int , int , int , struct svc_req *);
#define buscar 2
extern  struct primos * buscar_1(int , int , int , CLIENT *);
extern  struct primos * buscar_1_svc(int , int , int , struct svc_req *);
extern int primos_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define contar 1
extern  int * contar_1();
extern  int * contar_1_svc();
#define buscar 2
extern  struct primos * buscar_1();
extern  struct primos * buscar_1_svc();
extern int primos_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_primos (XDR *, primos*);
extern  bool_t xdr_contar_1_argument (XDR *, contar_1_argument*);
extern  bool_t xdr_buscar_1_argument (XDR *, buscar_1_argument*);

#else /* K&R C */
extern bool_t xdr_primos ();
extern bool_t xdr_contar_1_argument ();
extern bool_t xdr_buscar_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PRIMOS_H_RPCGEN */
