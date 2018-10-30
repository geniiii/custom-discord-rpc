#pragma once
#include <iostream>
#include <discord_rpc.h>

struct Config {
	const char *app_id, *state, *details, *largeImage, *smallImage;
	bool elapsedTimeEnabled;
	int64_t remainingTime;
};

class DiscordShit {
public:
	void InitDiscord(const char *app_id);
	void UpdatePresence(Config);
private:
	// discord-rpc events
	inline static void handleDiscordReady(const DiscordUser *connectedUser) { printf("\nDiscord: connected\n"); };
	inline static void handleDiscordDisconnected(int errcode, const char *message) { printf("\nDiscord: disconnected (%d: %s)\n", errcode, message); }
	inline static void handleDiscordError(int errcode, const char *message) { printf("\nDiscord: error (%d: %s)\n", errcode, message); }
};

Config readConfig(const char *fileName);
void recreateConfig(const char *fileName);

inline int64_t getCurrentTime();

inline void bailOut();