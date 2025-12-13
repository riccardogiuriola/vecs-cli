#include "cli.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int cli_parse_input(char *line, char ***argv_out) {
    int capacity = 10;
    int argc = 0;
    char **argv = malloc(sizeof(char*) * capacity);
    
    char *p = line;
    while (*p) {
        // Salta spazi iniziali
        while (*p && isspace(*p)) p++;
        if (!*p) break;

        // Trovato inizio token
        char *start = p;
        char *end = NULL;

        if (*p == '"') {
            // Gestione virgolette
            start++; // Salta la virgoletta aperta
            p++;
            // Cerca la virgoletta chiusa
            while (*p && *p != '"') p++;
            end = p;
            if (*p == '"') p++; // Salta la virgoletta chiusa
        } else {
            // Token normale fino al prossimo spazio
            while (*p && !isspace(*p)) p++;
            end = p;
        }

        // Estrai la stringa
        size_t len = end - start;
        char *arg = malloc(len + 1);
        memcpy(arg, start, len);
        arg[len] = '\0';

        // Aggiungi all'array
        if (argc >= capacity) {
            capacity *= 2;
            argv = realloc(argv, sizeof(char*) * capacity);
        }
        argv[argc++] = arg;
    }

    *argv_out = argv;
    return argc;
}

void cli_free_argv(int argc, char **argv) {
    for (int i = 0; i < argc; i++) free(argv[i]);
    free(argv);
}