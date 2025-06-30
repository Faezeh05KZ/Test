#include "Archaeologist.hpp"
#include <utility>

using namespace std;

Archaeologist::Archaeologist(string name, string startingLocation)
    : Hero(move(name), HeroType::ARCHAEOLOGIST, 4, move(startingLocation)) {}

bool Archaeologist::hasSpecialAction() const
{
    return true;
}

