#pragma once
#include <iostream>
#include <discord_rpc.h>
#include <vector>

struct Config {
	std::string app_id, details, state, largeImage, smallImage;
};

class DiscordShit {
public:
	void InitDiscord(std::string app_id);
	void UpdatePresence(Config);
private:
	static void handleDiscordReady(const DiscordUser* connectedUser) { printf("\nDiscord: connected\n"); };
	static void handleDiscordDisconnected(int errcode, const char* message) { printf("\nDiscord: disconnected (%d: %s)\n", errcode, message); }
	static void handleDiscordError(int errcode, const char* message) { printf("\nDiscord: error (%d: %s)\n", errcode, message); }
};

Config readConfig(const char* fileName);
void recreateConfig(const char* fileName);

void bailOut();