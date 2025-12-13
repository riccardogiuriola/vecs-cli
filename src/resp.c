#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Helper per inviare dati
static int send_bytes(int fd, const char *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = write(fd, buf + total, len - total);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

int cli_send_command(int fd, int argc, char **argv) {
    char buf[128];
    
    // 1. Invia Header Array (*N\r\n)
    snprintf(buf, sizeof(buf), "*%d\r\n", argc);
    if (send_bytes(fd, buf, strlen(buf)) == -1) return -1;

    // 2. Invia ogni argomento ($Len\r\nData\r\n)
    for (int i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);
        snprintf(buf, sizeof(buf), "$%zu\r\n", len);
        if (send_bytes(fd, buf, strlen(buf)) == -1) return -1;
        if (send_bytes(fd, argv[i], len) == -1) return -1;
        if (send_bytes(fd, "\r\n", 2) == -1) return -1;
    }
    return 0;
}

// Lettura semplice riga per riga (Bufferizzata minimalmente)
static ssize_t readline(int fd, char *buf, size_t maxlen) {
    size_t i = 0;
    char c;
    while (i < maxlen - 1) {
        ssize_t n = read(fd, &c, 1);
        if (n <= 0) return -1; // Errore o chiusura
        buf[i++] = c;
        if (c == '\n') break;
    }
    buf[i] = '\0';
    return i;
}

int cli_read_response(int fd) {
    char line[CLI_BUFFER_SIZE];
    
    // Leggi il primo byte per capire il tipo
    if (readline(fd, line, sizeof(line)) <= 0) return -1;

    char type = line[0];
    
    // Rimuovi \r\n finale per pulizia stampa
    line[strcspn(line, "\r\n")] = 0;

    switch (type) {
        case '+': // Simple String (+OK)
            printf("%s\n", line + 1);
            break;
        case '-': // Error (-ERR ...)
            printf("(error) %s\n", line + 1);
            break;
        case ':': // Integer (:1)
            printf("(integer) %s\n", line + 1);
            break;
        case '$': // Bulk String ($5\r\nhello)
        {
            int len = atoi(line + 1);
            if (len == -1) {
                printf("(nil)\n");
            } else {
                char *data = malloc(len + 2 + 1); // +2 per CRLF
                int nread = 0;
                while (nread < len + 2) {
                    int r = read(fd, data + nread, (len + 2) - nread);
                    if (r <= 0) { free(data); return -1; }
                    nread += r;
                }
                data[len] = '\0'; // Tronca il CRLF finale
                printf("\"%s\"\n", data);
                free(data);
            }
            break;
        }
        default:
            printf("Protocollo sconosciuto: %s\n", line);
    }
    return 0;
}