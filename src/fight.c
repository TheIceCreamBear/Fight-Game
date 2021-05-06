#include <stdio.h>

#include "player.h"
#include "enemy.h"

struct fight_info {
    struct player *player;
    struct enemy *enemies;
    int enemy_count;
    int cur_enemy;
};

/* For readability - pack a struct fight_info easily */
void pack_fight(struct fight_info *, struct player *, struct enemy *, int);

/* Use this to start a fight between a player and a number of enemies */
static void fight(struct fight_info *);

/* - Internals - */
/* Start a fight with a single enemy */
static void _fight();

/* Handle the player's turn */
static void _handle_player_turn(struct fight_info *);

/* Handle the enemy's turn */
static void _handle_enemy_turn(struct fight_info *);

void pack_fight(struct fight_info *finfo, struct player *p, struct enemy *e,
        int enemy_count) {
    finfo->player = p;
    finfo->enemies = e;
    finfo->enemy_count = enemy_count;
    finfo->cur_enemy = 0;
}

void fight(struct fight_info *finfo) {
    for (int i = 0; i < finfo->enemy_count; i++) {
        _fight(finfo);
    }
}

static void _fight(struct fight_info *finfo) {
    while (finfo->player->health > 0 || 
            finfo->enemies[finfo->cur_enemy].health > 0) {
        _handle_player_turn(finfo);
        _handle_enemy_turn(finfo);
    }
}

static void handle_player_turn(struct fight_info *finfo) {
    /* TODO
     *
     * 1. Grab valid input
     * 2. 
     */
}

static void handle_enemy_turn(struct fight_info *finfo) {
    /* TODO
     * AI makes decision
     * perform decision
     */
}
