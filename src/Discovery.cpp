/*******************************************************************************
 * Project:     Potions
 * File:        Discovery.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++98
 ******************************************************************************/


#include "Discovery.h"
#include <stdexcept>

using namespace std;

//------------------------------------------------------------------------------
Discovery::Discovery()
{
}

//------------------------------------------------------------------------------
int Discovery::findingsCount() const
{
	return this->findings.size();
}

//------------------------------------------------------------------------------
// Store the ingredient and effect together in a struct.
void Discovery::addFinding
	(const Ingredient & ingredient, const StatusEffect & effect)
{
	this->findings.push_back(Finding(ingredient, effect));
}

//------------------------------------------------------------------------------
const Ingredient & Discovery::getIngredient(const int index) const
{
	return this->findings[index].ingredient;
}

//------------------------------------------------------------------------------
const StatusEffect & Discovery::getEffect(const int index) const
{
	return this->findings[index].effect;
}