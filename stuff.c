#pragma warning(disable:4996) // god damn it vc++

#include <stuff.h>
#include <ini.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// config file_name
const char *file_name = "config.ini";

/* handles ini parsing

typedef struct {
	const char *app_id, *state, *details, *large_image, *small_image;
	bool elapsed_time_enabled;
	int remaining_time;
} Config; */

int handler(void* user, const char* section, const char* name, const char* value) {
	Config* conf = (Config*)user;

	#define MATCH(s, n, l) strncmp(section, s, 6) == 0 && strncmp(name, n, l) == 0
	#define BOOL(s) strncmp("true", s, 4) == 0 || strncmp("1", s, 1) == 0

	if (MATCH("config", "app_id", 6)) {
		conf->app_id = strndup(value, 18);
	} else if (MATCH("config", "state", 5)) {
		conf->state = strndup(value, 128);
	} else if (MATCH("config", "details", 7)) {
		conf->details = strndup(value, 128);
	} else if (MATCH("config", "large_image", 11)) {
		conf->large_image = strndup(value, 32);
	} else if (MATCH("config", "small_image", 11)) {
		conf->small_image = strndup(value, 32);
	} else if (MATCH("config", "elapsed_time_enabled", 20)) {
		conf->elapsed_time_enabled = BOOL(strdup(value));
	} else if (MATCH("config", "remaining_time", 14)) {
		conf->remaining_time = atoi(value);
	} else {
		char choice[1];

		printf("malformed config! please read the README\nwould you like to recreate the config? (Y/n)\n");
		fgets(choice, 2, stdin);
		choice[0] = tolower(choice[0]);

		switch (choice[0]) {
		case 'n':
			bail_out();
			break;
		default:
			recreate_config();
			bail_out();
			break;
		}
	}
	return 1;
}



/* discord-rpc stuff */
void init_discord(const char *app_id) {
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));

	handlers.ready = handle_discord_ready;
	handlers.disconnected = handle_discord_disconnected;
	handlers.errored = handle_discord_error;

	Discord_Initialize(app_id, &handlers, 0, NULL);
}

void update_presence(Config conf) {
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));

	discordPresence.state = conf.state;
	discordPresence.details = conf.details;
	discordPresence.largeImageKey = conf.large_image;
	discordPresence.smallImageKey = conf.small_image;
	discordPresence.instance = 0;


	if (conf.elapsed_time_enabled) {
		discordPresence.startTimestamp = get_current_time();
		if (conf.remaining_time) {
			discordPresence.endTimestamp = discordPresence.startTimestamp + conf.remaining_time;
		}
	}

	Discord_UpdatePresence(&discordPresence);
}
/* discord-rpc stuff */



/* config stuff */
Config read_config() {
	Config config;
	if (ini_parse(file_name, handler, &config) < 0) {
        printf("failed to load config!\n");
        recreate_config();
    }

	return config;
}

void recreate_config() {
	const char *fallback_conf = "[config]\napp_id=000000000000000000\ndetails=details\nstate=state\nlarge_image=A\nsmall_image=B\nelapsed_time_enabled=true\nremaining_time=60\n";
	printf("recreating...\n");

	FILE *conf = fopen(file_name, "w");
	fprintf(conf, fallback_conf);
	fclose(conf);

	printf("new config generated, reboot when configured\n");

	bail_out();
}
/* config stuff */

// get current unix time
unsigned long get_current_time() {
	return (unsigned long)time(NULL);
}

// (gracefully) terminating the program in the event of a fatal error
void bail_out() {
	printf("sleeping for 20 secs before exiting...\n");
	#ifdef _WIN32
	Sleep(2000);
	#else
	nanosleep((const struct timespec[]){{20, 0L}}, NULL);
	#endif

	printf("i'm outta here!");

	Discord_Shutdown();
	exit(1);
}
