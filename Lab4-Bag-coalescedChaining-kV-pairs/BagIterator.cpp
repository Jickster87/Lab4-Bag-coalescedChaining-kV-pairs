#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;

BagIterator::BagIterator(const Bag& c) : bag(c) {
    first();
}

void BagIterator::first() {
    currentPosition = 0;
    currentFrequency = 1;

    // Move to the first non-empty element
    while (currentPosition < bag.maxCapacity && bag.Table[currentPosition].pair == NULL_TPAIR) {
        currentPosition++;
    }
}

void BagIterator::next() {
    if (!valid()) {
        throw exception();
    }

    // Increment the frequency counter within the current position
    currentFrequency++;
    
    // If we have exceeded the frequency of the current element, move to the next position
    if (currentFrequency > bag.Table[currentPosition].pair.second) {
        // Reset frequency and move to the next position in the table
        currentFrequency = 1;
        currentPosition++;

        // Find the next non-empty position
        while (currentPosition < bag.maxCapacity && bag.Table[currentPosition].pair == NULL_TPAIR) {
            currentPosition++;
        }
    }
}

bool BagIterator::valid() const {
    // Check if currentPosition is within bounds and pointing to a valid element
    return currentPosition < bag.maxCapacity && bag.Table[currentPosition].pair != NULL_TPAIR;
}

TElem BagIterator::getCurrent() const {
    if (!valid()) {
        throw exception();
    }
    return bag.Table[currentPosition].pair.first;
}
