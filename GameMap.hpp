#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Item.hpp"
#include "Villager.hpp"


enum class Location{
    DOCKS, THEATRE, CAMP, LABORATORY, MANSION, INN, BARN, 
    INSTITUTE, PRECINCT, CAVE, DUNGEON, CRYPT, GRAVEYARD ,
    HOSPITAL , CHURCH , MUSEUM , ABBEY , SHOP , TOWER
};

class GameMap {
private:

    std::vector<std::string> locations; 
    std::multimap<std::string, std::string> connections; 
    std::multimap<std::string, std::shared_ptr<Item>> itemsByLocation;
    std::multimap<std::string, std::shared_ptr<Villager>> villagersByLocation; 

public:

    GameMap();
    void addLocation(const std::string& name);
    void connectLocations(const std::string& loc1, const std::string& loc2); 
    bool areConnected(const std::string& loc1, const std::string& loc2) const; 
    void placeItem(std::shared_ptr<Item> item, const std::string& location);
    void placeVillager(std::shared_ptr<Villager> villager, const std::string& location);
    std::vector<std::shared_ptr<Item>> getItemsAt(const std::string& location) const; 
    std::vector<std::shared_ptr<Villager>> getVillagersAt(const std::string& location) const; 
    std::vector<std::string> getConnectedLocations(const std::string& location) const; 
    int calculateDistance(const std::string& from, const std::string& to) const; 
    std::string findShortestPath(const std::string& from, const std::string& to) const; 
    std::shared_ptr<Item> removeItem(const std::string& location, const std::string& itemName);
    std::vector<std::string> getAllLocations() const { return locations; } 

};

#endif