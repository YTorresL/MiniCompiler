#include <stdio.h>

int main()
{
    while (1)
    {

        char archivo[100];
        printf("Ingrese el nombre del archivo: ");
        scanf("%s", archivo);

        if (lexicon(archivo) == 1)
        {
            continue;
        }

        if (syntactic(archivo) == 1)
        {
            continue;
        }

        if (semantic(archivo) == 1)
        {
            continue;
        }

        printf("El archivo no tiene errores\n");
        break;
    }
    return 0;
}