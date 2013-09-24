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
count(0)
{
}

//------------------------------------------------------------------------------
// Returns the number of findings
int Discovery::findingsCount() const
{
	return this->count;
}

//------------------------------------------------------------------------------
// Add a finding to the discovery
void Discovery::addFinding
	(const Ingredient & ingredient, const StatusEffect & effect)
{
	if (this->count == MAX_FINDINGS)
		throw range_error("Attempted to add to a Discovery that was "
			"already full.");
	
	// Store the values
	this->ingredients[this->count] = ingredient;
	this->effects[this->count] = effect;
	this->count++;
}

//------------------------------------------------------------------------------
// Retrieve the ingredient for an index.
const Ingredient & Discovery::getIngredient(const int index) const
{
	// Check range is valid
	if (index < 0 || index >= this->count)
		throw out_of_range("Attempted to access an ingredient outside a "
			"Discovery's range.");
	
	// Fetch the value
	return this->ingredients[index];
}

//------------------------------------------------------------------------------
// Retrieve the ingredient for an index.
const StatusEffect & Discovery::getEffect(const int index) const
{
	// Check range is valid
	if (index < 0 || index >= this->count)
		throw out_of_range("Attempted to access a status effect outside a "
			"Discovery's range.");
	
	// Fetch the value
	return this->effects[index];
}