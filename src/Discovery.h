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

class Discovery
{
	struct Finding
	{
		Ingredient ingredient;
		StatusEffect effect;
		
		Finding(const Ingredient& i, const StatusEffect& e) :
			ingredient(i), effect(e) {}
	};

	std::vector<Finding> findings;
	
public:
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