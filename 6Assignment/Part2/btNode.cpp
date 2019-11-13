#include "btNode.h"

void dumpToArrayInOrder(btNode* bst_root, int* dumpArray)
{
   if (bst_root == 0) return;
   int dumpIndex = 0;
   dumpToArrayInOrderAux(bst_root, dumpArray, dumpIndex);
}

void dumpToArrayInOrderAux(btNode* bst_root, int* dumpArray, int& dumpIndex)
{
   if (bst_root == 0) return;
   dumpToArrayInOrderAux(bst_root->left, dumpArray, dumpIndex);
   dumpArray[dumpIndex++] = bst_root->data;
   dumpToArrayInOrderAux(bst_root->right, dumpArray, dumpIndex);
}

void tree_clear(btNode*& root)
{
   if (root == 0) return;
   tree_clear(root->left);
   tree_clear(root->right);
   delete root;
   root = 0;
}

int bst_size(btNode* bst_root)
{
   if (bst_root == 0) return 0;
   return 1 + bst_size(bst_root->left) + bst_size(bst_root->right);
}

// write definition for bst_insert here
void bst_insert(btNode*& bst_root, int insInt){

   if(bst_root == 0) // If Tree is empty, add insInt to root
   {
      btNode* temp_root = new btNode;
      temp_root->data = insInt;
      temp_root->left = temp_root->right = 0;
      bst_root = temp_root;
      return;
   }

   btNode* cursor = bst_root; // Creating cursor for traversing tree

   while (cursor != 0) // If tree not empty
   {
      if (cursor->data > insInt) // If insInt smaller, insert left
      {
         if(cursor->left == 0) // If left empty, insert insInt
         {
            cursor->left = new btNode;
            cursor->left->data = insInt;
            cursor->left->left = cursor->left->right = 0;
            return;
         }
         else // Continue to next left node
         {
            cursor = cursor->left;
         }
      }
      else if(cursor->data < insInt) // If insInt smaller, insert left
      {
         if(cursor->right == 0) // If left empty, insert insInt
         {
            cursor->right = new btNode;
            cursor->right->data = insInt;
            cursor->right->left = cursor->right->right = 0;
            return;
         }
         else // Continue to next right node
         {
            cursor = cursor->right;
         }
      }
      else // Current data matches insert data, replacing anyway
      {
         cursor->data = insInt;
         return;
      }
   }

}

// write definition for bst_remove here
bool bst_remove(btNode*& bst_root, int remInt)
{
   if(bst_root == 0) // If tree is empty, return false
   {
      return false;
   }

   if(bst_root->data > remInt) // Target to remove is less than current
   {
      bst_remove(bst_root->left, remInt);

   }
   else if(bst_root->data < remInt) // Target to remove is greater than current
   {
      bst_remove(bst_root->right, remInt);
   }
   else                             // Target has been found
   {
      if(bst_root->left != 0 && bst_root->right != 0) // Current root has both
      {                                               // left and right children
         bst_remove_max(bst_root->left, bst_root->data);
      }
      else                          // Current root has only one child or none
      {
         btNode* old_bst_root = bst_root;
         if(bst_root->left == 0 && bst_root->right != 0) // Right child present
         {
            bst_root = bst_root->right;
         }
         else if (bst_root-> left != 0 && bst_root->right == 0) // Left present
         {
            bst_root = bst_root->left;
         }
         else                       // No children
         {
            bst_root = 0;
         }
         delete old_bst_root;       // Free old root
      }
      return true;
   }
}

// write definition for bst_remove_max here
void bst_remove_max(btNode*& bst_root, int& data)
{
   if(bst_root == 0) // If current root is empty, return
   {
      return;
   }

   if(bst_root->right == 0) // If no right child, root is largest item
   {
      btNode* removeNode = bst_root;
      data = bst_root->data;
      bst_root = bst_root->left;
      delete removeNode;
   }
   else                 // Right child found, root not largest item
   {
      bst_remove_max(bst_root->right, data);
   }
}
