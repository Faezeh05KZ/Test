#ifndef GAME_UTILS_HPP
#define GAME_UTILS_HPP

#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"
#include "Item.hpp"
#include "GameMap.hpp"
#include "PerkCard.hpp"
#include "MonsterCard.hpp"

class GameUtils {
    public:
    
        static int rollDice(int diceCount); // انداختن تاس
        static int randomInt(int min, int max); // تولید عدد تصادفی
        static double randomDouble(double min, double max); // تولید عدد اعشاری تصادفی
        
        static std::shared_ptr<PerkCard> drawRandomPerkCard(std::vector<std::shared_ptr<Card>>& cards); // انتخاب تصادفی کارت پرک
        static std::shared_ptr<MonsterCard> drawRandomMonsterCard(std::vector<std::shared_ptr<Card>>& cards); // انتخاب تصادفی کارت هیولا
        static void shuffleCards(std::vector<std::shared_ptr<Card>>& cards); // به هم ریختن ترتیب کارت ها 
        
        static std::shared_ptr<Item> drawRandomItem(std::vector<std::shared_ptr<Item>>& items); // انتخاب تصادفی ایتم 
        static std::vector<std::shared_ptr<Item>> filterItemsByColor(
        const std::vector<std::shared_ptr<Item>>& items, ItemColor color); // فیلتر ایتم بر اساس رنگ 
        static int calculateTotalPower(const std::vector<std::shared_ptr<Item>>& items); // محاسبه مجموع قدرت ها 
        
        static std::string toLower(const std::string& str); // تبدیل رشته به حروف کوچک
        static std::string trim(const std::string& str); // حذف فاصله اضافی بین رشته ها 
        static std::vector<std::string> splitString(const std::string& str, char delimiter); // تقسیم رشته بر اساس جداکننده


        static bool isValidHeroName(const std::string& name); // بررسی نام قهرمان
        static bool isValidItemName(const std::string& name); // بررسی نام ایتم 
        static bool isValidLocationName(const std::string& name); // بررسی مکان
};
#endif
