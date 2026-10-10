#include "BadgeManager.hpp"

using namespace geode::prelude;

BadgeManager* BadgeManager::get() {
	static BadgeManager* instance = new BadgeManager();
	return instance;
}

void BadgeManager::init() {
	this->loadBundled();
	this->loadCache();
	this->fetchRemote();
}

void BadgeManager::loadBundled() {
	auto path = Mod::get()->getResourcesDir() / "badges.json";
	auto res = file::readJson(path);
	if (res.isOk()) {
		this->parse(res.unwrap());
	} else {
		log::warn("No se pudo leer badges.json incluido en el mod");
	}
}

void BadgeManager::loadCache() {
	auto cached = Mod::get()->getSavedValue<std::string>("cache");
	if (cached.empty()) return;
	auto parsed = matjson::parse(cached);
	if (parsed.isOk()) {
		this->parse(parsed.unwrap());
	}
}

void BadgeManager::fetchRemote() {
	auto url = Mod::get()->getSettingValue<std::string>("badges-url");
	if (url.empty() || url.find("TU_USUARIO") != std::string::npos) {
		log::info("URL de insignias sin configurar; se usan solo las insignias locales");
		return;
	}

	m_listener.bind([this](web::WebTask::Event* e) {
		auto* res = e->getValue();
		if (!res) return;

		if (!res->ok()) {
			log::warn("No se pudo descargar badges.json (codigo {})", res->code());
			return;
		}

		auto text = res->string();
		if (!text.isOk()) return;

		auto parsed = matjson::parse(text.unwrap());
		if (!parsed.isOk()) {
			log::warn("badges.json remoto no es un JSON valido");
			return;
		}

		this->parse(parsed.unwrap());
		Mod::get()->setSavedValue<std::string>("cache", text.unwrap());
		log::info("Insignias actualizadas desde internet");
	});

	m_listener.setFilter(web::WebRequest().get(url));
}

void BadgeManager::parse(matjson::Value const& json) {
	// Definiciones de insignias
	if (json.contains("badges")) {
		auto arr = json["badges"].asArray();
		if (arr.isOk()) {
			for (auto const& item : arr.unwrap()) {
				BadgeInfo info;
				info.id = item["id"].asString().unwrapOr("");
				if (info.id.empty()) continue;
				info.name = item["name"].asString().unwrapOr(info.id);
				info.description = item["description"].asString().unwrapOr("");
				info.sprite = item["sprite"].asString().unwrapOr("");
				m_definitions[info.id] = info;
			}
		}
	}

	// Quién tiene qué insignia (reemplaza la lista anterior)
	if (json.contains("users")) {
		auto arr = json["users"].asArray();
		if (arr.isOk()) {
			m_users.clear();
			for (auto const& item : arr.unwrap()) {
				int accountID = static_cast<int>(item["accountID"].asInt().unwrapOr(0));
				if (accountID <= 0) continue;

				auto list = item["badges"].asArray();
				if (!list.isOk()) continue;

				for (auto const& b : list.unwrap()) {
					auto id = b.asString().unwrapOr("");
					if (!id.empty()) m_users[accountID].push_back(id);
				}
			}
		}
	}
}

std::vector<BadgeInfo> BadgeManager::getBadgesFor(int accountID) const {
	std::vector<BadgeInfo> out;
	auto it = m_users.find(accountID);
	if (it == m_users.end()) return out;

	for (auto const& id : it->second) {
		auto def = m_definitions.find(id);
		if (def != m_definitions.end()) out.push_back(def->second);
	}
	return out;
}
