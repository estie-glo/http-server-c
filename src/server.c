#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../include/server.h"
#include "../include/router.h"

/* ─── Initialisation du serveur ─────────────────────────────────────────── */

int server_init(Server *srv) {
    struct sockaddr_in addr;
    int opt = 1;

    srv->port = PORT;

    /* 1. Créer le socket
     *    AF_INET     = IPv4
     *    SOCK_STREAM = TCP (connexion fiable, ordonnée)
     *    0           = protocole par défaut */
    srv->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv->socket_fd < 0) {
        perror("Erreur : création du socket");
        return -1;
    }

    /* 2. Option SO_REUSEADDR : permet de relancer le serveur rapidement
     *    sans attendre que l'OS libère le port */
    if (setsockopt(srv->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Erreur : setsockopt");
        return -1;
    }

    /* 3. Configurer l'adresse d'écoute */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;        /* IPv4 */
    addr.sin_addr.s_addr = INADDR_ANY;     /* accepte toutes les interfaces */
    addr.sin_port        = htons(srv->port); /* port en big-endian réseau */

    /* 4. Attacher le socket à l'adresse/port */
    if (bind(srv->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Erreur : bind");
        return -1;
    }

    /* 5. Mettre le socket en écoute (BACKLOG = nb max de connexions en attente) */
    if (listen(srv->socket_fd, BACKLOG) < 0) {
        perror("Erreur : listen");
        return -1;
    }

    printf("Serveur démarré sur http://localhost:%d\n", srv->port);
    return 0;
}

/* ─── Parsing de la requête HTTP ─────────────────────────────────────────── */

/* Extrait la méthode et le chemin depuis la première ligne HTTP.
 * Ex: "GET /hello HTTP/1.1" → method="GET", path="/hello" */
static void parse_request(const char *buffer, char *method, char *path) {
    /* La première ligne d'une requête HTTP est : METHODE CHEMIN VERSION */
    sscanf(buffer, "%7s %127s", method, path);
}

/* ─── Boucle principale ──────────────────────────────────────────────────── */

void server_run(Server *srv) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char method[8];
    char path[128];

    printf("En attente de connexions...\n\n");

    while (1) {
        /* 6. Accepter une connexion entrante (bloquant : attend qu'un client arrive) */
        client_fd = accept(srv->socket_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Erreur : accept");
            continue; /* on ne quitte pas, on attend la prochaine connexion */
        }

        /* Afficher l'IP du client connecté */
        printf("Connexion de : %s\n", inet_ntoa(client_addr.sin_addr));

        /* 7. Lire la requête envoyée par le client */
        memset(buffer, 0, BUFFER_SIZE);
        if (read(client_fd, buffer, BUFFER_SIZE - 1) < 0) {
            perror("Erreur : read");
            close(client_fd);
            continue;
        }

        /* 8. Parser la méthode et le chemin */
        memset(method, 0, sizeof(method));
        memset(path,   0, sizeof(path));
        parse_request(buffer, method, path);

        printf("→ %s %s\n", method, path);

        /* 9. Dispatcher vers le bon handler */
        router_dispatch(client_fd, method, path);

        /* 10. Fermer la connexion avec ce client */
        close(client_fd);
    }
}

/* ─── Arrêt du serveur ───────────────────────────────────────────────────── */

void server_stop(Server *srv) {
    close(srv->socket_fd);
    printf("Serveur arrêté.\n");
}
