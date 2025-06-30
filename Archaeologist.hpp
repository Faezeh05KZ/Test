#ifndef ARCHAEOLOGIST_HPP
#define ARCHAEOLOGIST_HPP

#include "Hero.hpp"

class Archaeologist : public Hero
{
public:
    Archaeologist(std::string name, std::string startingLocation);
    bool hasSpecialAction() const override;
};

#endif

