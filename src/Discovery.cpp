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
// Default Constructor
Discovery::Discovery() :
_count(0)
{
}

//------------------------------------------------------------------------------
// Returns the number of findings
int Discovery::findingsCount() const
{
	return _count;
}

//------------------------------------------------------------------------------
// Add a finding to the discovery
void Discovery::addFinding
	(const Ingredient & ingredient, const StatusEffect & effect)
{
	if (_count == MAX_FINDINGS)
		throw range_error("Attempted to add to a Discovery that was "
			"already full.");
	
	// Store the values
	_ingredients[_count] = ingredient;
	_effects[_count] = effect;
	_count++;
}

//------------------------------------------------------------------------------
// Retrieve the ingredient for an index.
const Ingredient & Discovery::getIngredient(const int index) const
{
	// Check range is valid
	if (index < 0 || index >= _count)
		throw out_of_range("Attempted to access an ingredient outside a "
			"Discovery's range.");
	
	// Fetch the value
	return _ingredients[index];
}

//------------------------------------------------------------------------------
// Retrieve the ingredient for an index.
const StatusEffect & Discovery::getEffect(const int index) const
{
	// Check range is valid
	if (index < 0 || index >= _count)
		throw out_of_range("Attempted to access a status effect outside a "
			"Discovery's range.");
	
	// Fetch the value
	return _effects[index];
}