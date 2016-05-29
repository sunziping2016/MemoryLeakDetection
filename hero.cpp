#include <iostream>
#include <string>

#include "hero.h"

using namespace std;

istream &operator >> (istream &in, Hero &hero)
{
    in >> hero.name >> hero.strength >> hero.intelligence >> hero.power >> hero.charisma >> hero.luck;
    return in;
}
ostream &operator << (ostream &out, const Hero &hero)
{
    out << hero.name << "\t" << hero.strength  << "\t" << hero.intelligence  << "\t" << hero.power  << "\t" << hero.charisma  << "\t" << hero.luck;
    return out;
}

