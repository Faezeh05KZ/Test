#include "GameMap.hpp"
#include <algorithm>
#include <queue>
#include <sstream>
#include <unordered_map>


GameMap::GameMap() {
    locations = {
        "Docks", "Theatre", "Camp", "Laboratory", "Mansion",
        "Inn", "Barn", "Institute", "Precinct", "Cave",
        "Dungeon", "Crypt", "Graveyard", "Hospital", "Church",
        "Museum", "Abbey", "Shop", "Tower"
    };

    connectLocations("Institute", "Laboratory");
    connectLocations("Graveyard", "Church");
    connectLocations("Hospital", "Church");
    connectLocations("Museum", "Shop");
    connectLocations("Museum", "Mansion");
    connectLocations("Museum", "Church");
    connectLocations("Crypt", "Abbey");
    connectLocations("Abbey", "Mansion");
    connectLocations("Church", "Shop");
    connectLocations("Church", "Mansion");
    connectLocations("Laboratory", "Shop");
    connectLocations("Shop", "Theatre");
    connectLocations("Shop", "Mansion");
    connectLocations("Mansion", "Camp");
    connectLocations("Mansion", "Precinct");
    connectLocations("Mansion", "Inn");
    connectLocations("Mansion", "Theatre");
    connectLocations("Docks", "Tower");
    connectLocations("Tower", "Theatre");
    connectLocations("Tower", "Dungeon");
    connectLocations("Theatre", "Barn");
    connectLocations("Theatre", "Inn");
    connectLocations("Theatre", "Precinct");
    connectLocations("Theatre", "Camp");
    connectLocations("Inn", "Precinct");
    connectLocations("Inn", "Camp");
    connectLocations("Precinct", "Camp");
    connectLocations("Camp", "Cave");

}

void GameMap::addLocation(const std::string& name) {
    if (std::find(locations.begin(), locations.end(), name) == locations.end()) {
        locations.push_back(name);
    }
}

void GameMap::connectLocations(const std::string& loc1, const std::string& loc2) {
    connections.insert({loc1, loc2});
    connections.insert({loc2, loc1});
}

bool GameMap::areConnected(const std::string& loc1, const std::string& loc2) const {
    auto range = connections.equal_range(loc1);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == loc2) return true;
    }
    return false;
}

void GameMap::placeItem(std::shared_ptr<Item> item, const std::string& location) {
    itemsByLocation.insert({location, item});
}

void GameMap::placeVillager(std::shared_ptr<Villager> villager, const std::string& location) {
    villagersByLocation.insert({location, villager});
}

std::vector<std::shared_ptr<Item>> GameMap::getItemsAt(const std::string& location) const {
    std::vector<std::shared_ptr<Item>> items;
    auto range = itemsByLocation.equal_range(location);
    for (auto it = range.first; it != range.second; ++it) {
        items.push_back(it->second);
    }
    return items;
}

std::vector<std::shared_ptr<Villager>> GameMap::getVillagersAt(const std::string& location) const {
    std::vector<std::shared_ptr<Villager>> villagers;
    auto range = villagersByLocation.equal_range(location);
    for (auto it = range.first; it != range.second; ++it) {
        villagers.push_back(it->second);
    }
    return villagers;
}

std::vector<std::string> GameMap::getConnectedLocations(const std::string& location) const {
    std::vector<std::string> connected;
    auto range = connections.equal_range(location);
    for (auto it = range.first; it != range.second; ++it) {
        connected.push_back(it->second);
    }
    return connected;
}

int GameMap::calculateDistance(const std::string& from, const std::string& to) const {
    if (from == to) return 0;
    
    std::queue<std::string> q;
    std::unordered_map<std::string, int> distances;
    
    q.push(from);
    distances[from] = 0;
    
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        auto neighbors = getConnectedLocations(current);
        for (const auto& neighbor : neighbors) {
            if (distances.find(neighbor) == distances.end()) {
                distances[neighbor] = distances[current] + 1;
                
                if (neighbor == to) {
                    return distances[neighbor];
                }
                
                q.push(neighbor);
            }
        }
    }
    
    return -1;
}

std::string GameMap::findShortestPath(const std::string& from, const std::string& to) const {
    if (from == to) return from;
    
    std::queue<std::string> q;
    std::unordered_map<std::string, std::string> previous;
    
    q.push(from);
    previous[from] = "";
    
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        auto neighbors = getConnectedLocations(current);
        for (const auto& neighbor : neighbors) {
            if (previous.find(neighbor) == previous.end()) {
                previous[neighbor] = current;
                
                if (neighbor == to) {
                    std::vector<std::string> path;
                    std::string step = to;
                    while (step != from) {
                        path.push_back(step);
                        step = previous[step];
                    }
                    path.push_back(from);
                    
                    std::reverse(path.begin(), path.end());
                    
                    std::stringstream ss;
                    for (size_t i = 0; i < path.size(); ++i) {
                        if (i != 0) ss << " -> ";
                        ss << path[i];
                    }
                    return ss.str();
                }
                
                q.push(neighbor);
            }
        }
    }
    
    return "";
}


std::shared_ptr<Item> GameMap::removeItem(const std::string& location, const std::string& itemName) {
    auto range = itemsByLocation.equal_range(location);
    for (auto it = range.first; it != range.second; ) {
        if (it->second->getName() == itemName) {
            auto item = it->second;
            it = itemsByLocation.erase(it);
            return item;
        } 
        else {
            ++it;
        }
    }
    return nullptr;
}
