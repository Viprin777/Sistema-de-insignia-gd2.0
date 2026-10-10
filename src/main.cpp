#include <Geode/Geode.hpp>
#include "BadgeManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	BadgeManager::get()->init();
}
