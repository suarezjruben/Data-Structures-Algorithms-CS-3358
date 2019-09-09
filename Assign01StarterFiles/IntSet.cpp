// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     compile-time array whose size is IntSet::MAX_SIZE;
//     the member variable data references the array.
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
//     from data[used] through data[IntSet::MAX_SIZE - 1].
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

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

IntSet::IntSet()
{
   used = 0;
}

int IntSet::size() const
{
   // 'used' holds the total amount of distinct values, 
   // should be same as size.
   return used; 
}

bool IntSet::isEmpty() const
{
   bool empty;
   // if 'used'/'size' is greater than 0 
   // then the intSet is not empty, else is empty.
   if (used > 0)empty = false;
   else if (used == 0)empty = true;
   return empty; 
}

bool IntSet::contains(int anInt) const
{
   bool found = false;
   // Check that IntSet is not empty then check for 
   // anInt in the IntSet, return true if present, 
   // else return false.
   if (used > 0)
   {  
      for (int i=0; i < used; i++)
      {
         if(data[i] == anInt)found = true;
      }
   }
   return found; 
}

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
   cout << "In isSubsetOf" << endl;
   bool subset = true;
   int matches = 0;
   //Check size of 'this' IntSet first, if 0 then subset is true.
   if (used != 0)
   {
      for(int i=0; i<=otherIntSet.size(); i++)
      {
         for(int j=0; j<=used; j++)
         {
            if(otherIntSet.contains(data[j])) matches++;
         }
      }
   }
   if(matches != used)subset = false;
   return subset; 
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
   int shared = 0;
   IntSet unionSet = *this;
   //Identify # of shared values
   for (int i=0; i<=otherIntSet.size(); i++)
   {
      for (int j=0; j<=used; j++)
      {
         if (otherIntSet.contains(data[j])) shared++;
      }
   }
   //Verify that size of combineation does not go above MAX_SIZE
   if (((used + otherIntSet.size()) - shared) <= MAX_SIZE)
   {
      for (int i = 0; i < otherIntSet.size(); i++)
      {
         if (!unionSet.contains(otherIntSet.data[i]))
         {
            unionSet.add(otherIntSet.data[i]);
         }
      }
   }
   return unionSet; 
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
   // Creating retrun IntSet
   IntSet intersectSet = *this;

   // Removing every item that is not present
   // in otherIntSet
   for (int i = 0; i < size(); i++)
   {
      if (!otherIntSet.contains(data[i]))
      {
         intersectSet.remove(data[i]);
      }
   }
   
   return intersectSet;
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
   IntSet subtractedSet = *this;
   // Removing every item that is present 
   // in otherIntSet
   for (int i = 0; i < size(); i++)
   {
      if (otherIntSet.contains(data[i]))
      {
         subtractedSet.remove(data[i]);
      }
   }
   
   return subtractedSet;
}

void IntSet::reset()
{
   // Reseting 'used' to 0
   used = 0;
}

bool IntSet::add(int anInt)
{
   bool added = true;
   // Check that the IntSet is not full
   if (used < MAX_SIZE)
   {
      // Check contains() for int value
      // if contains() returns true, do 
      // NOT add new value and return false
      if(contains(anInt)) added = false;

      // if contains() returns false, 
      // add new value and return true
      else if(!contains(anInt))
      {
         data[used] = anInt;
         used++;
      }
   }
   else added = false;
   cout << "used: " << used << endl;
   return added; 
}

bool IntSet::remove(int anInt)
{
   bool erased = true;
   int flag = 0;
   //Checking contains(anInt), if return is true
   if (contains(anInt))
   {
      for (int i=0; i < used; i++)
      {
         if(data[i] == anInt)
         {
            flag = i;
         }
      }
      while (flag < MAX_SIZE) 
      {
         data[flag] = data[flag+1];
         flag++;
      } 
      used--;
   }
   else erased = false;
   return erased;
}

bool equal(const IntSet& is1, const IntSet& is2)
{
   cout << "Start of equal" << endl;
   bool equal = false;
   if (is1.isSubsetOf(is2) )//&& is2.isSubsetOf(is1))
   {
      cout << "if statement" << endl;
      equal = true;
   }
   cout << "End of equal" << endl;
   return equal;
}


