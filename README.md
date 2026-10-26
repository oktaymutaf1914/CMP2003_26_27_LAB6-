# 🚀 Lab Assignment 6 — Array-Based Linked List + Recursion

Welcome to **Lab 6**! This lab has two parts:

| Part | Topic | Time | Points |
|---|---|---|---|
| 1 | **Array-based linked list**: the Lab 5 linked list, but stored inside one array | ~40 min | 60 |
| 2 | **Recursion**: two classic examples | ~30 min | 40 |

Everything happens in **one file**, `MainProgram.cpp`. Do the questions **in order**. The other files are part of the grading system.
⚠️ **Do not edit any other files** — your grade depends on this!

---

## 📂 Project Structure

```text
.
├── README.md          # This file
├── Makefile           # Builds & runs your program (don't touch)
└── MainProgram.cpp    # ← ✍️ YOU WRITE THIS FILE
```

---

## 🧱 Part 1 — Array-Based Linked List (~40 min)

### The idea

It is **still a linked list**, exactly like Lab 5. The difference: **all nodes live in one array**, and a link is an **index** instead of a pointer.

| Lab 5 (node-based) | Lab 6 (array-based) |
|---|---|
| `nodeType<Type>* first` | `int head` |
| `nullptr` (no next node) | `-1` (no next cell: indexes start at 0, so -1 is never a real cell) |
| `p->info` | `cells[i].info` |
| `p = p->link` | `i = cells[i].next` |
| `new nodeType<Type>` | `int i = getNode();` (take a free cell) |
| `delete p` | `freeNode(i);` (give the cell back) |

Example: the list `10 → 20 → 30` in an array of 5 cells. **The cells don't have to be in order: only the `next` indexes matter.**

```text
 index :    0          1          2          3          4
 cells : [30 | -1]  [ ? | 4 ]  [10 | 3 ]  [20 | 0 ]  [ ? | -1]

 head     = 2   →  2 → 3 → 0          the list:      10, 20, 30
 freeHead = 1   →  1 → 4              the free list: the unused cells
```

Every cell is in **exactly one** of two lists: the **list** (starting at `head`) or the **free list** (starting at `freeHead`). `getNode()` and `freeNode()` (given) move cells between them. That is how the array version replaces `new` and `delete`.

> 💡 Call `list.printCells()` in your `main()` to see the whole array: `index | info | next`, plus `head` and `freeHead`. Draw it on paper too!

### Q1 — `int search(const Type& item) const` · 15 pts
Return the **index of the cell** that holds `item`, or `-1` if it's not in the list. Walk the list like the given `print()` does: `for (int i = head; i != -1; i = cells[i].next)`.

> ⚠️ Don't loop over the whole array `0 .. capacity-1`: free cells can still hold old values that are **not** in the list.

### Q2 — `void insertFirst(const Type& newItem)` · 15 pts
1. `int i = getNode();` If it's `-1`, the list is full: print a message to `cerr` and return.
2. Store `newItem` in `cells[i]`.
3. Link it in front of `head` and make it the new head.
4. Update `count`.

### Q3 — `void insertLast(const Type& newItem)` · 15 pts
Get a cell with `getNode()` (full → `cerr` message). An empty list → the new cell becomes the head. Otherwise **walk to the last cell** (the one whose `next` is `-1`) and link the new cell after it. Update `count`.

### Q4 — `void deleteNode(const Type& item)` · 15 pts
* The item is in the head → `head` moves to the next cell.
* Otherwise walk with **two indexes**, `prev` and `current`, and set `cells[prev].next = cells[current].next`.
* Give the removed cell back with **`freeNode(...)`**. That's the array version of `delete`; a cell you forget is lost forever.
* Update `count`. Empty list or item not found → `cerr` message, no change.

---

## 🔁 Part 2 — Recursion (~30 min)

Every recursive function has two parts:

1. **Base case**: a problem so small that the answer is known immediately.
2. **General case**: solve a **smaller** version of the same problem (call yourself) and use its answer.

Each call must get closer to the base case, or it never ends.

### R1 — `bool isPalindrome(const string& s, int left, int right)` · 20 pts
Is `s[left..right]` the same forwards and backwards? (`isPalindrome(s)`, which starts the recursion with `0` and `length-1`, is given.)

```text
isPalindrome("racecar", 0, 6)
   'r' == 'r'  →  isPalindrome("racecar", 1, 5)
                     'a' == 'a'  →  isPalindrome("racecar", 2, 4)
                                       'c' == 'c'  →  isPalindrome("racecar", 3, 3)
                                                         left >= right  →  true   (base case)
```

* **Base case:** `left >= right` (0 or 1 character left) → `true`.
* **General case:** the two **outer** characters must be equal, **and** the **inner** part `s[left+1 .. right-1]` must be a palindrome. That second check is the recursive call.

### R2 — `void reversePrintFrom(int index) const` · 20 pts
Print the list starting at cell `index` **backwards**, each value followed by one space. `reversePrint()` (given) calls `reversePrintFrom(head)`.

```text
list 10 → 20 → 30      reversePrint() prints:  30 20 10
```

* **Base case:** `index == -1` → nothing to print.
* **General case:** **first** print the rest of the list backwards (call yourself with `cells[index].next`), **then** print `cells[index].info`.

> 💡 The work done **after** the recursive call returns is what reverses the order. Swap those two lines and the list prints forwards: try it!

🚫 **R1 and R2 must be recursive: no `for` / `while` loops.** The autograder checks that each function calls itself and has no loops.

---

## ▶️ Run Your Program

```bash
make run      # builds and runs YOUR main()
make clean    # removes build files
```

(No `make`? `g++ -Wall --std=c++17 MainProgram.cpp -o main && ./main`)

`main()` tries every question and prints the cells array, so you can watch the free list at work. Unfinished questions print `... not implemented yet`. **Add your own tests to `main()`**: the autograder never runs it.

A finished solution prints (the cell tables are shortened here):

```text
Q2 insertFirst 20, 10 : 10 20
Q3 insertLast 30, 40  : 10 20 30 40
Q1 search(30) = cell 2,  search(99) = -1  (-1 = not found)
Q4 deleteNode(20)     : 10 30 40
insertFirst(5)        : 5 10 30 40   <- look: 5 is stored in the cell that 20 used
...
R1 isPalindrome("racecar") = true
R1 isPalindrome("abca") = false
R2 print()        : 5 10 30 40
R2 reversePrint() : 40 30 10 5
```

---

## 🧪 How It Will Be Tested

* The tests are **hidden**: they run on GitHub every time you **push**. Classroom 50 builds your `MainProgram.cpp` with the instructor's tests and runs **22 checks**. Test your code yourself with `main()` before you push!
* The tests build the lists **directly**, without your insert functions, with the cells deliberately **out of order**. So a bug in one question never costs points in another.
* Every Part 1 check also verifies that **each cell is in exactly one list** (the list or the free list) and that `count` is right.
* A crash or an infinite loop only fails the check where it happens.
* Your score appears under **Releases** in your repository a minute after you push.

### 💯 Grading breakdown

| Q | Function | Checks | Points |
|---|----------|:------:|:------:|
| Q1 | `search` | 3 | 15 |
| Q2 | `insertFirst` | 3 | 15 |
| Q3 | `insertLast` | 3 | 15 |
| Q4 | `deleteNode` | 5 | 15 |
| R1 | `isPalindrome` (recursive) | 4 | 20 |
| R2 | `reversePrintFrom` (recursive) | 4 | 20 |
| | **Total** | **22** | **100** |

---

## ⚠️ Rules & Grading

* ✅ **Only edit `MainProgram.cpp`**: the `TODO` functions, plus `main()` for your own experiments.
* ❌ **Do not modify**: `Makefile`, `.github/`, `.classroom50.yaml`, or rename `MainProgram.cpp`.
* 🚫 Part 1: no `new` / `delete`; use `getNode()` / `freeNode()`. Part 2: no loops.
* 🚫 No `std::vector`, `std::list` or other STL containers in your solution.
* 🧹 Your code must compile with **no warnings** (`-Wall`) using **C++17**.

---

<p align="center">
  <sub>
    🎓 <strong>Bahçeşehir University</strong> — CMP2003 Data Structures &amp; Algorithms (C++) · 2026–2027<br>
    Prepared by <strong>Ismail Taha Samed ÖZKAN</strong>
  </sub>
</p>
