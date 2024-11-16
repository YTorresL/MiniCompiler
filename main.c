#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
int lexicon(char *archivo);
int semantic(char *archivo);
int syntactic(char *archivo);

int lexicon(char *archivo)
{
    printf("\n.........................................Realizando análisis léxico.........................................\n");
    printf("\n");
    FILE *file = fopen(archivo, "r");

    if (file == NULL)
    {
        printf("No se ha encontrado el archivo\n");
        return 1;
    }

    char word[100];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Detectar directivas preprocesador (#include)
        if (word[0] == '#')
        {
            printf("Directiva del preprocesador: %s\n", word);
        }
        // Detectar cabeceras como <stdio.h>
        else if (word[0] == '<' && word[strlen(word) - 1] == '>')
        {
            printf("Cabecera: %s\n", word);
        }
        // Palabras reservadas
        else if (strcmp(word, "int") == 0 || strcmp(word, "float") == 0 || strcmp(word, "char") == 0 ||
                 strcmp(word, "if") == 0 || strcmp(word, "else") == 0 || strcmp(word, "while") == 0 ||
                 strcmp(word, "return") == 0 || strcmp(word, "break") == 0 || strcmp(word, "continue") == 0)
        {
            printf("Palabra reservada: %s\n", word);
        }
        // Literales de cadenas
        else if (word[0] == '"' && word[strlen(word) - 1] == '"')
        {
            printf("Literal de cadena: %s\n", word);
        }
        // Números
        else if (isdigit(word[0]) || (word[0] == '-' && isdigit(word[1])))
        {
            printf("Número: %s\n", word);
        }
        // Operadores comunes
        else if (strcmp(word, "+") == 0 || strcmp(word, "-") == 0 || strcmp(word, "*") == 0 || strcmp(word, "/") == 0 ||
                 strcmp(word, "==") == 0 || strcmp(word, "!=") == 0 || strcmp(word, "<") == 0 || strcmp(word, ">") == 0 ||
                 strcmp(word, "<=") == 0 || strcmp(word, ">=") == 0 || strcmp(word, "=") == 0)
        {
            printf("Operador: %s\n", word);
        }
        // Delimitadores y caracteres especiales
        else if (strcmp(word, ";") == 0 || strcmp(word, "{") == 0 || strcmp(word, "}") == 0 || strcmp(word, ",") == 0)
        {
            printf("Delimitador: %s\n", word);
        }
        // Parentesis dectectar si hay información dentro de los parentesis
        else if (word[0] == '(' || word[0] == ')')
        {
            printf("Parentesis: %s\n", word);
        }
        // Identificadores
        else if (isalpha(word[0]) || word[0] == '_')
        {
            printf("Identificador: %s\n", word);
        }
        // Tokens desconocidos
        else
        {
            printf("\nToken desconocido o error léxico: %s\n", word);
            printf("\n");
        }
    }

    fclose(file);
    return 0;
}

int syntactic(char *archivo)
{
    printf(".........................................Realizando análisis sintáctico.........................................\n");

    FILE *file = fopen(archivo, "r");
    if (file == NULL)
    {
        printf("No se ha encontrado el archivo\n");
        return 1;
    }

    char word[100];

    while (fscanf(file, "%s", word) != EOF)
    {
        // Detectar directivas del preprocesador
        if (word[0] == '#')
        {
            if (strcmp(word, "#include") == 0 || strcmp(word, "#define") == 0)
            {
                printf("Directiva del preprocesador: %s\n", word);
                continue;
            }
            else
            {
                printf("Error sintáctico en: %s\n", word);
                continue;
            }
        }

        // Ejemplo: Validar estructura básica del programa
        if (strcmp(word, "int") == 0)
        {
            fscanf(file, "%s", word); // Leer el siguiente token
            if (strcmp(word, "main") == 0)
            {
                printf("Definición de función principal detectada.\n");
                fscanf(file, "%s", word); // Leer el siguiente token
                if (strcmp(word, "()") == 0 || strcmp(word, "(") == 0)
                {
                    printf("Inicio de definición de función válida.\n");
                }
                else
                {
                    printf("Error sintáctico en: %s\n", word);
                }
            }
        }

        // Manejar delimitadores
        if (strcmp(word, "{") == 0 || strcmp(word, "}") == 0)
        {
            printf("Delimitador detectado: %s\n", word);
            continue;
        }

        // Validar palabras clave y estructuras
        if (strcmp(word, "while") == 0 || strcmp(word, "if") == 0 || strcmp(word, "else") == 0)
        {
            printf("Palabra reservada detectada: %s\n", word);
            continue;
        }

        // Detectar errores en tokens no esperados
        printf("Error sintáctico en: %s\n", word);
    }

    fclose(file);
    return 0;
}

// Función de verificación semántica: tipos básicos (ejemplo simple)
int semantic(char *archivo)
{
    FILE *file = fopen(archivo, "r");
    if (file == NULL)
    {
        printf("No se ha encontrado el archivo\n");
        return 1;
    }

    char word[100];
    int tipo_definido = 0;

    while (
        fscanf(file, "%s", word) != EOF)
    {
        if (strcmp(word, "int") == 0 || strcmp(word, "float") == 0 || strcmp(word, "char") == 0)
        {
            tipo_definido = 1;
        }
        else if (strcmp(word, "main") == 0)
        {
            if (tipo_definido == 0)
            {
                printf("Error semántico: Falta definir el tipo de dato\n");
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

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

    printf("Presione Enter para salir...");
    getchar();
    getchar();

    return 0;
}
