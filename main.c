#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *eliminarComentarios(const char *codigoOriginal)
{
    static char codigoLimpio[10000];
    int i = 0, j = 0;

    while (codigoOriginal[i] != '\0')
    {

        if (codigoOriginal[i] == '/' && codigoOriginal[i + 1] == '/')
        {
            i += 2;
            while (codigoOriginal[i] != '\n' && codigoOriginal[i] != '\0')
                i++;
        }

        else if (codigoOriginal[i] == '/' && codigoOriginal[i + 1] == '*')
        {
            i += 2;
            while (!(codigoOriginal[i] == '*' && codigoOriginal[i + 1] == '/') && codigoOriginal[i] != '\0')
                i++;
            if (codigoOriginal[i] != '\0')
                i += 2;
        }
        else
        {

            codigoLimpio[j++] = codigoOriginal[i++];
        }
    }

    codigoLimpio[j] = '\0';
    return codigoLimpio;
}

int analizarLexico(const char *codigo)
{
    for (int i = 0; codigo[i] != '\0'; i++)
    {
        if (!isalnum(codigo[i]) && !isspace(codigo[i]) && strchr("+-*/;(){}[]=<>!&|%,.\"\'\\", codigo[i]) == NULL)
        {
            return 0;
        }
    }
    return 1;
}

int analizarSintaxis(const char *codigo)
{
    int balanceParentesis = 0, balanceLlaves = 0;

    for (int i = 0; codigo[i] != '\0'; i++)
    {
        if (codigo[i] == '(')
            balanceParentesis++;
        else if (codigo[i] == ')')
            balanceParentesis--;
        else if (codigo[i] == '{')
            balanceLlaves++;
        else if (codigo[i] == '}')
            balanceLlaves--;

        if (balanceParentesis < 0 || balanceLlaves < 0)
            return 0;
    }

    return (balanceParentesis == 0 && balanceLlaves == 0);
}

int analizarSemantica(const char *codigo)
{
    int i = 0;
    int longitud = strlen(codigo);

    while (i < longitud)
    {
        if (strstr(codigo + i, "int ") != NULL || strstr(codigo + i, "void ") != NULL || strstr(codigo + i, "char ") != NULL || strstr(codigo + i, "float ") != NULL)
        {
            i += 4;

            while (isspace(codigo[i]))
                i++;

            if (!isalpha(codigo[i]))
            {
                printf("Error semántico: falta un identificador válido después de 'int'.\n");
                return 0;
            }

            while (isalnum(codigo[i]))
                i++;

            while (isspace(codigo[i]))
                i++;

            if (codigo[i] != '(')
            {
                printf("Error semántico: falta '(' después del nombre de la función.\n");
                return 0;
            }
            i++;

            while (codigo[i] != ')' && i < longitud)
                i++;

            if (codigo[i] != ')')
            {
                printf("Error semántico: falta ')' para cerrar los parámetros.\n");
                return 0;
            }
            i++;

            while (isspace(codigo[i]))
                i++;

            if (codigo[i] != '{')
            {
                printf("Error semántico: falta '{' para iniciar el bloque de la función.\n");
                return 0;
            }
            i++;

            while (codigo[i] != '}' && i < longitud)
                i++;

            if (codigo[i] != '}')
            {
                printf("Error semántico: falta '}' para cerrar el bloque de la función.\n");
                return 0;
            }
            i++;

            break;
        }

        i++;
    }

    if (i == longitud)
    {
        printf("Error semántico: no se detectó una estructura válida de función 'int main() { ... }'.\n");
        return 0;
    }

    return 1;
}

int main()
{
    printf("            Compilador básico en C            ");
    char codigo[10000] = ""; // Acumula el código ingresado por el usuario
    char linea[1024];        // Almacena temporalmente la línea ingresada

    while (1)
    {
        printf("\n...............................................\n");
        printf("\nEscriba su código (o escriba 'salir' para terminar):\n\n");

        fgets(linea, sizeof(linea), stdin);
        linea[strcspn(linea, "\n")] = '\0'; // Elimina el carácter de nueva línea

        if (strcmp(linea, "salir") == 0)
        {
            break;
        }

        // Concatenar la nueva línea al código acumulado
        if (strlen(codigo) + strlen(linea) + 1 < sizeof(codigo))
        {
            strcat(codigo, linea);
            strcat(codigo, "\n"); // Agrega un salto de línea después de cada entrada
        }
        else
        {
            printf("Error: El código acumulado excede el tamaño permitido.\n");
            break;
        }

        // Procesar el código acumulado
        char *codigoSinComentarios = eliminarComentarios(codigo);

        printf("\n");

        // Analizar léxico
        if (!analizarLexico(codigoSinComentarios))
        {
            printf("El código contiene errores léxicos.\n");
            continue;
        }
        else
        {
            printf("El código no contiene errores léxicos.\n");
        }

        // Analizar sintaxis
        if (!analizarSintaxis(codigoSinComentarios))
        {
            printf("El código contiene errores de sintaxis.\n");
            continue;
        }
        else
        {
            printf("El código no contiene errores de sintaxis.\n");
        }

        // Analizar semántica
        if (!analizarSemantica(codigoSinComentarios))
        {
            printf("El código contiene errores semánticos.\n");
            continue;
        }
        else
        {
            printf("El código no contiene errores semánticos.\n");
        }

        printf("El código ingresado hasta ahora es válido.\n");
    }

    printf("\nPresione Enter para salir...");
    getchar();
    return 0;
}