#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/router.h"

/* ─── Table de routes globale ────────────────────────────────────────────── */

Route routes[MAX_ROUTES];
int   route_count = 0;

/* ─── Helpers : construction des réponses HTTP ───────────────────────────── */

/* Envoie une réponse HTTP complète au client.
 * status  : ex "200 OK", "404 Not Found"
 * body    : contenu JSON à envoyer */
static void send_response(int client_fd, const char *status, const char *body) {
    char response[4096];

    /* Une réponse HTTP = ligne de statut + headers + ligne vide + body */
    snprintf(response, sizeof(response),
        "HTTP/1.1 %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"                   /* ligne vide obligatoire entre headers et body */
        "%s",
        status,
        strlen(body),
        body
    );

    write(client_fd, response, strlen(response));
}

/* ─── Handlers des routes ────────────────────────────────────────────────── */

/* GET / → message de bienvenue */
static void handle_root(int client_fd) {
    const char *body =
        "{\n"
        "  \"message\": \"Bienvenue sur le serveur HTTP en C\",\n"
        "  \"version\": \"1.0.0\"\n"
        "}";
    send_response(client_fd, "200 OK", body);
}

/* GET /status → état du serveur */
static void handle_status(int client_fd) {
    const char *body =
        "{\n"
        "  \"status\": \"running\",\n"
        "  \"language\": \"C\",\n"
        "  \"author\": \"Wandji Nguele Estelle\"\n"
        "}";
    send_response(client_fd, "200 OK", body);
}

/* GET /hello → salutation */
static void handle_hello(int client_fd) {
    const char *body =
        "{\n"
        "  \"message\": \"Hello, World!\",\n"
        "  \"description\": \"Mini serveur HTTP REST ecrit en C\"\n"
        "}";
    send_response(client_fd, "200 OK", body);
}

/* Handler 404 : aucune route trouvée */
static void handle_not_found(int client_fd) {
    const char *body =
        "{\n"
        "  \"error\": \"404 Not Found\",\n"
        "  \"message\": \"La route demandee n'existe pas\"\n"
        "}";
    send_response(client_fd, "404 Not Found", body);
}

/* ─── Enregistrement des routes ──────────────────────────────────────────── */

void router_add(const char *method, const char *path, RouteHandler handler) {
    if (route_count >= MAX_ROUTES) {
        fprintf(stderr, "Erreur : nombre maximum de routes atteint (%d)\n", MAX_ROUTES);
        return;
    }

    /* Copie sécurisée avec strncpy pour éviter les débordements de buffer */
    strncpy(routes[route_count].method,  method,  sizeof(routes[route_count].method)  - 1);
    strncpy(routes[route_count].path,    path,    sizeof(routes[route_count].path)    - 1);
    routes[route_count].handler = handler;

    route_count++;
    printf("Route enregistrée : %s %s\n", method, path);
}

/* ─── Dispatch : cherche et appelle le bon handler ───────────────────────── */

void router_dispatch(int client_fd, const char *method, const char *path) {
    int i;

    for (i = 0; i < route_count; i++) {
        if (strcmp(routes[i].method, method) == 0 &&
            strcmp(routes[i].path,   path)   == 0) {
            /* Route trouvée : on appelle son handler via le pointeur de fonction */
            routes[i].handler(client_fd);
            return;
        }
    }

    /* Aucune route trouvée → 404 */
    handle_not_found(client_fd);
}

/* ─── Initialisation de toutes les routes de l'application ──────────────── */

void router_init(void) {
    router_add("GET", "/",       handle_root);
    router_add("GET", "/status", handle_status);
    router_add("GET", "/hello",  handle_hello);
}
