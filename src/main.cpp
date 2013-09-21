/*******************************************************************************
 * Project:     Potions
 * File:        main.cpp
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++98
 *
 * This program reads in a list of doubles which represent the rarities of the
 * status effects found in the world of Skyrim. It then generates ingredients 
 * from these effects, and has alchemist brew potions from them following 
 * different methods. Various results are logged.
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "Alchemist.h"
#include "Instructor.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Seed random
	srand(time(0));
	
	// Read in status effects from file
	const char* filename = argv[1];
	ifstream file(filename);
	if (file.is_open())
	{
		while (file.good())
		{
			double rarity;
			file >> rarity;
			StatusEffect::newStatusEffect(rarity);
		}
	}
	else // failure to open file - use default effect rarities
	{
		cout << "Couldn't open effects file, Using default values" << endl;
		for (int i = 0; i < 20; i++)
			StatusEffect::newStatusEffect(1.0);
		for (int i = 0; i < 10; i++)
			StatusEffect::newStatusEffect(5.0);
		for (int i = 0; i < 2; i++)
			StatusEffect::newStatusEffect(50.0);
	}
	
	// Build some alchemists
	Alchemist alchemistA = Alchemist();
	
	// Discover some ingredients
	for (int i = 0; i < 60; i++)
		alchemistA.discoverNewIngredient();
	
	// Harvest ingredients to use
	alchemistA.forage(1000);
	
	// Create alchemist with same conditions
	Alchemist alchemistB = alchemistA;
	
	// See what we earn from random mixing
	Instructor::randomlyCombineRemainingPairs(alchemistA);
	cout << "Approach A" << endl
		 << "Inventory Value: " << alchemistA.inventoryValue() << endl
		 << "Worthless Potions: " << alchemistA.worthlessPotionCount() << endl
		 << "Varieties Remaining: " << alchemistA.calculateVarietiesInStock()
		 << endl
		 << "Ingredients Remaining: " << alchemistA.totalIngredientsRemaining()
		 << endl;
	
	// See what we earn from mixing matching effects
	Instructor::combineAllPairsWithMatchingEffects(alchemistB);
	Instructor::randomlyCombineRemainingPairs(alchemistB);
	cout << "Approach B" << endl
		 << "Inventory Value: " << alchemistB.inventoryValue() << endl
		 << "Worthless Potions: " << alchemistB.worthlessPotionCount() << endl
		 << "Varieties Remaining: " << alchemistB.calculateVarietiesInStock()
		 << endl
		 << "Ingredients Remaining: " << alchemistB.totalIngredientsRemaining()
		 << endl;
	
	return 0;
}