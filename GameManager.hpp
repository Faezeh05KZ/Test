#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>

class Hero;
class Monster;
class GameMap;
class Item;
class Villager;
class Player;
class MonsterCard;
class PerkCard;
struct MonsterStrike;
enum class TargetType;  
struct TargetInfo;

class GameManager {
    public:
    GameManager();
    
    Hero* getHeroAtLocation(const std::string& location) const; // راستش نمیدونم پابلیک تعریف کردن این کار درستیه یانه
    std::shared_ptr<Hero> getHeroByName(const std::string& name) const;
    std::shared_ptr<Monster> getMonsterByName(const std::string& name) const;
    std::shared_ptr<Villager> getVillagerByName(const std::string& name) const;

    
    void initializeGame();
    void startGame(const std::vector<std::string>& playerNames);
    void startNextTurn();
    void monsterPhase();
    
    bool isGameOver() const { return gameOver; }
    bool checkWinConditions() const;
    bool checkLoseConditions();

    bool moveHero(const std::string& heroName, const std::string& destination);
    bool guideVillager(const std::string& heroName, const std::string& villagerName, const std::string& destination);
    bool pickUpItem(const std::string& heroName, const std::string& itemName);
    bool useSpecialAbility(const std::string& heroName, const std::string& optionalTarget = "");
    bool usePerkCard(const std::string& heroName, const std::string& cardName);
    bool advanceTask(const std::string& heroName, const std::string& monsterName);
    bool defeatMonster(const std::string& heroName, const std::string& monsterName);
    bool defendAttack(const std::shared_ptr<Hero>& hero);
    
    int getTerrorLevel() const { return terrorLevel; }
    const std::vector<std::shared_ptr<Hero>>& getHeroes() const { return heroes; }
    const std::vector<std::shared_ptr<Monster>>& getMonsters() const { return monsters; }
    const std::vector<std::shared_ptr<Player>>& getPlayers() const { return players; }
    std::string getCurrentPlayerName() const;
    std::shared_ptr<Hero> getCurrentHero() const;   
    std::shared_ptr<GameMap> getGameMap() const { return gameMap; }
    std::vector<std::shared_ptr<Item>> getItemsAtLocation(const std::string& location) const;
    std::vector<std::shared_ptr<Villager>> getVillagersAtLocation(const std::string& location) const;
    
    void increaseTerrorLevel(int amount = 1);
    void drawAndPlaceItems(int count);
    void overstockItems();
    void setSkipMonsterPhase(bool skip) { skipNextMonsterPhase = skip; }
    void placeMonster(const std::string& monsterName, const std::string& location);
    void placeVillagerOnBoard(const std::string& villagerName, const std::string& location);
    void moveMonsterToHero(const std::string& monsterName);
    void resolveStrikes(const std::vector<MonsterStrike>& strikes);
    
    void applyMonsterEvent(const std::string& cardName);
    void thiefEvent();
    void hypnoticGazeEvent();
    void onTheMoveEvent();
    
    void promptAndPlaceMonster(const std::string& monsterName);
    void promptAndMoveMonster(const std::string& monsterName, int steps);
    void promptAndMoveHero(const std::string& heroName, int steps);
    
    void moveMonsterTowardsTarget(const std::shared_ptr<Monster>& monster, int steps);
    TargetInfo findNearestTarget(const std::string& fromLocation) const;
    void handleAttack(const std::shared_ptr<Monster>& monster, int diceCount);
    void rescueVillager(const std::string& villagerName, const std::string& heroName);
    void drawPerkCard(const std::string& heroName);
    void updateFrenzyOrder(bool forceNext = false);
    
private:

    int terrorLevel;
    int turnCount;
    bool gameOver;
    bool skipNextMonsterPhase;
    int currentPlayerIndex;
    std::string frenziedMonster;

    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Hero>> heroes;
    std::vector<std::shared_ptr<Monster>> monsters;
    std::shared_ptr<GameMap> gameMap;
    std::vector<std::shared_ptr<Villager>> villagers;
    
    std::vector<std::shared_ptr<Item>> itemPool;
    std::vector<std::shared_ptr<PerkCard>> perkCardDeck;
    std::vector<std::shared_ptr<MonsterCard>> monsterCardDeck;
    
    void initializeHeroes();
    void initializeMonsters();
    void initializeItems();
    void initializeVillagers();
    void initializeCards();
    void distributeInitialPerkCards();


};

#endif
