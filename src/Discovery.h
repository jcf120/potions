/*******************************************************************************
 * Project:     Potions
 * File:        Discovery.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++98
 *
 * A container for information uncovered by the alchemist upon combining 
 * ingredients. This includes the association of a status effect with an 
 * ingredient and the value of the created potion.
 ******************************************************************************/

#pragma once
#include "Ingredient.h"
#include "StatusEffect.h"

// The maximum possible number of discoveries from combining 3 ingredients is:
// 3 (ingredients) * 4 (effects) / 2 (number needed for a match) = 6.
#define MAX_FINDINGS 6

class Discovery
{
	// The number of findings
	int count;
	// Storing the findings
	Ingredient ingredients[MAX_FINDINGS];
	StatusEffect effects[MAX_FINDINGS];
	
public:
	// Default Constructor - initialises the container empty.
	Discovery();
	
	// The increase in inventory value from combining the ingredients.
	double potionValue;
	
	// Returns the number of findings
	int findingsCount() const;
	
	// Stores the finding and increments count
	void addFinding(const Ingredient& ingredient, const StatusEffect& effect);
	
	// Finding accessors
	const Ingredient& getIngredient(const int index) const;
	const StatusEffect& getEffect(const int index) const;
};