#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "BadgeManager.hpp"

using namespace geode::prelude;

class $modify(BadgeProfilePage, ProfilePage) {
	void loadPageFromUserInfo(GJUserScore* score) {
		ProfilePage::loadPageFromUserInfo(score);

		if (!score) return;
		if (!Mod::get()->getSettingValue<bool>("show-badges")) return;

		auto menu = m_mainLayer->getChildByID("username-menu");
		if (!menu) return;

		// Quitar insignias nuestras de una carga anterior de la misma página
		std::string prefix = Mod::get()->getID() + "/badge-";
		std::vector<CCNode*> old;
		for (auto child : menu->getChildrenExt()) {
			if (std::string(child->getID()).rfind(prefix, 0) == 0) {
				old.push_back(child);
			}
		}
		for (auto node : old) node->removeFromParent();

		auto badges = BadgeManager::get()->getBadgesFor(score->m_accountID);
		if (badges.empty()) return;

		for (auto const& badge : badges) {
			auto spr = CCSprite::create(Mod::get()->expandSpriteName(badge.sprite.c_str()).c_str());
			if (!spr) continue;
			limitNodeSize(spr, {22.f, 22.f}, 1.f, .1f);

			auto name = badge.name;
			auto desc = badge.description;
			auto btn = CCMenuItemExt::createSpriteExtra(spr, [name, desc](CCMenuItemSpriteExtra*) {
				FLAlertLayer::create(name.c_str(), desc, "OK")->show();
			});
			btn->setID(prefix + badge.id);
			menu->addChild(btn);
		}

		menu->updateLayout();
	}
};
