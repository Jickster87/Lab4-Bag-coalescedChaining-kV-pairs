#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
using namespace std;


Bag::Bag() {
    maxCapacity = 31;
    firstEmpty = 0;
    sizeBag = 0;
    alphaLoad = 0.0f;
    Table = new coalescedNode[maxCapacity];
    initTable();
}

void Bag::updateFirstEmpty() {
    firstEmpty++;
    while (firstEmpty < maxCapacity && Table[firstEmpty].pair != NULL_TPAIR) {
        firstEmpty++;
    }
}

void Bag::initTable() {
    for (int i = 0; i < maxCapacity; i++) {
        Table[i].pair = NULL_TPAIR;
        Table[i].next = -1;
    }
}

void Bag::resizeRehash() {
    //new table with double the size
    int newMaxCapacity = maxCapacity * 2;
    coalescedNode * newTable = new coalescedNode[newMaxCapacity];
    
    //initialize the new table
    for (int i = 0; i < newMaxCapacity; i++) {
        newTable[i].pair = NULL_TPAIR;
        newTable[i].next = -1;
    }
    
    //rehash the elements
    for (int i = 0; i < maxCapacity; i++) {
        if (Table[i].pair != NULL_TPAIR) {
            //hash with new capacity
            int newPos = abs(Table[i].pair.first) % newMaxCapacity;
            
            if (newTable[newPos].pair == NULL_TPAIR) {
                // Place directly if slot is empty
                newTable[newPos] = Table[i];
            } else {
                // Handle collision using chaining
                int prevPos = newPos;
                while (newTable[prevPos].next != -1) {
                    prevPos = newTable[prevPos].next;
                }
                updateFirstEmpty();
                newTable[prevPos].next = firstEmpty;
                newTable[firstEmpty] = Table[i];
            }
        }
    }
    delete [] Table;
    Table = newTable;
    maxCapacity = newMaxCapacity;
    updateFirstEmpty();
    updateAlpha();
    
}

int Bag::h(TElem elem) const {
    return abs(elem) % maxCapacity;
}

void Bag::updateAlpha() {
    alphaLoad = static_cast<float>(sizeBag) / maxCapacity;
}


void Bag::add(TElem elem) {
    //if the load factor is greater than 0.7, resize and rehash the table
    if (alphaLoad > 0.7) {
        resizeRehash();
    }
    //compute the hash value of the element
    int position = h(elem);
    int currentPos = position;
    bool inserted = false;
    
    while (!inserted) {
        //case 1: the position is empty
        if (Table[currentPos].pair == NULL_TPAIR) {
            Table[currentPos].pair.first = elem;
            Table[currentPos].pair.second = 1;
            sizeBag++;
            inserted = true;
        }
        else if (Table[currentPos].pair.first == elem) {
            Table[currentPos].pair.second++;
            inserted = true;
            sizeBag++;
        }
        else {
            if (Table[currentPos].next == -1) {
                updateFirstEmpty();
                currentPos = firstEmpty;
            } else {
                currentPos = Table[currentPos].next;
            }
        }
    }
    
    updateAlpha();
}


bool Bag::remove(TElem elem) {
    int pos = h(elem);
    int prevPos = -1;

    // Search for the element in the chain
    while (pos != -1 && Table[pos].pair.first != elem) {
        prevPos = pos;
        pos = Table[pos].next;
    }

    // Element not found
    if (pos == -1) {
        return false;
    }

    // Element found, decrease frequency
    Table[pos].pair.second--;
    sizeBag--;

    // If frequency becomes 0, remove the element from the table
    if (Table[pos].pair.second == 0) {
        Table[pos].pair = NULL_TPAIR;

        // Adjust the chain if necessary
        if (prevPos != -1) {
            Table[prevPos].next = Table[pos].next;
        } else if (Table[pos].next != -1) {
            // If the removed element was the head of a chain, move the next element up
            int nextPos = Table[pos].next;
            Table[pos] = Table[nextPos];
            Table[nextPos].pair = NULL_TPAIR;
            Table[nextPos].next = -1;
        } else {
            // If there's no chain, just mark the spot as free
            if (pos < firstEmpty) {
                firstEmpty = pos;
            }
        }
    }

    // Update load factor
    updateAlpha();
    return true;
}


bool Bag::search(TElem elem) const {
    int pos = h(elem);
    // Traverse the chain to find the element
    while (pos != -1) {
        if (Table[pos].pair.first == elem) {
            return true;
        }
        pos = Table[pos].next;
    }
    return false;
}

void Bag::printTable() const {
          std::cout << "Current state of the hash table:\n";
          for (int i = 0; i < maxCapacity; ++i) {
              if (Table[i].pair != NULL_TPAIR) {
                  std::cout << "Index " << i << ": ("
                            << Table[i].pair.first << ", "
                            << Table[i].pair.second << "), Next: "
                            << Table[i].next << "\n";
              } else {
                  std::cout << "Index " << i << ": NULL\n";
              }
          }
          std::cout << "First empty position: " << firstEmpty << "\n";
          std::cout << "Table load factor: " << alphaLoad << "\n";
          std::cout << "Table size: " << size() << "\n\n";
      }

int Bag::nrOccurrences(TElem elem) const {
    // Get the hash position of the element
    int pos = h(elem);
    // Traverse the chain at that position
    while (pos != -1 && Table[pos].pair.first != NULL_TPAIR.first) {
        if (Table[pos].pair.first == elem) {
            return Table[pos].pair.second;
        }
        pos = Table[pos].next;
    }
    return 0;  // Element not found
}


int Bag::size() const {
    return sizeBag;
}


bool Bag::isEmpty() const {
    return sizeBag == 0;
}

BagIterator Bag::iterator() const {
    return BagIterator(*this);
}


Bag::~Bag() {
    delete [] Table;
}

