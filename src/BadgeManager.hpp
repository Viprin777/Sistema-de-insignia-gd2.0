#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <map>
#include <string>
#include <vector>

struct BadgeInfo {
	std::string id;
	std::string name;
	std::string description;
	std::string sprite; // archivo dentro de resources/, p. ej. "badge-owner.png"
};

class BadgeManager {
public:
	static BadgeManager* get();

	// Carga: archivo incluido -> caché guardada -> descarga remota
	void init();

	// Insignias de un jugador, en el orden en que aparecen en badges.json
	std::vector<BadgeInfo> getBadgesFor(int accountID) const;

private:
	void loadBundled();
	void loadCache();
	void fetchRemote();
	void parse(matjson::Value const& json);

	std::map<std::string, BadgeInfo> m_definitions;
	std::map<int, std::vector<std::string>> m_users;
	geode::EventListener<geode::utils::web::WebTask> m_listener;
};
