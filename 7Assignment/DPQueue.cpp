// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_FA2019_A7
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }

   // CONSTRUCTORS AND DESTRUCTOR
p_queue::p_queue(size_type initial_capacity) : capacity(initial_capacity),
                                                  used(0)
   {
      // If provided capacity < 1, set to default
      if(initial_capacity < 1){capacity = DEFAULT_CAPACITY;}

      // Allocating new dynamic array with specified capacity
      heap = new ItemType[capacity];
   }

   p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
   {
      // Allocating new dynamic array with src.cpacity
      heap = new ItemType[capacity];

      // Deep copy of items in src
      for(size_type index = 0; index < capacity; ++index)
         heap[index] = src.heap[index];
   }

   // ~DESTRUCTOR
   p_queue::~p_queue()
   {
      delete [] heap;
      heap = 0;
   }

   // MODIFICATION MEMBER FUNCTIONS
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
      // If same (this) object return this
      if (this == &rhs)
         return *this;

      // Temp array to store contents of rhs
      ItemType *temp_heap = new ItemType[rhs.capacity];

      // rsh content transfer
      for (size_type index = 0; index < rhs.used; ++index)
      {
         temp_heap[index] = rhs.heap[index];
      }

      // Deallocating old dynamic array
      delete [] heap;

      // Assigning to new members
      heap = temp_heap;
      capacity = rhs.capacity;
      used = rhs.used;
      return *this;
   }

   void p_queue::push(const value_type& entry, size_type priority)
   {
      // Resizing if at capacity
      if(used == capacity)
      {
         resize(size_type (1.5 * capacity)+1);
      }

      size_type index = used;

      // Pushing new item and updating used
      heap[used].data = entry;
      heap[used].priority = priority;
      ++used;

      // While new item has higher priority than parent, swap
      while(index !=0 && parent_priority(index) < heap[index].priority)
      {
         swap_with_parent(index);
         index = parent_index(index);
      }
   }

   void p_queue::pop()
   {
      // Checking precondition
      assert(size() > 0);

      // If only one item
      if (used == 1)
      {
         --used;
         return;
      }

      // Relocating end data to front
      heap[0].data = heap[used-1].data;

      // Relocating end priority to front
      heap[0].priority = heap[used-1].priority;
      --used;

      // Tracking indeces
      size_type parentIndx = 0,
                childIndx = 0;

      // Swaping smaller parents with larger children
      while (!is_leaf(parentIndx) && heap[parentIndx].priority
            <= big_child_priority(parentIndx))
      {
         childIndx = big_child_index(parentIndx);
         swap_with_parent(big_child_index(parentIndx));
         parentIndx = childIndx;
      }

   }

   // CONSTANT MEMBER FUNCTIONS

   p_queue::size_type p_queue::size() const
   {
      return used;
   }

   p_queue::value_type p_queue::front() const
   {
      // Checking precondition
      assert(size() > 0);

      return heap[0].data;
   }

   bool p_queue::empty() const
   {
      return (used == 0);
   }

   // PRIVATE HELPER FUNCTIONS
   void p_queue::resize(size_type new_capacity)
   // Pre:  (none)
   // Post: The size of the dynamic array pointed to by heap (thus
   //       the capacity of the p_queue) has been resized up or down
   //       to new_capacity, but never less than used (to prevent
   //       loss of existing data).
   //       NOTE: All existing items in the p_queue are preserved and
   //             used remains unchanged.
   {
      // Checking to see if new_capacity less than used
      // if so, then set new_capacity to used
      if(new_capacity < used)
      {
         new_capacity = used;
      }

      //Creating temp heap to store new_capacity heap
      ItemType* temp_heap = new ItemType[new_capacity];

      // Deep copy of items
      for(size_type index = 0; index < used; ++index)
      {
         temp_heap[index] = heap[index];
      }
      // Deallocating memory
      delete [] heap;
      heap = temp_heap;
      capacity = new_capacity;
   }

   bool p_queue::is_leaf(size_type i) const
   // Pre:  (i < used)
   // Post: If the item at heap[i] has no children, true has been
   //       returned, otherwise false has been returned.
   {
      // Checking precondition
      assert(i <used);

      return (((i*2)+1) >= used);
   }

   p_queue::size_type
   p_queue::parent_index(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The index of "the parent of the item at heap[i]" has
   //       been returned.
   {
      // Checking preconditions
      assert(i > 0);
      assert(i < used);

      return static_cast<size_type>((i-1)/2);
   }

   p_queue::size_type
   p_queue::parent_priority(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The priority of "the parent of the item at heap[i]" has
   //       been returned.
   {
      // Checking preconditions
      assert(i > 0);
      assert(i < used);
      return heap[parent_index(i)].priority;
   }

   p_queue::size_type
   p_queue::big_child_index(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The index of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      // checking precondition
      assert(!(is_leaf(i)));

      size_type LHSC_index = (i * 2) + 1; // Index of LHS child
      size_type RHSC_index = (i * 2) + 2; // Index of RHS child

      if(i == 0)
      {
         if(heap[1].priority >= heap[2].priority)
         {
            return 1;
         }
         else
         {
            return 2;
         }
      }
      if (RHSC_index < used && heap[RHSC_index].priority > heap[LHSC_index].priority)
      {
         return RHSC_index;  // Two children present
      }
      else
      {
         return LHSC_index;  // One child present
      }
   }

   p_queue::size_type
   p_queue::big_child_priority(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The priority of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      // Checking precondition
      assert(!(is_leaf(i)));

      return heap[big_child_index(i)].priority;
   }

   void p_queue::swap_with_parent(size_type i)
   // Pre:  (i > 0) && (i < used)
   // Post: The item at heap[i] has been swapped with its parent.
   {
      // Checking preconditions
      assert(i > 0);
      assert(i < used);

      // Finding parent index
      size_type parentIndx = parent_index(i);

      // Copying parent item
      ItemType temp_item = heap[parentIndx];

      // Swaping parent item with child[i] item
      heap[parentIndx] = heap[i];

      // Swaping child item with parent's item
      heap[i] = temp_item;
   }
}
