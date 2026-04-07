#ifndef SERVER_H
#define SERVER_H

#define PORT        8080
#define BUFFER_SIZE 4096
#define BACKLOG     10

/* Structure représentant le serveur */
typedef struct {
    int socket_fd;   /* descripteur du socket d'écoute */
    int port;        /* port d'écoute */
} Server;

/* Crée et configure le socket du serveur.
 * Retourne 0 si succès, -1 si erreur. */
int server_init(Server *srv);

/* Boucle principale : attend et traite les connexions entrantes. */
void server_run(Server *srv);

/* Ferme le socket proprement. */
void server_stop(Server *srv);

#endif /* SERVER_H */
