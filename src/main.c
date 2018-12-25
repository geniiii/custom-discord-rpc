#include "stuff.h"

#define MATCH(command, s) strncmp(command, s, 16) == 0

int main()
{
	while (1) {
		// initialize defaults to prevent crashes
		Config conf = { "000000000000000000", "", "", "", "", "", "", false, 0 };
		conf = read_config(conf);

		// initialize discord
		init_discord(conf.app_id);
		update_presence(conf);

		// jesus fucking christ
		printf(
			"app_id:%s\nstate:%s\ndetails:%s\nlarge_image_key:%s\nlarge_image_text:%s\nsmall_image_key:%s\nsmall_image_text:%s\nelapsed_time_enabled:%i\nremaining_time:%i\ndone...\n",
			conf.app_id,
			conf.state,
			conf.details,
			conf.large_image_key,
			conf.large_image_text,
			conf.small_image_key,
			conf.small_image_text,
			conf.elapsed_time_enabled,
			conf.remaining_time
		);

		// command loop
		while (1) {
			char command[16];
			fgets(command, 17, stdin);

			command[strcspn(command, "\n")] = 0;

			for (size_t i = 0; i < strnlen(command, 16); i++) {
				command[i] = tolower(command[i]);
			}

			if (MATCH(command, "reload")) {
				Discord_Shutdown();
				break;
			}
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