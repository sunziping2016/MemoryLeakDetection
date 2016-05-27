#ifndef HERO_H_INCLUDE
#define HERO_H_INCLUDE

#include <string>
#include <iostream>

class Hero
{
public:
	std::string name;
	int strength;
	int intelligence;
	int power;
	int charisma;
	int luck;

	Hero(): name(), strength(0), intelligence(0), power(0), charisma(0), luck(0) {}
};

std::istream &operator >> (std::istream &in, Hero &hero);
std::ostream &operator << (std::ostream &out, const Hero &hero);

#endif
