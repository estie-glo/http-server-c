#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../include/server.h"
#include "../include/router.h"

/* Serveur global pour pouvoir l'arrêter proprement depuis le handler SIGINT */
static Server srv;

/* Handler du signal CTRL+C : arrête le serveur proprement */
static void handle_sigint(int sig) {
    (void)sig; /* évite le warning "paramètre non utilisé" */
    printf("\n\nArrêt du serveur...\n");
    server_stop(&srv);
    exit(0);
}

int main(void) {
    /* 1. Intercepter CTRL+C pour fermer proprement le socket */
    signal(SIGINT, handle_sigint);

    printf("=================================\n");
    printf("  Mini HTTP Server en C v1.0.0  \n");
    printf("  Auteure : Wandji Nguele Estelle\n");
    printf("=================================\n\n");

    /* 2. Enregistrer toutes les routes */
    router_init();
    printf("\n");

    /* 3. Initialiser le serveur (socket, bind, listen) */
    if (server_init(&srv) < 0) {
        fprintf(stderr, "Impossible de démarrer le serveur.\n");
        return EXIT_FAILURE;
    }

    /* 4. Lancer la boucle principale (bloquant) */
    server_run(&srv);

    return EXIT_SUCCESS;
}
