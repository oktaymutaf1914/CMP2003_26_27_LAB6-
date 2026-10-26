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
     Q1 - search(item)                           15 pts
     Q2 - insertFirst(item)                      15 pts
     Q3 - insertLast(item)                       15 pts
     Q4 - deleteNode(item)                       15 pts
   PART 2 - RECURSION                          (~30 min, 40 pts)
     R1 - isPalindrome(s, left, right)           20 pts
     R2 - reversePrintFrom(index)                20 pts

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
// ==========================================================

// ----------------------------------------------------------
// Q1 - search  (15 pts)
// ----------------------------------------------------------
template <class Type>
int arrayLinkedList<Type>::search(const Type& item) const {
    for (int i = head; i != -1; i = cells[i].next) {
        if (cells[i].info == item) {
            return i;
        }
    }
    return -1;
}

// ----------------------------------------------------------
// Q2 - insertFirst  (15 pts)
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::insertFirst(const Type& newItem) {
    int i = getNode();
    if (i == -1) {
        cerr << "Cannot insert in a full list." << endl;
        return;
    }
    cells[i].info = newItem;
    cells[i].next = head;
    head = i;
    count++;
}

// ----------------------------------------------------------
// Q3 - insertLast  (15 pts)
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::insertLast(const Type& newItem) {
    int i = getNode();
    if (i == -1) {
        cerr << "Cannot insert in a full list." << endl;
        return;
    }
    cells[i].info = newItem;
    cells[i].next = -1;

    if (head == -1) {
        head = i;
    } else {
        int current = head;
        while (cells[current].next != -1) {
            current = cells[current].next;
        }
        cells[current].next = i;
    }
    count++;
}

// ----------------------------------------------------------
// Q4 - deleteNode  (15 pts)
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::deleteNode(const Type& item) {
    if (head == -1) {
        cerr << "Cannot delete from an empty list." << endl;
        return;
    }

    if (cells[head].info == item) {
        int temp = head;
        head = cells[head].next;
        freeNode(temp);
        count--;
        return;
    }

    int prev = head;
    int current = cells[head].next;

    while (current != -1 && cells[current].info != item) {
        prev = current;
        current = cells[current].next;
    }

    if (current == -1) {
        cerr << "The item to be deleted is not in the list." << endl;
    } else {
        cells[prev].next = cells[current].next;
        freeNode(current);
        count--;
    }
}


// ##########################################################
//  PART 2 - RECURSION
// ##########################################################

// ----------------------------------------------------------
// R1 - isPalindrome  (20 pts)
// ----------------------------------------------------------
bool isPalindrome(const string& s, int left, int right) {
    if (left >= right) {
        return true;
    }
    if (s[left] != s[right]) {
        return false;
    }
    return isPalindrome(s, left + 1, right - 1);
}

// given: the starting call
bool isPalindrome(const string& s) {
    return isPalindrome(s, 0, (int)s.length() - 1);
}

// ----------------------------------------------------------
// R2 - reversePrintFrom  (20 pts)
// ----------------------------------------------------------
template <class Type>
void arrayLinkedList<Type>::reversePrintFrom(int index) const {
    if (index == -1) {
        return;
    }
    reversePrintFrom(cells[index].next);
    cout << cells[index].info << " ";
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