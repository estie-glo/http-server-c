#ifndef ROUTER_H
#define ROUTER_H

#define MAX_ROUTES  10
#define MAX_PATH    128

/* Type de fonction handler : reçoit le socket client,
 * écrit la réponse HTTP dedans. */
typedef void (*RouteHandler)(int client_fd);

/* Structure représentant une route */
typedef struct {
    char         method[8];          /* "GET", "POST", etc. */
    char         path[MAX_PATH];     /* "/", "/status", "/hello" */
    RouteHandler handler;            /* fonction à appeler */
} Route;

/* Table de routes globale */
extern Route routes[MAX_ROUTES];
extern int   route_count;

/* Enregistre une nouvelle route. */
void router_add(const char *method, const char *path, RouteHandler handler);

/* Cherche la route correspondant à method + path et appelle son handler.
 * Si aucune route trouvée, envoie une réponse 404. */
void router_dispatch(int client_fd, const char *method, const char *path);

#endif /* ROUTER_H */
