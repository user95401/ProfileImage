//geode
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <filesystem>
$on_mod(Loaded) {
	try {
        std::filesystem::copy_file(
			(Mod::get()->getConfigDir()/"settings.json").string(), //from game dir
			(Mod::get()->getSaveDir()/"settings.json").string(), //to game save dir
		std::filesystem::copy_options::overwrite_existing);
		Mod::get()->loadData();
    }
    catch (std::exception& theException) {
		log::error("{}", theException.what());
    }
}

#include <Geode/modify/AppDelegate.hpp>
class $modify(AppDelegate) {
	void trySaveGame(bool p0) {
		AppDelegate::trySaveGame(p0);
		try {
			std::filesystem::copy_file(
				(Mod::get()->getSaveDir()/"settings.json").string(), //from game save dir
				(Mod::get()->getConfigDir()/"settings.json").string(), //to game dir
			std::filesystem::copy_options::overwrite_existing);
		}
		catch (std::exception& theException) {
			log::error("{}", theException.what());
		}
	}
};