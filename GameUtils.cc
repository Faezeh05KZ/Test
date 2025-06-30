#include "GameUtils.hpp"
#include <random>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "PerkCard.hpp"
#include "MonsterCard.hpp"

static std::random_device rd;
static std::mt19937 gen(rd());

int GameUtils::rollDice(int diceCount) {
    std::uniform_int_distribution<> dis(1, 6);
    int total = 0;
    for (int i = 0; i < diceCount; ++i) {
        total += dis(gen);
    }
    return total;
}

int GameUtils::randomInt(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

double GameUtils::randomDouble(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

std::shared_ptr<PerkCard> GameUtils::drawRandomPerkCard(std::vector<std::shared_ptr<Card>>& cards) {
    std::vector<std::shared_ptr<PerkCard>> perkCards; 

    for (auto& card : cards) {
        if (auto perk = std::dynamic_pointer_cast<PerkCard>(card)) {
            perkCards.push_back(perk);
        }
    }
    
    if (perkCards.empty()) {
        return nullptr;
    }
    std::uniform_int_distribution<> dis(0 , perkCards.size() - 1);
    return perkCards[dis(gen)];
}

std::shared_ptr<MonsterCard> GameUtils::drawRandomMonsterCard(std::vector<std::shared_ptr<Card>>& cards) {
    std::vector<std::shared_ptr<MonsterCard>> monsterCards;
    
    for (auto& card : cards) {
        if (auto monster = std::dynamic_pointer_cast<MonsterCard>(card)) {
            monsterCards.push_back(monster);
        }
    }
    
    if (monsterCards.empty()) {
        return nullptr;
    }
    
    std::uniform_int_distribution<> dis(0, monsterCards.size() - 1);
    return monsterCards[dis(gen)];
}

void GameUtils::shuffleCards(std::vector<std::shared_ptr<Card>>& cards) {
    std::shuffle(cards.begin(), cards.end(), gen);
}

std::shared_ptr<Item> GameUtils::drawRandomItem(std::vector<std::shared_ptr<Item>>& items) {
    if (items.empty()) {
        return nullptr;
    }
    
    std::uniform_int_distribution<> dis(0, items.size() - 1);
    return items[dis(gen)];
}

std::vector<std::shared_ptr<Item>> GameUtils::filterItemsByColor(
    const std::vector<std::shared_ptr<Item>>& items, ItemColor color) {
    
    std::vector<std::shared_ptr<Item>> filtered;
    for (const auto& item : items) {
        if (item->getColor() == color) {
            filtered.push_back(item);
        }
    }
    return filtered;
}

int GameUtils::calculateTotalPower(const std::vector<std::shared_ptr<Item>>& items) {
    int total = 0;
    for (const auto& item : items) {
        total += item->getStrength();
    }
    return total;
}

std::string GameUtils::toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

std::string GameUtils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> GameUtils::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}

bool GameUtils::isValidHeroName(const std::string& name) {
    const std::vector<std::string> validNames = {"Archaeologist", "Mayor"};
    
    return std::find(validNames.begin(), validNames.end(), name) != validNames.end();
}

bool GameUtils::isValidItemName(const std::string& name) {
    return !name.empty();
}

bool GameUtils::isValidLocationName(const std::string& name) {
    const std::vector<std::string> validLocations = {
        "Docks", "Theatre", "Graveyard", "Laboratory", "Precinct",
        "Barn", "Institute", "Mansion", "Inn", "Cave", "Dungeon", "Crypt"
    };
    
    return std::find(validLocations.begin(), validLocations.end(), name) != validLocations.end();
}