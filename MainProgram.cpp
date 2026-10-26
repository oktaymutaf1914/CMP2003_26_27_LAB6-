/************************************************************
 CMP2003 - Data Structures
 LAB 6 - Array-Based Linked List  +  Recursion
 ------------------------------------------------------------
 THIS IS THE ONLY FILE YOU EDIT AND SUBMIT.

 Build & run (no other files needed):
     make run
 or  g++ -Wall --std=c++17 MainProgram.cpp -o main && ./main

 The program compiles and runs from the start: unfinished
 functions just print a "not implemented" message in main().
 Your score: push, then look at 'Releases' in your repository.

 YOUR TASKS  (do them in order)
   PART 1 - ARRAY-BASED LINKED LIST            (~40 min, 60 pts)
     Q1 - search(item)                          15 pts
     Q2 - insertFirst(item)                     15 pts
     Q3 - insertLast(item)                      15 pts
     Q4 - deleteNode(item)                      15 pts
   PART 2 - RECURSION                           (~30 min, 40 pts)
     R1 - isPalindrome(s, left, right)          20 pts
     R2 - reversePrintFrom(index)               20 pts

 RULES
   - Part 1: no new / delete. Cells come from getNode() and go
     back with freeNode() (both given).
   - Part 2: your solutions MUST be recursive (no loops in R1
     and R2). This is checked by hand.
   - no STL containers (std::vector, std::list, ...)
************************************************************/
#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>
using namespace std;

// ##########################################################
//  PART 1 - ARRAY-BASED LINKED LIST
// ##########################################################
//
//  Same idea as the linked list of Lab 5, but ALL nodes live in
//  ONE array ("cells"), and a link is an INDEX instead of a pointer.
//  Since indexes start at 0, the value -1 means "no cell" - it plays
//  the role of nullptr.
//
//    Lab 5 (node-based)                 Lab 6 (array-based)
//    --------------------------------   -----------------------------------
//    nodeType<Type>* first              int head
//    nullptr  (no next node)            -1       (no next cell)
//    p->info                            cells[i].info
//    p = p->link                        i = cells[i].next
//    new nodeType<Type>                 int i = getNode();   (take a free cell)
//    delete p                           freeNode(i);         (give it back)
//
//  Example: the list 10 -> 20 -> 30 stored in an array of 5 cells.
//  The cells do NOT have to be in order - only the `next` indexes matter.
//
//      index :   0         1         2         3         4
//      cells : [30|-1 ]  [ ? | 4 ]  [10| 3 ]  [20| 0 ]  [ ? |-1 ]
//                                     ^
//      head = 2       ->  2 -> 3 -> 0           (10, 20, 30)
//      freeHead = 1   ->  1 -> 4                (the unused cells: the "free list")
//
template <class Type>
struct cellType {
    Type info;   // the data
    int  next;   // index of the next cell (-1 = end)
};

template <class Type>
class arrayLinkedList {
public:
    arrayLinkedList(int capacity = 100);
    ~arrayLinkedList();
    arrayLinkedList(const arrayLinkedList&) = delete;             // copying is not part of this lab
    arrayLinkedList& operator=(const arrayLinkedList&) = delete;

    // ---------- given ----------
    bool isEmptyList() const;
    bool isFullList() const;
    int  length() const;
    void print() const;          // 10 20 30
    void printCells() const;     // shows the whole array (great for debugging!)
    void reversePrint() const;   // calls your reversePrintFrom(head)

    // ---------- YOUR TASKS ----------
    int  search(const Type& item) const;     // Q1
    void insertFirst(const Type& newItem);   // Q2
    void insertLast(const Type& newItem);    // Q3
    void deleteNode(const Type& item);       // Q4

protected:
    // ---------- given: the array version of new / delete ----------
    int  getNode();              // takes a cell from the free list (-1 if full)
    void freeNode(int index);    // gives a cell back to the free list

    void reversePrintFrom(int index) const;  // R2 (YOUR TASK)

    cellType<Type>* cells;   // the array that holds ALL cells
    int capacity;            // number of cells
    int head;                // index of the first cell of the list (-1 = empty)
    int freeHead;            // index of the first FREE cell        (-1 = full)
    int count;               // number of elements in the list
};

// ==========================================================
// PROVIDED IMPLEMENTATIONS (already working - read them!)
// ==========================================================
template <class Type>
arrayLinkedList<Type>::arrayLinkedList(int cap) {
    capacity = (cap > 0) ? cap : 100;
    cells = new cellType<Type>[capacity]();     // () -> every info starts as 0 / ""
    // at the start EVERY cell is free: 0 -> 1 -> 2 -> ... -> capacity-1 -> -1
    for (int i = 0; i < capacity - 1; i++)
        cells[i].next = i + 1;
    cells[capacity - 1].next = -1;
    freeHead = 0;
    head = -1;
    count = 0;
}

template <class Type>
arrayLinkedList<Type>::~arrayLinkedList() { delete[] cells; }

template <class Type>
bool arrayLinkedList<Type>::isEmptyList() const { return head == -1; }

template <class Type>
bool arrayLinkedList<Type>::isFullList() const { return freeHead == -1; }

template <class Type>
int arrayLinkedList<Type>::length() const { return count; }

// Traversal: compare it with Lab 5's  for (p = first; p != nullptr; p = p->link)
template <class Type>
void arrayLinkedList<Type>::print() const {
    for (int i = head; i != -1; i = cells[i].next)
        cout << cells[i].info << " ";
}

template <class Type>
void arrayLinkedList<Type>::printCells() const {
    cout << "   head = " << head << ", freeHead = " << freeHead << ", count = " << count << endl;
    for (int i = 0; i < capacity; i++)
        cout << "   [" << i << "]  info = " << cells[i].info << "\tnext = " << cells[i].next << endl;
}

// The array version of  new nodeType<Type>:
// take the FIRST cell of the free list. Returns -1 if no cell is free.
template <class Type>
int arrayLinkedList<Type>::getNode() {
    if (freeHead == -1)
        return -1;
    int index = freeHead;
    freeHead = cells[index].next;
    cells[index].next = -1;
    return index;
}

// The array version of  delete p:
// put the cell back at the FRONT of the free list.
template <class Type>
void arrayLinkedList<Type>::freeNode(int index) {
    cells[index].next = freeHead;
    freeHead = index;
}

template <class Type>
void arrayLinkedList<Type>::reversePrint() const {
    reversePrintFrom(head);
}

// ==========================================================
// YOUR WORK STARTS HERE  -  PART 1
// Draw the cells array on paper: index | info | next
// ==========================================================

// ----------------------------------------------------------
// Q1 - search  (15 pts)
// Return the INDEX of the first cell whose info == item,
// or -1 if the item is not in the list.
// Walk the list like print() does:  i = cells[i].next
// (Do NOT just loop over the array 0..capacity-1: free cells
//  may contain old values that are not in the list!)
// ----------------------------------------------------------
template <class Type>
int arrayLinkedList<Type>::search(const Type& item) const {
    // TODO
    throw runtime_error("Q1 search() not implemented yet");
}

// ----------------------------------------------------------
// Q2 - insertFirst  (15 pts)
// Put newItem at the FRONT of the list:
//   1) int i = getNode();      (-1 -> list is full: print an
//                               error to cerr and return)
//   2) store newItem in cells[i]
//   3) link it in front of head, make it the new head
//   4) update count
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::insertFirst(const Type& newItem) {
    // TODO
    throw runtime_error("Q2 insertFirst() not implemented yet");
}

// ----------------------------------------------------------
// Q3 - insertLast  (15 pts)
// Put newItem at the END of the list.
//   - get a cell with getNode() (full -> cerr message, return)
//   - empty list: the new cell becomes the head
//   - otherwise: walk to the LAST cell (the one whose next is
//     -1) and link the new cell after it
//   - update count
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::insertLast(const Type& newItem) {
    // TODO
    throw runtime_error("Q3 insertLast() not implemented yet");
}

// ----------------------------------------------------------
// Q4 - deleteNode  (15 pts)
// Remove the first cell whose info == item:
//   - the item is the head: head moves to the next cell
//   - otherwise: walk with TWO indexes (prev and current) and
//     set  cells[prev].next = cells[current].next
//   - give the removed cell back with freeNode(...)
//   - update count
// Empty list or item not found: print a message to cerr and
// change nothing.
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::deleteNode(const Type& item) {
    // TODO
    throw runtime_error("Q4 deleteNode() not implemented yet");
}


// ##########################################################
//  PART 2 - RECURSION
// ##########################################################
//
//  Every recursive function has two parts:
//    1) BASE CASE    - a problem so small that the answer is known
//    2) GENERAL CASE - solve a SMALLER version of the same problem
//                      (call yourself) and use its answer
//  Each call must move closer to the base case, or it never ends.

// ----------------------------------------------------------
// R1 - isPalindrome  (20 pts)
// Is s[left..right] the same forwards and backwards?
//   isPalindrome("racecar") -> true      isPalindrome("abca") -> false
//   isPalindrome("abba")    -> true      isPalindrome("")     -> true
// Think:
//   base case    : left >= right  (0 or 1 character left) -> true
//   general case : the two OUTER characters must be equal AND
//                  the INNER part s[left+1 .. right-1] must be
//                  a palindrome  (that is the recursive call!)
// No loops - use recursion.
// ----------------------------------------------------------
bool isPalindrome(const string& s, int left, int right) {
    // TODO
    throw runtime_error("R1 isPalindrome() not implemented yet");
}

// given: the starting call
bool isPalindrome(const string& s) {
    return isPalindrome(s, 0, (int)s.length() - 1);
}

// ----------------------------------------------------------
// R2 - reversePrintFrom  (20 pts)
// Print the list that starts at cell `index` BACKWARDS,
// each value followed by one space:
//   list 10 -> 20 -> 30   ->   reversePrint() prints "30 20 10 "
// Think:
//   base case    : index == -1 -> nothing to print
//   general case : FIRST print the rest of the list backwards
//                  (recursive call with cells[index].next),
//                  THEN print cells[index].info
// The work done AFTER the recursive call returns is what makes
// the order reverse. No loops - use recursion.
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::reversePrintFrom(int index) const {
    // TODO
    throw runtime_error("R2 reversePrintFrom() not implemented yet");
}


// ==========================================================
// EXAMPLE MAIN - try your functions here, add your own tests!
// (the autograder compiles this file without this main)
// ==========================================================
#ifndef AUTOGRADING
int main() {
    cout << "=== Part 1: array-based linked list (capacity 5) ===" << endl;
    arrayLinkedList<int> list(5);
    cout << "empty list, every cell is free:" << endl;
    list.printCells();

    try {
        list.insertFirst(20);
        list.insertFirst(10);                       // 10 20
        cout << "\nQ2 insertFirst 20, 10 : ";
        list.print();
        cout << endl;
    } catch (const exception& e) {
        cout << "\n" << e.what() << endl;
    }

    try {
        list.insertLast(30);
        list.insertLast(40);                        // 10 20 30 40
        cout << "Q3 insertLast 30, 40  : ";
        list.print();
        cout << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    try {
        int found = list.search(30);
        int missing = list.search(99);
        cout << "Q1 search(30) = cell " << found
             << ",  search(99) = " << missing << "  (-1 = not found)" << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    try {
        list.deleteNode(20);                        // 10 30 40, the cell of 20 becomes free
        cout << "Q4 deleteNode(20)     : ";
        list.print();
        cout << endl;
        list.insertFirst(5);                        // re-uses the freed cell!
        cout << "insertFirst(5)        : ";
        list.print();
        cout << "  <- look: 5 is stored in the cell that 20 used" << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }
    cout << "the array now:" << endl;
    list.printCells();

    cout << "\n=== Part 2: recursion ===" << endl;
    try {
        for (string w : {"racecar", "abba", "a", "", "abca", "recursion"}) {
            bool result = isPalindrome(w);
            cout << "R1 isPalindrome(\"" << w << "\") = " << (result ? "true" : "false") << endl;
        }
    } catch (const exception& e) {
        cout << e.what() << endl;
    }
    try {
        cout << "R2 print()        : ";
        list.print();
        cout << "\nR2 reversePrint() : ";
        list.reversePrint();
        cout << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    cout << "\n=== DONE ===  (push to GitHub to get your score)" << endl;
    return 0;
}
#endif
