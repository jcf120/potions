/*******************************************************************************
 * Project:     Potions
 * File:        Alchemist.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (auto types and range-based loops)
 ******************************************************************************/

#include "Alchemist.h"
#include <stdexcept>
#include <algorithm>
#include "WeightedRandomizedStack.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
//                                 Setup
//
////////////////////////////////////////////////////////////////////////////////	

//------------------------------------------------------------------------------
Alchemist::Alchemist() :
inventoryValue(0.0), totalIngredientsRemaining(0), worthlessPotionCount(0)
{
}

//------------------------------------------------------------------------------
// 'Discover' a new unique ingredient and make a record of it as empty in stock.
const Ingredient Alchemist::discoverNewIngredient()
{
	// Fetch a new ingredient with unique id and properties.
	const Ingredient ingredient = Ingredient::newIngredient();
	
	// Add the ingredient to the store with a stock of zero.
	this->ingredientStore[ingredient] = 0;
	
	// 'Eat' the ingredient and learn it's first effect
	learnIngredientEffect(ingredient, ingredient[0]);
	
	// Return the new ingredient
	return ingredient;
}

//------------------------------------------------------------------------------
// Forage - adds a specified number of ingredients to the store. The ingredient
// varieties are determined by the rarities of those that have been 'discovered'
void Alchemist::forage(const int count)
{
	// Generate a WeightedRandomizedStack from the discovered ingredients. This
	// represents the garden from which ingredients are foraged.
	WeightedRandomizedStack<Ingredient> garden;
	
	// Fills the garden with potential ingredients and info about rarity.
	for (auto& p : this->ingredientStore) // p is an Ingredient-int pair
		garden.push(p.first, 1.0 / p.first.calculateRarity());
	
	// Fetch ingredients from the garden the specified number of times.
	// Increment the stock of each ingredient retrieved.
	for (int i = 0; i < count; i++)
		this->ingredientStore[garden.peak()]++;
	
	// Note the increase in stock
	this->totalIngredientsRemaining += count;
}

////////////////////////////////////////////////////////////////////////////////
//
//                               Accessors
//
////////////////////////////////////////////////////////////////////////////////	

//------------------------------------------------------------------------------
double Alchemist::getInventoryValue() const
{
	return this->inventoryValue;
}

//------------------------------------------------------------------------------
int Alchemist::getWorthlessPotionCount() const
{
	return this->worthlessPotionCount;
}

//------------------------------------------------------------------------------
int Alchemist::getTotalIngredientsRemaining() const
{
	return this->totalIngredientsRemaining;
}

////////////////////////////////////////////////////////////////////////////////
//
//                   Effect & Ingredient Stock Queries
//
////////////////////////////////////////////////////////////////////////////////	

//------------------------------------------------------------------------------
// Returns a vector of all known ingredients - even if stock is zero
vector<Ingredient> Alchemist::allKnownIngredients() const
{
	// State the vector's size, so it isn't returned needlessly large.
	vector<Ingredient> ingredients(this->ingredientStore.size());
	
	// Copy in the ingredients
	int i = 0;
	for (auto& p : this->ingredientStore) // p is a pair reference
		ingredients[i++] = p.first;
	
	// Return the ingredients
	return ingredients;
}

//------------------------------------------------------------------------------
// Check if an ingredient is currently stocked (one or more)
bool Alchemist::hasIngredient(const Ingredient& ingredient) const
{
	return countOfIngredient(ingredient) > 0;
}

//------------------------------------------------------------------------------
// Returns the stock count of the specified ingredient
int Alchemist::countOfIngredient(const Ingredient& ingredient) const
{
	// Search for the ingredient in the map.
	auto iP = this->ingredientStore.find(ingredient); // iP is a pair iterator
	
	// Check a value was found.
	if (iP == this->ingredientStore.end())
		return 0;
	
	// Return the pair stock value
	return iP->second;
}

//------------------------------------------------------------------------------
// Returns the number of ingredient varieties still in stock.
int Alchemist::calculateVarietiesInStock() const
{
	int varietiesInStock = 0;
	
	// Increment for each ingredient stock more than 0;
	for (auto& p : this->ingredientStore) // p is an ingredient-int pair
		if (p.second > 0) varietiesInStock++;
		
	// Return the calculated value
	return varietiesInStock;
}
//------------------------------------------------------------------------------
// Returns a vector of all StatusEffects in the effectsReference map.
vector<StatusEffect> Alchemist::allKnownEffects() const
{
	vector<StatusEffect> effects;
	
	// Add each key to the vector
	for (auto& p : this->effectsReference) // p is a effect-vector pair
		effects.push_back(p.first);
	
	// Return all the vector of keys
	return effects;
}

//------------------------------------------------------------------------------
// Counts the stock of each ingredient listed under an effect
unsigned int Alchemist::calculateTotalIngredientsRemainingWithEffect
	(const StatusEffect& effect) const
{
	unsigned int count = 0;
	for (const Ingredient& ingr : this->effectsReference.at(effect))
		count += this->ingredientStore.at(ingr);
	
	return count;
}

//------------------------------------------------------------------------------
// Returns the vector of ingredients in effectsReference for the effect.
const vector<Ingredient>& Alchemist::getIngredientsWithEffect
	(const StatusEffect& effect) const
{
	return this->effectsReference.at(effect);
}

//------------------------------------------------------------------------------
// Returns true if the ingredient is found under the effect in effectsReference.
bool Alchemist::ingredientHasEffect
	(const Ingredient& ingredient, const StatusEffect& effect) const
{
	// Has the effect been registered?
	auto iRef = this->effectsReference.find(effect); // iRef is a pair iterator
	if (iRef == this->effectsReference.end())
		return false;
	
	// Is the ingredient listed under the found effect?
	const vector<Ingredient>& vec = iRef->second;
	return find(vec.begin(), vec.end(), ingredient) != vec.end();
}


////////////////////////////////////////////////////////////////////////////////
//
//                   Effect & Ingredient Stock Queries
//
////////////////////////////////////////////////////////////////////////////////	

//------------------------------------------------------------------------------
// Combine ingredients - increase inventory value and record common effects
Discovery Alchemist::combine
	(const Ingredient& ingredient1, const Ingredient& ingredient2)
{
	// Are both ingredients in stock?
	if (!hasIngredient(ingredient1) || !hasIngredient(ingredient2))
		throw logic_error("Alchemist attempted to combine ingredients that "
			"weren't in stock.");
	
	// Make sure the two ingredients are different.
	if (ingredient1 == ingredient2)
		throw invalid_argument("Alchemist::combine() - ingredient1 and "
			"ingredient2 cannot share the same value");
	
	// Remove one of each ingredient from the store
	this->ingredientStore[ingredient1]--;
	this->ingredientStore[ingredient2]--;
	this->totalIngredientsRemaining -= 2;
	
	// Findings from this combination will be returned with this object.
	// If nothing is learned on gained, this will be returned empty.
	Discovery discovery;
	
	// Find the rarest matching status effects in the ingredients.
	const StatusEffect* pRarestEffect = nullptr;
	
	// Loop over all possible effect combinations and check for matches.
	for (const StatusEffect& eff1 : ingredient1)
		for (const StatusEffect& eff2 : ingredient2)
			if (eff1 == eff2)
			{
				// Note the match if none were previously found.
				if (!pRarestEffect)
					pRarestEffect = &eff1;
				// Else, note the match if the effect's rarer than the last.
				else if (pRarestEffect->getRarity() < eff1.getRarity())
					pRarestEffect = &eff1;
				
				// Check if the effects have already been discovered, and learn 
				// them if not. Note anything learned in the returned Discovery.
				if (!ingredientHasEffect(ingredient1, eff1)) {
					learnIngredientEffect(ingredient1, eff1);
					discovery.addFinding(ingredient1, eff1);
				}
				if (!ingredientHasEffect(ingredient2, eff2)) {
					learnIngredientEffect(ingredient2, eff2);
					discovery.addFinding(ingredient2, eff2);
				}
			}
			
	// If a match was found, set the potion's value as the effect's rarity.
	if (pRarestEffect) {
		discovery.potionValue = pRarestEffect->getRarity();
		
		// Increase the inventory's value by that of the potion
		this->inventoryValue += discovery.potionValue;
	}
	// Otherwise, note the waste of ingredients
	else {
		this->worthlessPotionCount++;
	}
	
	return discovery;
}


////////////////////////////////////////////////////////////////////////////////
//
//                              Private Methods
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Notes a new status effect for an ingredient. 
void Alchemist::learnIngredientEffect
	(const Ingredient& ingredient, const StatusEffect& effect)
{
	// Check the ingredient is know to the alchemist
	// iStore is an ingredient-int pair iterator
	auto iStore = this->ingredientStore.find(ingredient);
	if (iStore == this->ingredientStore.end())
		throw logic_error("Alchemist attempted to learn the effect of an "
			"ingredient that it's encountered before.");
	
	// Look for the status effect in the effectsRefence.
	auto iRef = this->effectsReference.find(effect); // effect-vec pair iterator
	
	// Add the effect to the reference if it hasn't been before, along with the
	// ingredient exhibiting the effect.
	if (iRef == this->effectsReference.end()) {
		this->effectsReference[effect] = vector<Ingredient>({ingredient});
	}
	
	// Otherwise add the ingredient under the effect if it hasn't been already.
	else {
		vector<Ingredient>& vec = iRef->second;
		if (find(vec.begin(), vec.end(), ingredient) == vec.end())
			vec.push_back(ingredient);
		}
}