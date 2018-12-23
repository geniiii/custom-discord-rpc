#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <discord_rpc.h>

// config file name
extern const char *file_name;

// bool in c without including stdbool
typedef enum { false, true } bool;


typedef struct {
	const char *app_id, *state, *details, *large_image, *small_image; /* check rich presence docs */
	bool elapsed_time_enabled;
	int remaining_time; /* max 86400 seconds (5 byte string) */
} Config;


// handles ini parsing
int handler(void* user, const char* section, const char* name, const char* value);


/* discord-rpc stuff */
void init_discord(const char *app_id);
void update_presence(Config conf);
/* discord-rpc stuff */


/* discord-rpc events */
inline static void handle_discord_ready(const DiscordUser *connectedUser) { printf("\nDiscord: connected\n"); };
inline static void handle_discord_disconnected(int errcode, const char *message) { printf("\nDiscord: disconnected (%d: %s)\n", errcode, message); }
inline static void handle_discord_error(int errcode, const char *message) { printf("\nDiscord: error (%d: %s)\n", errcode, message); }
/* discord-rpc events */


/* config stuff */
Config read_config();
void recreate_config();
/* config stuff */

// get current unix time
unsigned long get_current_time();


// (gracefully) terminating the program in the event of a fatal error
void bail_out();
