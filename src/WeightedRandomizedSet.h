/*******************************************************************************
 * Project:     Potions
 * File:        WeightedRandomizedSet.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        11th September 2013
 * Standard:    C++98
 *
 * This template class maintains a collection of generic objects, which can be
 * retrieved with a likelihood determined by the weighting used at the time 
 * adding the item to the collection. The WeightedRandomizedSet achieves this 
 * by recording each cumulative weighting in ascending order as items are added.
 * The index of each stored cumulative weighting is equal to the index of the 
 * added item. To retrieve an item, the WeightedRandomizedSet chooses a random
 * number between 0 and the total weighting, and traverses the cumulative
 * weightings until one is found that is greater than the random value. The item
 * for the corresponding index is then returned.
 ******************************************************************************/

#pragma once
#include <vector>
#include <stdexcept>
#include <random>


template<class T> class WeightedRandomizedSet
{
public:
	
	// Initializes an empty WeightedRandomizedSet.
	WeightedRandomizedSet<T>();
	
	// Return true if the set is empty.
	bool isEmpty() const;
	
	// Returns is size of the set.
	int size() const;
	
	// Adds the item to the set and records it's weighting.
	void add(const T & item, const double weighting);
	
	// Retrieves an item with a probability according to it's weighting.
	// Throws logic_error if the set is empty.
	const T& retrieve();
	
private:
	
	// Holds the set's values
	std::vector<T> _values;
	
	// The set's random number device, generator and distribution
	std::default_random_engine _gen;
    std::uniform_real_distribution<double> _dis;
	
	// The cumulative probabilities with indices corresponding to their
	// represented values.
	std::vector<double> _probabilitySpace;
	
	// The sum of all elements' weightings
	double _probabilitySpaceSize;
};

//------------------------------------------------------------------------------
// Default constructor
template <class T>
WeightedRandomizedSet<T>::WeightedRandomizedSet() :
_probabilitySpace(0)
{
	// Build the probability distribution
    _dis = std::uniform_real_distribution<double>(0.0, 1.0);
}

//------------------------------------------------------------------------------
// Check whether set is empty.
template <class T>
bool WeightedRandomizedSet<T>::isEmpty() const
{
	return _values.empty();
}

//------------------------------------------------------------------------------
// Returns the number of items in the set.
template <class T>
int WeightedRandomizedSet<T>::size() const
{
	return _values.size();
}

//------------------------------------------------------------------------------
// Adds an item and stores its weighting
template <class T>
void WeightedRandomizedSet<T>::add(const T & item, const double weighting)
{	
	// Store the item
	_values.push_back(item);
	
	// Increase the size of probability space
	_probabilitySpaceSize += weighting;
	
	// Push this value to the map of probability space
	_probabilitySpace.push_back(_probabilitySpaceSize);
}

//------------------------------------------------------------------------------
// Returns an item with a probability according to its recorded weighting.
// non-const because of random mechanism
template <class T>
const T& WeightedRandomizedSet<T>::retrieve()
{
	// Check there's an item to return.
	if (isEmpty())
		throw std::logic_error("Attempted to retrieve an item from an empty"
			"WeightedRandomizedSet");
			
	// Select a random number within probability space
	double f = _dis(_gen) * _probabilitySpaceSize;
	
	// Find the index of the first element greater than this value in
	// probability space. Worst case linear - binary search would be better.
	int index = 0;
	std::vector<double>::const_iterator i = _probabilitySpace.begin();
	// Increase the index until we reach the end or f is greater.
	while (i != _probabilitySpace.end() && *(i++) < f) index++;
	
	// Return the index's corresponding value
	return _values[index];
}