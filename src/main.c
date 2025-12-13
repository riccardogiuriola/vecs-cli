#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    const char *host = CLI_DEFAULT_HOST;
    int port = CLI_DEFAULT_PORT;

    if (argc > 1) host = argv[1];
    if (argc > 2) port = atoi(argv[2]);

    printf("Connessione a %s:%d...\n", host, port);
    int fd = cli_connect(host, port);
    if (fd == -1) return 1;
    
    printf("Connesso a vecs. Digita 'exit' per uscire.\n");

    char line[CLI_BUFFER_SIZE];
    
    while (1) {
        printf("vecs> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) break;

        // Rimuovi newline finale
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) break;
        if (strlen(line) == 0) continue;

        // 1. Parsing
        int cmd_argc = 0;
        char **cmd_argv = NULL;
        cmd_argc = cli_parse_input(line, &cmd_argv);

        if (cmd_argc > 0) {
            // 2. Invio
            if (cli_send_command(fd, cmd_argc, cmd_argv) == 0) {
                // 3. Lettura Risposta
                if (cli_read_response(fd) == -1) {
                    printf("Server disconnesso.\n");
                    break;
                }
            } else {
                printf("Errore invio comando.\n");
                break;
            }
        }

        cli_free_argv(cmd_argc, cmd_argv);
    }

    close(fd);
    return 0;
}