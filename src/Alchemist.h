/*******************************************************************************
 * Project:     Potions
 * File:        Alchemist.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (auto types and range-based loops)
 *
 * The Alchemist holds a stock of ingredients, cumulative knowledge of the 
 * ingredients' status effects, and the logic for combining ingredients into
 * potions. This class provides represents the information a Skyrim player has
 * access to when deciding what ingredients to combine.
 *
 * DISCLAIMER: The rules here used for determining a potion's effects and values
 * do not directly reflect those used by Skyrim's alchemy system. They are only
 * an approximation of those rules.
 *
 * Combination Rules for both 2 and 3 ingredients:
 * - If two or more status effects match, they become the resulting effect.
 * - The rarest of the matching effects are used.
 *
 * Combination Rules for 2 ingredients:
 * - Only one effect can be used by the resulting potion.
 * - The potion's value is determined by the effect's rarity.
 *
 * Combination Rules for 3 ingredients:
 * - One or two effects can be used by the resulting potions.
 * - The potion's value is determined by the sum of the effect's rarities.
 ******************************************************************************/
 
#pragma once
#include <vector>
#include <map>
#include "Ingredient.h"
#include "Discovery.h"


class Alchemist
{
public:
	// Returns the combined value of all the brewed potions
	double inventoryValue() const;
	
	// Returns the number of potions with no value
	int worthlessPotionCount();

#pragma mark Setup
	
	// Initializes an alchemist with no discoveries or stock.
	Alchemist();

	// Returns a new ingredient with random status effects and set's stock 
	// to zero. The ingredient's first status effect is also discovered
	const Ingredient discoverNewIngredient();

	// Refills the alchemist's stores by the specified amount with ingredients
	// according to their rarities.
	void forage(const int count);

#pragma mark -

#pragma mark Ingredient Stock Queries

	// Returns all known stocked and unstocked ingredients.
	std::vector<Ingredient> allKnownIngredients() const;

	// Returns true if the specified ingredient is stocked.
	bool hasIngredient(const Ingredient& ingredient) const;

	// Returns the stock count for the specified ingredient.
	int countOfIngredient(const Ingredient& ingredient) const;

	// Returns the total number of ingredients remaining.
	int totalIngredientsRemaining() const;
	
	// Returns the number of remaining of ingredient varieties still in stock.
	int calculateVarietiesInStock() const;
	
	// Returns a vector containing all status effects known by the alchemist.
	std::vector<StatusEffect> allKnownEffects() const;

	// Returns the total number of ingredients remaining in stock with the
	// specified effect.
	int calculateTotalIngredientsRemainingWithEffect
		(const StatusEffect& effect);

	// Returns all ingredients known to have the specified status effect
	const std::vector<Ingredient>& getIngredientsWithEffect
		(const StatusEffect& effect);
	
	// Returns true if an ingredient is known to express the status effect.
	bool ingredientHasEffect
		(const Ingredient& ingredient, const StatusEffect& effect) const;

#pragma mark -

	// Uses two or three ingredients to 'brew a potion' adding value to the 
	// inventory, and learning new status effects repeated in the ingredients
	Discovery combine
		(const Ingredient& i1, const Ingredient& i2);
	Discovery combine
		(const Ingredient& i1, const Ingredient& i2, const Ingredient& i3);

private:
	// The combined value of all the brewed potions.
	double _inventoryValue;
	
	// Keep a count of total ingredients in the store.
	int _totalIngredientsRemaining;
	
	// Keep a count of combinations that gained nothing.
	int _worthlessPotionCount;

	// Holds the quantity of each ingredient.
	std::map<Ingredient, unsigned int> _ingredientStore;

	// Sorts ingredients by their discovered effects
	std::map<StatusEffect, std::vector<Ingredient> > _effectsReference;
	
	// Note that an ingredient expresses a particular status effect.
	void learnIngredientEffect
		(const Ingredient& ingredient, const StatusEffect& effect);
};

