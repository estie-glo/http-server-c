# ─── Configuration ────────────────────────────────────────────────────────────
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic -std=c11 -Iinclude
TARGET  = http_server
SRC_DIR = src
OBJ_DIR = obj

# ─── Sources & objets ─────────────────────────────────────────────────────────
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/server.c \
       $(SRC_DIR)/router.c

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# ─── Règles ───────────────────────────────────────────────────────────────────

## Compilation complète
all: $(OBJ_DIR) $(TARGET)

## Créer le dossier obj si inexistant
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

## Lier tous les objets pour produire l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo ""
	@echo "✅ Compilation réussie → ./$(TARGET)"

## Compiler chaque fichier .c en fichier .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

## Lancer le serveur
run: all
	./$(TARGET)

## Supprimer les fichiers compilés
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "🧹 Nettoyage effectué"

## Recompiler depuis zéro
re: clean all

.PHONY: all run clean re
