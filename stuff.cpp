#pragma warning(disable:4996) // god damn it vc++
#include "stuff.h"
#include <chrono>
#include <thread>
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/writer.h>

// TODO: fix this mess

int64_t getCurrentTime() {
	int64_t currentTime = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	return currentTime;
}

void bailOut() {
	std::this_thread::sleep_for(std::chrono::seconds(20));
	printf("\ni'm outta here!");
	Discord_Shutdown();
	terminate();
}

void DiscordShit::InitDiscord(std::string app_id) {
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = handleDiscordReady;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.errored = handleDiscordError;
	Discord_Initialize(app_id.c_str(), &handlers, 1, NULL);
}

void DiscordShit::UpdatePresence(Config conf) {
	char buffer[256];
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = conf.state.c_str();
	discordPresence.largeImageKey = conf.largeImage.c_str();
	discordPresence.smallImageKey = conf.smallImage.c_str();

	if (conf.elapsedTimeEnabled || conf.remainingTime) {
		int64_t currentTime = getCurrentTime(); 
		
		discordPresence.startTimestamp = currentTime;
		if (conf.remainingTime) discordPresence.endTimestamp = currentTime + conf.remainingTime;
	}

	sprintf(buffer, "%s", conf.details.c_str());
	discordPresence.details = buffer;
	discordPresence.instance = 1;
	Discord_UpdatePresence(&discordPresence);
}

void recreateConfig(const char* fileName) {
	rapidjson::Document docTemp;
	printf("failed to open config file, making a new one...\n");
	FILE* file = fopen(fileName, "wb");

	docTemp.Parse("{\"app_id\":\"407579153060331521\",\"details\":\"test\",\"state\":\"test\",\"largeImage\":\"cmus\",\"smallImage\":\"paused\",\"elapsedTimeEnabled\":true,\"remainingTime\":30}");

	char writeBuffer[65536];
	rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(writeStream);
	docTemp.Accept(writer);

	fclose(file);
	printf("new config generated, reboot when configured\nsleeping for 20 seconds...");

	bailOut();
}

Config readConfig(const char* fileName) { // TODO: definitely fix this mess
	FILE* file = fopen(fileName, "rb");
	rapidjson::Document doc;

	if (!file) recreateConfig(fileName);

	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	doc.ParseStream(readStream);
	fclose(file);

	if (!doc.IsObject()) { printf("malformed json! please fix before running again\nsleeping for 20 seconds..."); bailOut(); }

	Config conf;
	conf.app_id = doc["app_id"].GetString();
	conf.details = doc["details"].GetString();
	conf.state = doc["state"].GetString();
	conf.largeImage = doc["largeImage"].GetString();
	conf.smallImage = doc["smallImage"].GetString();
	conf.elapsedTimeEnabled = doc["elapsedTimeEnabled"].GetBool();
	conf.remainingTime = doc["remainingTime"].GetInt64();
	return conf;
}