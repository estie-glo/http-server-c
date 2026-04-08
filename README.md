# Mini HTTP Server en C

Un serveur HTTP RESTful minimal écrit en C pur, sans bibliothèque externe.

## Fonctionnalités

- Écoute les requêtes HTTP sur le port 8080
- Routing dynamique via pointeurs de fonction
- Réponses au format JSON
- Gestion propre des signaux (CTRL+C)
- Architecture modulaire (server / router)

## Structure du projet

```
http_server/
├── src/
│   ├── main.c       # Point d'entrée
│   ├── server.c     # Socket, bind, listen, accept
│   └── router.c     # Routes et réponses JSON
├── include/
│   ├── server.h     # Interface du serveur
│   └── router.h     # Interface du routeur
├── Makefile
└── README.md
```

## Prérequis

- GCC
- Linux / macOS (Ubuntu recommandé)
- Make

## Compilation & lancement

```bash
# Compiler
make

# Lancer le serveur
make run

# Ou directement
./http_server
```

## Routes disponibles

| Méthode | Route     | Description              |
|---------|-----------|--------------------------|
| GET     | /         | Message de bienvenue     |
| GET     | /status   | État du serveur          |
| GET     | /hello    | Salutation               |

## Tester avec curl

```bash
curl http://localhost:8080/
curl http://localhost:8080/status
curl http://localhost:8080/hello
curl http://localhost:8080/inexistant   # → 404
```

## Exemple de réponse

```json
{
  "status": "running",
  "language": "C",
  "author": "Wandji Nguele Estelle"
}
```

## Concepts illustrés

- **Sockets TCP** : création, binding, écoute et acceptation de connexions
- **Protocole HTTP** : parsing de requêtes, construction de réponses
- **Pointeurs de fonction** : dispatch dynamique des routes
- **Gestion mémoire** : pas de fuite, fermeture propre des descripteurs
- **Signaux UNIX** : arrêt propre via SIGINT (CTRL+C)

## Auteure

**Wandji Nguele Estelle**  
Étudiante en Génie des Systèmes Informatiques — ESTLC, Université d'Ébolowa  
[LinkedIn](https://www.linkedin.com/in/estelle-wandji-1086ba2b8)
