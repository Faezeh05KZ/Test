#include "GameManager.hpp"
#include "GameUtils.hpp"
#include "Constants.hpp"
#include "Dracula.hpp"
#include "Invisibleman.hpp"
#include "Archaeologist.hpp"
#include "Mayor.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Villager.hpp"
#include "GameMap.hpp"
#include "PerkCard.hpp"
#include "MonsterCard.hpp"

#include <iostream>
#include <algorithm>
#include <random>
#include <limits>
#include <stdexcept>

using namespace std;

GameManager::GameManager() 
    : terrorLevel(0), turnCount(0), gameOver(false), 
      skipNextMonsterPhase(false), currentPlayerIndex(0) {}

void GameManager::initializeGame() {
    gameMap = make_shared<GameMap>();
    initializeHeroes(); 
    initializeMonsters();
    initializeVillagers(); 
    initializeItems(); 
    initializeCards(); 
    cout << "Game components initialized." << endl;
}

void GameManager::initializeHeroes() {
    heroes.push_back(make_shared<Archaeologist>("Archaeologist", "Docks"));
    heroes.push_back(make_shared<Mayor>("Mayor", "Theatre"));
}

void GameManager::initializeMonsters() {
    monsters.push_back(make_shared<Dracula>("Graveyard"));
    monsters.push_back(make_shared<InvisibleMan>("Laboratory"));
    frenziedMonster = "Dracula";
}

void GameManager::initializeVillagers() {
    villagers.push_back(make_shared<Villager>("Dr. Cranly", "Precinct"));
    villagers.push_back(make_shared<Villager>("Dr. Reed", "Camp"));
    villagers.push_back(make_shared<Villager>("Prof. Pearson", "Museum"));
    villagers.push_back(make_shared<Villager>("Maleva", "Shop"));
    villagers.push_back(make_shared<Villager>("Fritz", "Institute"));
    villagers.push_back(make_shared<Villager>("Wilbur & Chick", "Dungeon"));
    villagers.push_back(make_shared<Villager>("Maria", "Camp"));
}

void GameManager::initializeItems() {
    itemPool.push_back(make_shared<Item>("Dart", ItemColor::RED, 2, "Inn"));
    itemPool.push_back(make_shared<Item>("Fire Poker", ItemColor::RED, 3, "Mansion"));
    itemPool.push_back(make_shared<Item>("Rapier", ItemColor::RED, 5, "Theatre"));
    itemPool.push_back(make_shared<Item>("Shovel", ItemColor::RED, 2, "Graveyard"));
    itemPool.push_back(make_shared<Item>("Torch", ItemColor::RED, 5, "Barn"));
    itemPool.push_back(make_shared<Item>("Pitchfork", ItemColor::RED, 4, "Barn"));
    itemPool.push_back(make_shared<Item>("Rifle", ItemColor::RED, 6, "Barn"));
    itemPool.push_back(make_shared<Item>("Silver Cane", ItemColor::RED, 6, "Shop"));
    itemPool.push_back(make_shared<Item>("Knife", ItemColor::RED, 3, "Docks"));
    itemPool.push_back(make_shared<Item>("Pistol", ItemColor::RED, 6, "Precinct"));
    itemPool.push_back(make_shared<Item>("Bear Trap", ItemColor::RED, 4, "Shop"));
    itemPool.push_back(make_shared<Item>("Speargun", ItemColor::RED, 4, "Institute"));
    itemPool.push_back(make_shared<Item>("Anatomy Text", ItemColor::BLUE, 4, "Institute"));
    itemPool.push_back(make_shared<Item>("Centrifuge", ItemColor::BLUE, 4, "Laboratory"));
    itemPool.push_back(make_shared<Item>("Kite", ItemColor::BLUE, 2, "Tower"));
    itemPool.push_back(make_shared<Item>("Research", ItemColor::BLUE, 2, "Tower"));
    itemPool.push_back(make_shared<Item>("Telescope", ItemColor::BLUE, 2, "Mansion"));
    itemPool.push_back(make_shared<Item>("Searchlight", ItemColor::BLUE, 2, "Precinct"));
    itemPool.push_back(make_shared<Item>("Experiment", ItemColor::BLUE, 3, "Laboratory"));
    itemPool.push_back(make_shared<Item>("Analysis", ItemColor::BLUE, 2, "Institute"));
    itemPool.push_back(make_shared<Item>("Rotenone", ItemColor::BLUE, 3, "Institute"));
    itemPool.push_back(make_shared<Item>("Cosmic Ray Diffuser", ItemColor::BLUE, 3, "Tower"));
    itemPool.push_back(make_shared<Item>("Nebularium", ItemColor::BLUE, 3, "Tower"));
    itemPool.push_back(make_shared<Item>("Monocane Mixture", ItemColor::BLUE, 3, "Inn"));
    itemPool.push_back(make_shared<Item>("Fossil", ItemColor::BLUE, 6, "Camp"));
    itemPool.push_back(make_shared<Item>("Flower", ItemColor::YELLOW, 2, "Docks"));
    itemPool.push_back(make_shared<Item>("Tarot Deck", ItemColor::YELLOW, 3, "Camp"));
    itemPool.push_back(make_shared<Item>("Garlic", ItemColor::YELLOW, 2, "Inn"));
    itemPool.push_back(make_shared<Item>("Mirrored Box", ItemColor::YELLOW, 3, "Mansion"));
    itemPool.push_back(make_shared<Item>("Stake", ItemColor::YELLOW, 3, "Abbey"));
    itemPool.push_back(make_shared<Item>("Scroll of Thoth", ItemColor::YELLOW, 4, "Museum"));
    itemPool.push_back(make_shared<Item>("Violin", ItemColor::YELLOW, 3, "Camp"));
    itemPool.push_back(make_shared<Item>("Tablet", ItemColor::YELLOW, 3, "Museum"));
    itemPool.push_back(make_shared<Item>("Wolfsbane", ItemColor::YELLOW, 4, "Camp"));
    itemPool.push_back(make_shared<Item>("Charm", ItemColor::YELLOW, 4, "Camp"));

    shuffle(itemPool.begin(), itemPool.end(), default_random_engine(random_device()()));
    drawAndPlaceItems(Constants::INITIAL_ITEMS_ON_BOARD);
}

void GameManager::initializeCards() {
    for(int i = 0; i < 3; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Visit from the Detective", PerkCard::PerkEffectType::VISIT_FROM_THE_DETECTIVE, "Place Invisible Man in any location"));
    for(int i = 0; i < 3; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Break of Dawn", PerkCard::PerkEffectType::BREAK_OF_DAWN, "Skip the next monster phase. Draw 2 items."));
    for(int i = 0; i < 4; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Overstock", PerkCard::PerkEffectType::OVERSTOCK, "Each player draws 1 item."));
    for(int i = 0; i < 4; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Late into the Night", PerkCard::PerkEffectType::LATE_INTO_THE_NIGHT, "Current player gets 2 extra actions."));
    for(int i = 0; i < 3; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Repel", PerkCard::PerkEffectType::REPEL, "Move each monster up to 2 spaces."));
    for(int i = 0; i < 3; ++i) perkCardDeck.push_back(make_shared<PerkCard>("Hurry", PerkCard::PerkEffectType::HURRY, "Move each hero up to 2 spaces."));
    shuffle(perkCardDeck.begin(), perkCardDeck.end(), default_random_engine(random_device()()));

    for(int i = 0; i < 3; ++i) monsterCardDeck.push_back(make_shared<MonsterCard>("Form of The Bat", 2, vector<MonsterStrike>{{"Invisibleman", 1, 2}} ));
    for(int i = 0; i < 3; ++i) monsterCardDeck.push_back(make_shared<MonsterCard>("Sunrise", 0, vector<MonsterStrike>{{"Invisibleman", 1, 2}, {"Frenzied", 0, 0}} ));
    for(int i = 0; i < 5; ++i) monsterCardDeck.push_back(make_shared<MonsterCard>("Thief", 2, vector<MonsterStrike>{{"Invisibleman", 1, 3}, {"Dracula", 0, 0}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("The Delivery", 3, vector<MonsterStrike>{{"All", 1, 3}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("Fortune Teller", 3, vector<MonsterStrike>{{"All", 1, 2}, {"Frenzied", 0, 0}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("Former Employer", 3, vector<MonsterStrike>{{"Invisibleman", 1, 2}, {"Frenzied", 0, 0}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("Hurried Assistant", 3, vector<MonsterStrike>{{"Dracula", 2, 3}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("The Innocent", 3, vector<MonsterStrike>{{"Frenzied", 1, 3}, {"Dracula", 0, 0}, {"Invisibleman", 0, 0}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("Egyptian Expert", 3, vector<MonsterStrike>{{"Dracula", 2, 2}, {"Frenzied", 0, 0}} ));
    monsterCardDeck.push_back(make_shared<MonsterCard>("The Ichthyologist", 3, vector<MonsterStrike>{{"Frenzied", 1, 2}} ));
    for(int i = 0; i < 2; ++i) monsterCardDeck.push_back(make_shared<MonsterCard>("Hypnotic Gaze", 2, vector<MonsterStrike>{{"Invisibleman", 1, 2}} ));
    for(int i = 0; i < 2; ++i) monsterCardDeck.push_back(make_shared<MonsterCard>("On The Move", 3, vector<MonsterStrike>{{"Frenzied", 3, 2}} ));
    shuffle(monsterCardDeck.begin(), monsterCardDeck.end(), default_random_engine(random_device()()));
}

void GameManager::distributeInitialPerkCards() {
    for (const auto& player : players) {
        drawPerkCard(player->getHero()->getName());
    }
}


void GameManager::startGame(const vector<string>& playerNames) {
    if (playerNames.size() != 2) throw invalid_argument("Exactly 2 players required.");
    players.clear();
    cout << playerNames[0] << ", how many hours since you last ate garlic? ";
    int time1; cin >> time1;
    cout << playerNames[1] << ", how many hours since you last ate garlic? ";
    int time2; cin >> time2;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    int firstPlayerIdx = (time1 <= time2) ? 0 : 1;
    cout << "\n" << playerNames[firstPlayerIdx] << " will start the game!\n";

    cout << "Available Heroes:\n1. Archaeologist\n2. Mayor\n";
    cout << playerNames[firstPlayerIdx] << ", choose your hero (1 or 2): ";
    int choice; cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto p1 = make_shared<Player>(playerNames[firstPlayerIdx]);
    auto p2 = make_shared<Player>(playerNames[(firstPlayerIdx + 1) % 2]);
    
    if (choice == 1) {
        p1->assignHero(getHeroByName("Archaeologist"));
        p2->assignHero(getHeroByName("Mayor"));
    } else {
        p1->assignHero(getHeroByName("Mayor"));
        p2->assignHero(getHeroByName("Archaeologist"));
    }
    
    players.push_back(p1);
    players.push_back(p2);
    currentPlayerIndex = 0;

    distributeInitialPerkCards();
    
    gameOver = false;
    turnCount = 1;
    getCurrentHero()->resetActions();
}

void GameManager::startNextTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    turnCount++;
    cout << "\n--- Turn " << turnCount << ": " << getCurrentPlayerName() << "'s Phase ---" << endl;
    getCurrentHero()->resetActions();
}

void GameManager::monsterPhase() {
    if (gameOver) return;
    cout << "\n--- MONSTER PHASE ---" << endl;
    
    if (skipNextMonsterPhase) {
        cout << "Monster phase is skipped due to Break of Dawn perk!" << endl;
        skipNextMonsterPhase = false;
        startNextTurn();
        return;
    }

    if (monsterCardDeck.empty()) {
        cout << "The monster deck is empty! The villagers lose hope..." << endl;
        gameOver = true;
        return;
    }
    
    auto card = monsterCardDeck.front();
    monsterCardDeck.erase(monsterCardDeck.begin());
    
    cout << "Monster card drawn: " << card->getCardName() << endl;
    card->applyEffect(*this);
    
    if (checkWinConditions()) gameOver = true;
    else if (checkLoseConditions()) gameOver = true;
    else startNextTurn();
}


bool GameManager::moveHero(const string& heroName, const string& destination) {
    auto hero = getHeroByName(heroName);
    if (!hero) return false;
    if (gameMap->areConnected(hero->getCurrentLocation(), destination)) {
        hero->setCurrentLocation(destination);
        cout << hero->getName() << " moved to " << destination << "." << endl;
        return true;
    }
    cout << "Invalid move: " << hero->getCurrentLocation() << " and " << destination << " are not connected." << endl;
    return false;
}

bool GameManager::guideVillager(const string& heroName, const string& villagerName, const string& destination) {
    auto hero = getHeroByName(heroName);
    auto villager = getVillagerByName(villagerName);
    if (!hero || !villager || !villager->isInPlay()) return false;

    string vLoc = villager->getCurrentLocation();
    string hLoc = hero->getCurrentLocation();

    if ((vLoc == hLoc && gameMap->areConnected(hLoc, destination)) || (destination == hLoc && gameMap->areConnected(vLoc, hLoc))) {
        villager->moveTo(destination);
        cout << villagerName << " guided to " << destination << "." << endl;
        if (destination == villager->getSafeLocation()) rescueVillager(villagerName, heroName);
        return true;
    }
    cout << "Invalid guide action." << endl;
    return false;
}

bool GameManager::pickUpItem(const string& heroName, const string& itemName) {
    auto hero = getHeroByName(heroName);
    if (!hero) return false;
    auto item = gameMap->removeItem(hero->getCurrentLocation(), itemName);
    if (item) {
        hero->addItem(item);
        cout << heroName << " picked up " << itemName << "." << endl;
        return true;
    }
    cout << "Item '" << itemName << "' not found at this location." << endl;
    return false;
}

bool GameManager::useSpecialAbility(const string& heroName, const string& optionalTarget) {
    auto hero = getHeroByName(heroName);
    if (!hero || !hero->hasSpecialAction()) {
        cout << "This hero has no special action." << endl;
        return false;
    }
    if (auto archaeologist = dynamic_pointer_cast<Archaeologist>(hero)) {
        if (optionalTarget.empty()) {
            cout << "Archaeologist needs to target an item in an adjacent location." << endl;
            return false;
        }
        auto neighbors = gameMap->getConnectedLocations(hero->getCurrentLocation());
        for (const auto& loc : neighbors) {
             auto item = gameMap->removeItem(loc, optionalTarget);
             if(item) {
                 hero->addItem(item);
                 cout << "Archaeologist used special action to pick up " << optionalTarget << " from " << loc << endl;
                 return true;
             }
        }
        cout << "Item '" << optionalTarget << "' not found in adjacent locations." << endl;
    }
    return false;
}

bool GameManager::usePerkCard(const string& heroName, const string& cardName) {
    auto hero = getHeroByName(heroName);
    if (!hero) return false;
    auto it = find_if(hero->getPerkCards().begin(), hero->getPerkCards().end(), 
        [&](const shared_ptr<PerkCard>& card){ return GameUtils::toLower(card->getCardName()) == GameUtils::toLower(cardName); });
    if (it != hero->getPerkCards().end()) {
        auto card = hero->removePerkCard((*it)->getCardName());
        if (card) {
            card->applyEffect(*this);
            return true;
        }
    }
    cout << "Perk card '" << cardName << "' not found in your hand." << endl;
    return false;
}


bool GameManager::advanceTask(const string& heroName, const string& monsterName) {
    auto hero = getHeroByName(heroName);
    auto monster = getMonsterByName(monsterName);
    if (!hero || !monster) return false;
    if (auto dracula = dynamic_pointer_cast<Dracula>(monster)) {
        const auto& coffinLocs = dracula->getCoffinLocations();
        if (find(coffinLocs.begin(), coffinLocs.end(), hero->getCurrentLocation()) != coffinLocs.end()) {
            if (hero->getTotalItemStrengthByColor(ItemColor::RED) >= Constants::DRACULA_COFFIN_TASK_STRENGTH) {
                hero->removeItemsByStrength(ItemColor::RED, Constants::DRACULA_COFFIN_TASK_STRENGTH);
                if(dracula->destroyCoffin(hero->getCurrentLocation()))
                    cout << "A coffin at " << hero->getCurrentLocation() << " has been smashed!" << endl;
                return true;
            }
        }
    } else if (auto invisibleMan = dynamic_pointer_cast<InvisibleMan>(monster)) {
        if (hero->getCurrentLocation() == "Precinct") {
            for(const auto& item : hero->getItems()) {
                if(invisibleMan->getEvidenceLocations().count(item->getOriginalLocation())) {
                    if(invisibleMan->addEvidence(item)) {
                        cout << "Evidence from " << item->getOriginalLocation() << " supplied!" << endl;
                        hero->removeItem(item->getName());
                        return true;
                    }
                }
            }
        }
    }
    cout << "Conditions for advancing task are not met." << endl;
    return false;
}

bool GameManager::defeatMonster(const string& heroName, const string& monsterName) {
    auto hero = getHeroByName(heroName);
    auto monster = getMonsterByName(monsterName);
    if (!hero || !monster || hero->getCurrentLocation() != monster->getCurrentLocation()) {
        cout << "Hero and monster must be in the same location." << endl;
        return false;
    }
    if (!monster->canBeDefeated(*this)) {
        cout << "Prerequisites to defeat " << monsterName << " are not met. (Task not complete)" << endl;
        return false;
    }
    if (auto dracula = dynamic_pointer_cast<Dracula>(monster)) {
        if (hero->getTotalItemStrengthByColor(ItemColor::YELLOW) >= Constants::DRACULA_DEFEAT_STRENGTH) {
            hero->removeItemsByStrength(ItemColor::YELLOW, Constants::DRACULA_DEFEAT_STRENGTH);
            monster->setDefeated(true);
            cout << heroName << " has defeated Dracula!" << endl;
            updateFrenzyOrder();
            return true;
        }
    } else if (auto invisibleMan = dynamic_pointer_cast<InvisibleMan>(monster)) {
        if (hero->getTotalItemStrengthByColor(ItemColor::RED) >= Constants::INVISIBLE_MAN_DEFEAT_STRENGTH) {
            hero->removeItemsByStrength(ItemColor::RED, Constants::INVISIBLE_MAN_DEFEAT_STRENGTH);
            monster->setDefeated(true);
            cout << heroName << " has defeated the Invisible Man!" << endl;
            updateFrenzyOrder();
            return true;
        }
    }
    cout << "Not enough required item strength to defeat " << monsterName << "." << endl;
    return false;
}

//فاز مانستر 

void GameManager::applyMonsterEvent(const string& cardName) {
    if (cardName == "Form of The Bat") moveMonsterToHero("Dracula");
    else if (cardName == "Sunrise") placeMonster("Dracula", "Crypt");
    else if (cardName == "Thief") thiefEvent();
    else if (cardName == "The Delivery") placeVillagerOnBoard("Wilbur & Chick", "Docks");
    else if (cardName == "Fortune Teller") placeVillagerOnBoard("Maleva", "Camp");
    else if (cardName == "Former Employer") placeVillagerOnBoard("Dr. Cranly", "Laboratory");
    else if (cardName == "Hurried Assistant") placeVillagerOnBoard("Fritz", "Tower");
    else if (cardName == "The Innocent") placeVillagerOnBoard("Maria", "Barn");
    else if (cardName == "Egyptian Expert") placeVillagerOnBoard("Prof. Pearson", "Cave");
    else if (cardName == "The Ichthyologist") placeVillagerOnBoard("Dr. Reed", "Institute");
    else if (cardName == "Hypnotic Gaze") hypnoticGazeEvent();
    else if (cardName == "On The Move") onTheMoveEvent();
}

void GameManager::resolveStrikes(const vector<MonsterStrike>& strikes) {
    bool attackMadeThisTurn = false;
    for (const auto& strike : strikes) {
        if (attackMadeThisTurn) break;
        shared_ptr<Monster> monsterToAct = nullptr;
        if (strike.targetMonster == "Frenzied") monsterToAct = getMonsterByName(frenziedMonster);
        else if (strike.targetMonster == "All") {
            for (auto& m : monsters) {
                if (!m->isDefeated() && !attackMadeThisTurn) {
                    moveMonsterTowardsTarget(m, strike.moves);
                    if(!m->getTargetsAtLocation(*this).empty()) { handleAttack(m, strike.dice); attackMadeThisTurn = true; }
                }
            }
            continue;
        } else {
            monsterToAct = getMonsterByName(strike.targetMonster);
        }
        if (monsterToAct && !monsterToAct->isDefeated()) {
            moveMonsterTowardsTarget(monsterToAct, strike.moves);
            if (!monsterToAct->getTargetsAtLocation(*this).empty()) { handleAttack(monsterToAct, strike.dice); attackMadeThisTurn = true; }
        }
    }
}

void GameManager::moveMonsterTowardsTarget(const shared_ptr<Monster>& monster, int steps) {
    if (steps <= 0) return;
    string startLoc = monster->getCurrentLocation();
    TargetInfo target = findNearestTarget(startLoc);
    if (target.distance == 0) {
        cout << monster->getName() << " is already with a target and does not move." << endl;
        return;
    }
    if (target.distance != numeric_limits<int>::max()) {
        string current = startLoc;
        for (int i=0; i < steps; ++i) {
             string nextStep = gameMap->findShortestPath(current, target.location);
             if (nextStep == current) break;
             current = nextStep;
             if (current == target.location) break;
        }
        monster->setCurrentLocation(current);
        cout << monster->getName() << " moves from " << startLoc << " to " << current << "." << endl;
    }
}

void GameManager::handleAttack(const shared_ptr<Monster>& monster, int diceCount) {
    if (diceCount == 0) return;
    auto targets = monster->getTargetsAtLocation(*this);
    if (targets.empty()) return;
    cout << monster->getName() << " attacks at " << monster->getCurrentLocation() << "!" << endl;

    bool powerActivated = false, attackHit = false;
    for (int i = 0; i < diceCount; ++i) {
        int roll = GameUtils::rollDice(1);
        if (roll <= 2) attackHit = true; else if (roll == 6) powerActivated = true;
    }
    if (powerActivated) {
        cout << monster->getName() << " activates its power!" << endl;
        monster->usePower(*this);
        targets = monster->getTargetsAtLocation(*this);
        if (targets.empty()) return;
    }
    if (attackHit) {
        cout << monster->getName() << " strikes!" << endl;
        sort(targets.begin(), targets.end(), [](const auto& a, const auto& b) { return a.type < b.type; });
        const auto& firstTarget = targets.front();
        if (firstTarget.type == TargetType::HERO) {
            auto hero = getHeroByName(firstTarget.name);
            if (!defendAttack(hero)) {
                cout << hero->getName() << " is defeated!" << endl;
                hero->takeDamage(); hero->setCurrentLocation("Hospital"); increaseTerrorLevel();
            }
        } else {
            auto villager = getVillagerByName(firstTarget.name);
            cout << villager->getName() << " is killed by " << monster->getName() << "!" << endl;
            villager->setDead(true); increaseTerrorLevel();
        }
    } else {
        cout << monster->getName() << "'s attack missed." << endl;
    }
}

bool GameManager::defendAttack(const shared_ptr<Hero>& hero) {
    if (hero->getItems().empty()) return false;
    cout << hero->getName() << ", you are being attacked! Spend an item to defend? (y/n): ";
    char choice; cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (tolower(choice) == 'y') {
        while (true) {
            cout << "Which item to discard? (Type name): ";
            string itemName; getline(cin, itemName);
            auto item = hero->removeItem(itemName);
            if(item){
                cout << hero->getName() << " defended by discarding " << item->getName() << "!" << endl;
                return true;
            } else {
                cout << "Item not found! Try again or type 'cancel' to skip defense." << endl;
                if (itemName == "cancel") break;
            }
        }
        return false;
    }
    return false;
}

void GameManager::increaseTerrorLevel(int amount) {
    terrorLevel += amount;
    cout << "Terror Level increased to " << terrorLevel << "!" << endl;
}

void GameManager::drawAndPlaceItems(int count) {
    for (int i = 0; i < count; ++i) {
        if (itemPool.empty()) { cout << "Item pool is empty!" << endl; return; }
        int index = GameUtils::randomInt(0, itemPool.size() - 1);
        auto item = itemPool[index];
        gameMap->placeItem(item, item->getOriginalLocation());
        cout << "Item '" << item->getName() << "' placed at " << item->getOriginalLocation() << "." << endl;
        itemPool.erase(itemPool.begin() + index);
    }
}

void GameManager::overstockItems() {
    for(const auto& player : players) {
        if (itemPool.empty()) break;
        int index = GameUtils::randomInt(0, itemPool.size() - 1);
        auto item = itemPool[index];
        gameMap->placeItem(item, player->getHero()->getCurrentLocation());
        cout << "Item '" << item->getName() << "' placed at " << player->getHero()->getCurrentLocation() << " for " << player->getName() << "." << endl;
    }
}

void GameManager::placeVillagerOnBoard(const string& villagerName, const string& location) {
    auto villager = getVillagerByName(villagerName);
    if (villager && !villager->isInPlay()) {
        villager->enterPlay(location);
        gameMap->placeVillager(villager, location);
        cout << villagerName << " has appeared at " << location << "!" << endl;
    }
}

void GameManager::thiefEvent() {
    string locWithMostItems = ""; size_t maxItems = 0;
    for (const auto& loc : gameMap->getAllLocations()) {
        size_t itemCount = gameMap->getItemsAt(loc).size();
        if (itemCount > maxItems) { maxItems = itemCount; locWithMostItems = loc; }
    }
    if (!locWithMostItems.empty()) {
        cout << "Thief event! Invisible Man moves to " << locWithMostItems << " and discards all items." << endl;
        placeMonster("Invisible Man", locWithMostItems);
        auto items = gameMap->getItemsAt(locWithMostItems);
        for(const auto& item : items) gameMap->removeItem(locWithMostItems, item->getName());
    }
}

void GameManager::hypnoticGazeEvent() {
    auto draculaLoc = getMonsterByName("Dracula")->getCurrentLocation();
    auto target = findNearestTarget(draculaLoc);
    if (target.distance != numeric_limits<int>::max()) {
        cout << "Hypnotic Gaze! " << target.name << " is moved 1 space towards Dracula." << endl;
        if(target.type == TargetType::HERO) {
            auto hero = getHeroByName(target.name);
            hero->setCurrentLocation(gameMap->findShortestPath(hero->getCurrentLocation(), draculaLoc));
        } else {
             auto villager = getVillagerByName(target.name);
             villager->moveTo(gameMap->findShortestPath(villager->getCurrentLocation(), draculaLoc));
        }
    }
}

void GameManager::onTheMoveEvent() {
    cout << "On The Move! The Frenzy marker passes to the next monster." << endl;
    updateFrenzyOrder(true);
    cout << "All villagers move 1 space towards their safe location." << endl;
    for(const auto& v : villagers) {
        if(v->isInPlay()) {
            string nextStep = gameMap->findShortestPath(v->getCurrentLocation(), v->getSafeLocation());
            if(nextStep != v->getCurrentLocation()){
                v->moveTo(nextStep);
                if (nextStep == v->getSafeLocation()) {
                    v->setRescued(true); cout << v->getName() << " reached their safe place!" << endl;
                }
            }
        }
    }
}

void GameManager::updateFrenzyOrder(bool forceNext) {
    if (monsters.size() < 2) return;
    string nextFrenziedName = frenziedMonster;
    if (forceNext) {
        nextFrenziedName = (frenziedMonster == "Dracula") ? "Invisible Man" : "Dracula";
    }
    auto nextMonster = getMonsterByName(nextFrenziedName);
    if (nextMonster && !nextMonster->isDefeated()) frenziedMonster = nextFrenziedName;
    else {
        string otherName = (nextFrenziedName == "Dracula") ? "Invisible Man" : "Dracula";
        auto otherMonster = getMonsterByName(otherName);
        if (otherMonster && !otherMonster->isDefeated()) frenziedMonster = otherName;
        else frenziedMonster = "";
    }
    if (!frenziedMonster.empty()) cout << "Frenzy marker is now on " << frenziedMonster << "." << endl;
}

std::string GameManager::getCurrentPlayerName() const { return players.at(currentPlayerIndex)->getName(); }
shared_ptr<Hero> GameManager::getCurrentHero() const { if (players.empty()) return nullptr; return players.at(currentPlayerIndex)->getHero(); }
shared_ptr<Hero> GameManager::getHeroByName(const string& name) const { for (const auto& h : heroes) if (h->getName() == name) return h; return nullptr; }
shared_ptr<Monster> GameManager::getMonsterByName(const string& name) const { for (const auto& m : monsters) if (m->getName() == name) return m; return nullptr; }
shared_ptr<Villager> GameManager::getVillagerByName(const string& name) const { for (const auto& v : villagers) if (v->getName() == name) return v; return nullptr; }

void GameManager::rescueVillager(const string& villagerName, const string& heroName) {
    auto villager = getVillagerByName(villagerName);
    if(villager && villager->isInPlay()) {
        villager->setRescued(true);
        cout << villager->getName() << " has been rescued by " << heroName << "!" << endl;
        drawPerkCard(heroName);
    }
}

void GameManager::drawPerkCard(const string& heroName) {
    if (perkCardDeck.empty()) { cout << "Perk card deck is empty!" << endl; return; }
    auto hero = getHeroByName(heroName);
    if (hero) {
        auto card = perkCardDeck.front();
        perkCardDeck.erase(perkCardDeck.begin());
        hero->addPerkCard(card);
        cout << heroName << " received a new perk card: '" << card->getCardName() << "'" << endl;
    }
}

TargetInfo GameManager::findNearestTarget(const string& fromLocation) const {
    TargetInfo nearestTarget = {"", TargetType::HERO, "", numeric_limits<int>::max()};
    for(const auto& hero : heroes) if(!hero->isDamaged()) {
        int dist = gameMap->calculateDistance(fromLocation, hero->getCurrentLocation());
        if (dist < nearestTarget.distance) nearestTarget = {hero->getName(), TargetType::HERO, hero->getCurrentLocation(), dist};
    }
    for(const auto& villager : villagers) if(villager->isInPlay()) {
        int dist = gameMap->calculateDistance(fromLocation, villager->getCurrentLocation());
        if (dist < nearestTarget.distance) nearestTarget = {villager->getName(), TargetType::VILLAGER, villager->getCurrentLocation(), dist};
    }
    return nearestTarget;
}

bool GameManager::checkWinConditions() const {
    return all_of(monsters.begin(), monsters.end(), [](const auto& m){ return m->isDefeated(); });
}

bool GameManager::checkLoseConditions() {
    if (terrorLevel >= Constants::MAX_TERROR_LEVEL) { cout << "The terror level is maxed out!" << endl; return true; }
    if (monsterCardDeck.empty()) { cout << "The monster deck is empty!" << endl; return true; }
    return false;
}

void GameManager::placeMonster(const std::string& monsterName, const std::string& location) {
    auto monster = getMonsterByName(monsterName);
    if(monster) { monster->setCurrentLocation(location); cout << monsterName << " was moved to " << location << "." << endl; }
}

void GameManager::moveMonsterToHero(const std::string& monsterName) {
    auto monster = getMonsterByName(monsterName);
    auto hero = getCurrentHero();
    if(monster && hero) { monster->setCurrentLocation(hero->getCurrentLocation()); cout << monsterName << " moved to " << hero->getName() << "'s location." << endl; }
}

void GameManager::promptAndPlaceMonster(const std::string& monsterName) {
    cout << "Where do you want to place " << monsterName << "? > "; string loc; getline(cin, loc); placeMonster(monsterName, loc);
}
void GameManager::promptAndMoveMonster(const std::string& monsterName, int steps) { cout << "TUI should handle monster movement for " << monsterName << "." << endl; }
void GameManager::promptAndMoveHero(const std::string& heroName, int steps) { cout << "TUI should handle hero movement for " << heroName << "." << endl; }

vector<shared_ptr<Item>> GameManager::getItemsAtLocation(const string& location) const { return gameMap->getItemsAt(location); }
vector<shared_ptr<Villager>> GameManager::getVillagersAtLocation(const string& location) const { return gameMap->getVillagersAt(location); }

Hero* GameManager::getHeroAtLocation(const std::string& location) const {
    for (const auto& hero : heroes) {
        if (hero->getCurrentLocation() == location && !hero->isDamaged()) {
            return hero.get();
        }
    }
    return nullptr;
}
