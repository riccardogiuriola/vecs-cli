#ifndef VECS_CLI_H
#define VECS_CLI_H

#include <stddef.h>

#define CLI_DEFAULT_HOST "127.0.0.1"
#define CLI_DEFAULT_PORT 6380
#define CLI_BUFFER_SIZE 4096

// --- network.c ---
int cli_connect(const char *host, int port);

// --- parser.c ---
// Parsa la stringa utente gestendo le virgolette.
// Ritorna il numero di argomenti e popola argv (che va liberato).
int cli_parse_input(char *line, char ***argv);
void cli_free_argv(int argc, char **argv);

// --- resp.c ---
// Invia i dati formattati RESP e stampa la risposta
int cli_send_command(int fd, int argc, char **argv);
int cli_read_response(int fd);

#endif