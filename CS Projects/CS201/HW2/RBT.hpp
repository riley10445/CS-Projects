#ifndef _RBT_HPP_
#define _RBT_HPP_

#include <algorithm>
#include "RBTNode.hpp"

template <class Key, class Type>
class RedBlackTree {
protected:
   RBTNode<Key, Type>* root;

   void DeleteTree(RBTNode<Key, Type>* treeRoot) {
      if (treeRoot)
      {
          DeleteTree(treeRoot->left);
          DeleteTree(treeRoot->right);
          delete treeRoot;
      }
   }
   
   int GetHeightRecursive(RBTNode<Key, Type>* node) {
      if (node == nullptr) {
         return -1;
      }
      int leftHeight = GetHeightRecursive(node->left);
      int rightHeight = GetHeightRecursive(node->right);
      return 1 + std::max(leftHeight, rightHeight);
   }
   
   void BSTRemove(Key key) {
      RBTNode<Key, Type>* node = Search(key);
      BSTRemoveNode(node);
   }

   void BSTRemoveNode(RBTNode<Key, Type>* node) {
      if (node == nullptr) {
         return;
      }

      // Case 1: Internal node with 2 children
      if (node->left && node->right) {
         // Find successor
         RBTNode<Key, Type>* successorNode = node->right;
         while (successorNode->left) {
            successorNode = successorNode->left;
         }

         // Copy successor's key and value
         Key successorKey = successorNode->key;
         Type successorValue = successorNode->value;

         // Recursively remove successor
         BSTRemoveNode(successorNode);

         // Set node's key and value to copied successor key and value
         node->key = successorKey;
         node->value = successorValue;
      }
      
      // Case 2: Root node (with 1 or 0 children)
      else if (node == root) {
         if (node->left == nullptr) {
            root = node->right;
         }
         else {
            root = node->left;
         }
        
         // Make sure the new root, if non-null, has parent set to null
         if (root) {
            root->parent = nullptr;
         }
         
         delete node;
      }
      
      // Case 3: Internal with left child only
      else if (node->left) {
         node->parent->ReplaceChild(node, node->left);
         delete node;
      }

      // Case 4: Internal with right child OR leaf
      else {
         node->parent->ReplaceChild(node, node->right);
         delete node;
      }
   }
   
   void PrepareForRemoval(RBTNode<Key, Type>* node) {
      if (TryCase1(node)) {
         return;
      }

      RBTNode<Key, Type>* sibling = node->GetSibling();
      if (TryCase2(node, sibling)) {
         sibling = node->GetSibling();
      }
      if (TryCase3(node, sibling)) {
         return;
      }
      if (TryCase4(node, sibling)) {
         return;
      }
      if (TryCase5(node, sibling)) {
         sibling = node->GetSibling();
      }
      if (TryCase6(node, sibling)) {
         sibling = node->GetSibling();
      }

      sibling->color = node->parent->color;
      node->parent->color = RBTNode<Key, Type>::BLACKCOLOR;
      if (node == node->parent->left) {
         sibling->right->color = RBTNode<Key, Type>::BLACKCOLOR;
         RotateLeft(node->parent);
      }
      else {
         sibling->left->color = RBTNode<Key, Type>::BLACKCOLOR;
         RotateRight(node->parent);
      }
   }
   
   bool TryCase1(RBTNode<Key, Type>* node) {
      if (node->IsRed() || node->parent == nullptr) {
         return true;
      }
      return false; // node case 1
   }
   
   bool TryCase2(RBTNode<Key, Type>* node, RBTNode<Key, Type>* sibling) {
      if (sibling->IsRed()) {
         node->parent->color = RBTNode<Key, Type>::REDCOLOR;
         sibling->color = RBTNode<Key, Type>::BLACKCOLOR;
         if (node == node->parent->left) {
            RotateLeft(node->parent);
         }
         else {
            RotateRight(node->parent);
         }
         return true;
      }
      return false; // not case 2
   }
   
   bool TryCase3(RBTNode<Key, Type>* node, RBTNode<Key, Type>* sibling) {
      if (node->parent->IsBlack() && sibling->AreBothChildrenBlack()) {
         sibling->color = RBTNode<Key, Type>::REDCOLOR;
         PrepareForRemoval(node->parent);
         return true;
      }
      return false; // not case 3
   }
   
   bool TryCase4(RBTNode<Key, Type>* node, RBTNode<Key, Type>* sibling) {
      if (node->parent->IsRed() && sibling->AreBothChildrenBlack()) {
         node->parent->color = RBTNode<Key, Type>::BLACKCOLOR;
         sibling->color = RBTNode<Key, Type>::REDCOLOR;
         return true;
      }
      return false; // not case 4
   }
   
   bool TryCase5(RBTNode<Key, Type>* node, RBTNode<Key, Type>* sibling) {
      if (IsNotNoneAndRed(sibling->left)) {
         if (IsNoneOrBlack(sibling->right)) {
            if (node == node->parent->left) {
               sibling->color = RBTNode<Key, Type>::REDCOLOR;
               sibling->left->color = RBTNode<Key, Type>::BLACKCOLOR;
               RotateRight(sibling);
               return true;
            }
         }
      }
      return false; // not case 5
   }
   
   bool TryCase6(RBTNode<Key, Type>* node, RBTNode<Key, Type>* sibling) {
      if (IsNoneOrBlack(sibling->left)) {
         if (IsNotNoneAndRed(sibling->right)) {
            if (node == node->parent->right) {
               sibling->color = RBTNode<Key, Type>::REDCOLOR;
               sibling->right->color = RBTNode<Key, Type>::BLACKCOLOR;
               RotateLeft(sibling);
               return true;
            }
         }
      }
      return false; // not case 6
   }

public:
   RedBlackTree() {
      root = nullptr;
   }

   virtual ~RedBlackTree() {
      DeleteTree(root);
   }
   
   RBTNode<Key, Type>* GetRoot() {
      return root;
   }
   
   int GetLength() {
      if (root == nullptr) {
         return 0;
      }
      return root->Count();
   }
   
   // Returns the height of this tree
   int GetHeight() {
      return GetHeightRecursive(root);
   }
   
   RBTNode<Key, Type>* Insert(Key key, Type value) {
      RBTNode<Key, Type>* newNode = new RBTNode<Key, Type>(key, value, nullptr, true, nullptr, nullptr);
      InsertNode(newNode);
      return newNode;
   }

   void InsertNode(RBTNode<Key, Type>* node) {
      // Begin with normal BST insertion
      if (root == nullptr) {
         // Special case for root
         root = node;
      }
      else {
         RBTNode<Key, Type>* currentNode = root;
         while (currentNode) {
            if (node->key < currentNode->key) {
               if (currentNode->left == nullptr) {
                  currentNode->SetChild(RBTNode<Key, Type>::LEFTCHILD, node);
                  break;
               }
               else {
                  currentNode = currentNode->left;
               }
            }
            else {
               if (currentNode->right == nullptr) {
                  currentNode->SetChild(RBTNode<Key, Type>::RIGHTCHILD, node);
                  break;
               }
               else {
                  currentNode = currentNode->right;
               }
            }
         }
      }
      
      // Color the node red, then balance
      node->color = RBTNode<Key, Type>::REDCOLOR;
      InsertionBalance(node);
   }
   
   void InsertionBalance(RBTNode<Key, Type>* node) {
      // If node is the tree's root, then color node black and return
      if (node->parent == nullptr) {
         node->color = RBTNode<Key, Type>::BLACKCOLOR;
         return;
      }
        
      // If parent is black, then return without any changes
      if (node->parent->IsBlack()) {
         return;
      }
    
      // Pointers to parent, grandparent, and uncle are needed for remaining operations
      RBTNode<Key, Type>* parent = node->parent;
      RBTNode<Key, Type>* grandparent = node->GetGrandparent();
      RBTNode<Key, Type>* uncle = node->GetUncle();
        
      // If parent and uncle are both red, then color parent and uncle black, 
      // color grandparent red, recursively balance  grandparent, then return
      if (uncle && uncle->IsRed()) {
         parent->color = uncle->color = RBTNode<Key, Type>::BLACKCOLOR;
         grandparent->color = RBTNode<Key, Type>::REDCOLOR;
         InsertionBalance(grandparent);
         return;
      }

      // If node is parent's right child and parent is grandparent's left child, then rotate left
      // at parent, update node and parent to point to parent and grandparent, respectively
      if (node == parent->right && parent == grandparent->left) {
         RotateLeft(parent);
         node = parent;
         parent = node->parent;
      }
      // Else if node is parent's left child and parent is grandparent's right 
      // child, then rotate right at parent, update node and parent to point to 
      // parent and grandparent, respectively
      else if (node == parent->left && parent == grandparent->right) {
         RotateRight(parent);
         node = parent;
         parent = node->parent;
      }

      // Color parent black and grandparent red
      parent->color = RBTNode<Key, Type>::BLACKCOLOR;
      grandparent->color = RBTNode<Key, Type>::REDCOLOR;
                
      // If node is parent's left child, then rotate right at grandparent, 
      // otherwise rotate left at grandparent
      if (node == parent->left) {
         RotateRight(grandparent);
      }
      else {
         RotateLeft(grandparent);
      }
   }
   
   // Performs a left rotation at the given node. Returns the
   // subtree's new root.
   RBTNode<Key, Type>* RotateLeft(RBTNode<Key, Type>* node) {
      // Define a convenience pointer to the right child of the 
      // left child.
      RBTNode<Key, Type>* rightLeftChild = node->right->left;
        
      // Step 1 - the right child moves up to the node's position.
      // node is temporarily detached from the tree, but will be reattached
      // later.
      if (node->parent) {
         node->parent->ReplaceChild(node, node->right);
      }
      else { // node is root
         root = node->right;
         root->parent = nullptr;
      }

      // Step 2 - the node becomes the left child of what used
      // to be node's right child, but is now node's parent. This will
      // detach rightLeftChild from the tree.
      node->right->SetChild(RBTNode<Key, Type>::LEFTCHILD, node);
        
      // Step 3 - reattach rightLeftChild as the right child of node.
      node->SetChild(RBTNode<Key, Type>::RIGHTCHILD, rightLeftChild);
        
      return node->parent;
   }
   
   // Performs a right rotation at the given node. Returns the
   // subtree's new root.
   RBTNode<Key, Type>* RotateRight(RBTNode<Key, Type>* node) {
      // Define a convenience pointer to the left child of the 
      // right child.
      RBTNode<Key, Type>* leftRightChild = node->left->right;
        
      // Step 1 - the left child moves up to the node's position.
      // node is temporarily detached from the tree, but will be reattached
      // later.
      if (node->parent) {
         node->parent->ReplaceChild(node, node->left);
      }
      else { // node is root
         root = node->left;
         root->parent = nullptr;
      }

      // Step 2 - the node becomes the right child of what used
      // to be node's left child, but is now node's parent. This will
      // detach leftRightChild from the tree.
      node->left->SetChild(RBTNode<Key, Type>::RIGHTCHILD, node);
        
      // Step 3 - reattach leftRightChild as the right child of node.
      node->SetChild(RBTNode<Key, Type>::LEFTCHILD, leftRightChild);
        
      return node->parent;
   }
   
   bool IsNoneOrBlack(RBTNode<Key, Type>* node) {
      if (node == nullptr) {
         return true;
      }
      return node->IsBlack();
   }
   
   bool IsNotNoneAndRed(RBTNode<Key, Type>* node) {
      if (node == nullptr) {
         return false;
      }
      return node->IsRed();
   }
   
   bool Remove(Key key) {
      RBTNode<Key, Type>* node = Search(key);
      if (node) {
         RemoveNode(node);
         return true;
      }
      return false;
   }

   void RemoveNode(RBTNode<Key, Type>* node) {
      if (node->left && node->right) {
         RBTNode<Key, Type>* predecessorNode = node->GetPredecessor();
         Key predecessorKey = predecessorNode->key;
         Type predecessorValue = predecessorNode->value;
         RemoveNode(predecessorNode);
         node->key = predecessorKey;
         node->value = predecessorValue;
         return;
      }

      if (node->IsBlack()) {
         PrepareForRemoval(node);
      }
      BSTRemove(node->key);

      // One special case if the root was changed to red
      if (root && root->IsRed()) {
         root->color = RBTNode<Key, Type>::BLACKCOLOR;
      }
   }
   
   // Searches for a node with a matching key. Does a regular
   // binary search tree search operation. Returns the node with the
   // matching key if it exists in the tree, or None if there is no
   // matching key in the tree.
   RBTNode<Key, Type>* Search(Key desiredKey) {
      RBTNode<Key, Type>* currentNode = root;
      while (currentNode) {
         // Return the node if the key matches
         if (currentNode->key == desiredKey) {
            return currentNode;
         }
         
         // Navigate to the left if the search key is 
         // less than the node's key.
         else if (desiredKey < currentNode->key) {
            currentNode = currentNode->left;
         }
         
         // Navigate to the right if the search key is 
         // greater than the node's key.
         else {
            currentNode = currentNode->right;
         }
      }
      
      // The key was not found in the tree.
      return nullptr;
   }
};

#endif

