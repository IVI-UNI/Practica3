#define TAM 5761456


struct primos {
    int numero;
    int vector[TAM];
};

program PRIMOS
{
    version UNO
    {
        int contar(int nip, int min, int max) = 1;
        struct primos buscar(int nip, int min, int max) = 2;
    } = 1;
} = 11;
