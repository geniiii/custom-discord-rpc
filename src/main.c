#include <stuff.h>

#define MATCH(command, s) strncmp(command, s, 16) == 0

int main()
{
	while (1) {
		Config conf = read_config();

		// initialize discord
		init_discord(conf.app_id);
		update_presence(conf);

		printf("done...\n");

		// command loop
		while (1) {
			char command[16];
			fgets(command, 17, stdin);

			command[strcspn(command, "\n")] = 0;

			for (size_t i = 0; i < strnlen(command, 16); i++) {
				command[i] = tolower(command[i]);
			}

			if (MATCH(command, "reload")) break;
			else if (MATCH(command, "help")) printf("reload: reloads program\nquit/exit: exits program\n");
			else if (MATCH(command, "quit") || MATCH(command, "exit")) {
				Discord_Shutdown();
				return 0;
            }
			else printf("unknown command\n");
		}

		// clean-up
		Discord_Shutdown();
	}
}