// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

void IntSet::resize(int new_capacity)
{
   // Validating new capacity value
   if (used != 0 && new_capacity < used )
   {
      capacity = used;
   }
   else if (new_capacity <= 0)
   {
      capacity = DEFAULT_CAPACITY;
   }
   else
   {
      capacity = new_capacity;
   }

   // Creating temp dynamic array with new capacity value.
   int * temp_array = new int[capacity];

   // Transferring old elements to new dynamic array.
   for (int i = 0; i < used; i++)
   {
      temp_array[i] = data[i];
   }

   // Deallocating old array.
   delete [] data;
   // Assigning data to new array.
   data = temp_array;
   // Removing temp_array pointer because it is no longer needed
   temp_array = NULL;
   delete temp_array;
}

IntSet::IntSet(int initial_capacity) : capacity(initial_capacity), used(0)
{
   // Initializing capacity to DEFAULT_CAPACITY if initial_capacity < 1
   if (initial_capacity < 1)
   {
      capacity = DEFAULT_CAPACITY;
   }
   // Assingning 'data' to a new intance of a dynamic array of size 'capacity'
   data = new int[capacity];
}

IntSet::IntSet(const IntSet& src) : capacity(src.capacity), used(src.used)
{
   // Assingning 'data' to a new intance of a dynamic array of size 'capacity'
   data = new int[capacity];

   // Copying every element in src to data
   for (int i = 0; i < src.used; i++)
   {
      data[i] = src.data[i];
   }
}


IntSet::~IntSet()
{
   // Deallocating dynamic variables
   delete [] data;
   data = NULL;
}

IntSet& IntSet::operator=(const IntSet& rhs)
{
   // Allocating space in temp array to hold elements in rhs
   int * temp_array = new int[rhs.capacity];
   for (int i = 0; i < rhs.used; i++)
   {
      temp_array[i] = rhs.data[i];
   }

   // Deleting current dynamic array pinted to by data
   // and assigning data to temp_array
   delete [] data;
   data = temp_array;

   // Copying over all properties from rhs to data
   capacity = rhs.capacity;
   used = rhs.used;

   // Delingt temp_array pointer because is no longer needed
   temp_array = NULL;
   delete temp_array;

   return *this;
}

int IntSet::size() const
{
   // Returning # of distinct int values the invoking IntSet currently contains
   // which is stored in the member variable used.
   return used;
}

bool IntSet::isEmpty() const
{
   // Returning true if used is equal to 0.
   return (used == 0);
}

bool IntSet::contains(int anInt) const
{
   if (used > 0)
   {
      for (int i = 0; i < used; i++)
      {
         // if (data[i] == anInt) // Differnt version
         if (*(data + i) == anInt)
         {
            return true;
         }
      }
   }
   return false;
}

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
   // An empty set is a subset of any set.
   if (isEmpty())
   {
      return true;
   }
   // Check for all elements of invoking set, if any one element is not
   // contained return false.
   else
   {
      for (int i = 0; i < used; i++)
      {
         if (!(otherIntSet.contains(data[i])))
         {
            return false;
         }
      }
   }
   return true;
}

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}

IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
   // Instanciating IntSet unionSet = *this to hold union elements of both
   // this and otherIntSet
   IntSet unionSet = *this;

   // Copying over unique elements from ohterIntSet since all
   // elements from *this are already contained
   for (int i = 0; i < otherIntSet.used; i++)
   {
      unionSet.add(otherIntSet.data[i]);
   }

   return unionSet;
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
   // IntSet representing the intersection of the invoking IntSet
   // and otherIntSet that will be returned
   IntSet interSet = *this;

   // Removing all elements not contained in otherIntSet
   // from the interSet
   for (int i = 0; i < used; i++)
   {
      if (!otherIntSet.contains(interSet.data[i]))
      {
         interSet.remove(data[i]);
      }
   }

   return interSet;
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
   // IntSet representing the difference between the
   // invoking IntSet and otherIntSet
   IntSet subtractSet = *this;

   // Removing all elemts in otherIntSet that
   // are contained in subtractSet
   for (int i = 0; i < otherIntSet.used; i++)
   {
      // Calling remove() right away because the remove function
      // does the contain() check already
      subtractSet.remove(otherIntSet.data[i]);
   }

   return subtractSet;
}

void IntSet::reset()
{
   delete [] data;
   data = new int[DEFAULT_CAPACITY];
   used = 0;
   capacity = DEFAULT_CAPACITY;
}

bool IntSet::add(int anInt)
{
   // Validating new value, if not new then false is returned
   if (!contains(anInt))
   {
      // Validating that array is not full,
      // if so then calling the resize function
      if (used >= capacity)
      {
         // Resizing to atleast capacity + 1.
         resize(int(1.5 * capacity) + 1);
      }

      // Adding new element
      data[used] = anInt;
      used++;

      return true;
   }
   else
   {
      return false;
   }
}

bool IntSet::remove(int anInt)
{
   IntSet temp_intSet;
   if (contains(anInt))
   {
      for (int i = 0; i < used; i++)
      {
         if (!(data[i] == anInt))
         {
            temp_intSet.add(data[i]);
         }
      }
      delete [] data;
      data = temp_intSet.data;
      used = temp_intSet.used;
      capacity = temp_intSet.capacity;
      return true;
   }

   return false;
}

bool operator==(const IntSet& is1, const IntSet& is2)
{
   if (is1.isEmpty() && is2.isEmpty())
   {
      return true;
   }
   else if (is1.isSubsetOf(is2) && is2.isSubsetOf(is1))
   {
      return true;
   }

   return false;
}
