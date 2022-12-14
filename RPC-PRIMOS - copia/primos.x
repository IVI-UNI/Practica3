#define TAM 5761456


struct encontrados {
    int nPrimos;
    int vectorPrimos[TAM];
};

program PRIMOS
{
    version UNO
    {
        int contar(int nip, int min, int max) = 1;
        struct encontrados buscar(int nip, int min, int max) = 2;
    } = 1;
} = 11;
