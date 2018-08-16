#include <string>
#include <algorithm>
#include "stuff.h"

int main()
{
	while (1) {
		Config conf = readConfig("config.json");

		DiscordShit discord;
		discord.InitDiscord(conf.app_id);
		discord.UpdatePresence(conf);

		printf("done...\n");

		while (1) {
			std::string command;
			std::cin >> command;
			std::transform(command.begin(), command.end(), command.begin(), ::tolower);

			if (command == "reload") break;
			else if (command == "help") printf("reload: reloads program\nquit/stop: stops program\n");
			else if (command == "quit" || command == "stop") { return 0; }
			else printf("unknown command\n");
		}

		// clean-up
		Discord_ClearPresence();
		continue;
	}
}

