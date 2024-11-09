#include "ShortTest.h"
#include <assert.h>
#include "Bag.h"
#include "BagIterator.h"
#include <iostream>



void testAll() {
    Bag b;
    assert(b.isEmpty() == true);
    assert(b.size() == 0);
    
    // Adding elements
    std::cout << "Adding elements:\n";
    b.add(5);
    b.printTable();
    
    b.add(1);
    b.printTable();
    
    b.add(10);
    b.printTable();
    
    b.add(7);
    b.printTable();
    
    b.add(1); // Adding duplicate to check frequency count
    b.printTable();
    
    b.add(11);
    b.printTable();
    
    b.add(-3);
    b.printTable();
    
    // Check size and occurrences
    std::cout << "Checking Bag size and occurrences...\n";
    assert(b.size() == 7);
    
    // Search checks
    std::cout << "Searching for elements:\n";
    std::cout << "Searching for 10: " << (b.search(10) ? "Found" : "Not Found") << "\n";
    assert(b.search(10) == true);
    
    std::cout << "Searching for 16: " << (b.search(16) ? "Found" : "Not Found") << "\n";
    assert(b.search(16) == false);
    
    // Occurrence checks
    std::cout << "Checking occurrences of elements:\n";
    assert(b.nrOccurrences(1) == 2);
    assert(b.nrOccurrences(7) == 1);
    
    // Remove elements
    std::cout << "Removing elements:\n";
    std::cout << "Removing 1 (should succeed): " << (b.remove(1) ? "Removed" : "Not Removed") << "\n";
    b.printTable();
    
    std::cout << "Removing 6 (should fail): " << (b.remove(6) ? "Removed" : "Not Removed") << "\n";
    b.printTable();
    
    // Check size and occurrences after removal
    std::cout << "Final checks after removal:\n";
    assert(b.size() == 6);
    assert(b.nrOccurrences(1) == 1);
    
    // Iterator test
    BagIterator it = b.iterator();
    it.first();
    std::cout << "Iterating over elements:\n";
    while (it.valid()) {
        TElem e = it.getCurrent();
       // std::cout << "Iterator at element: (" << e.first << ", " << e.second << ")\n";
        it.next();
    }
    std::cout << "End of iteration.\n";
}
