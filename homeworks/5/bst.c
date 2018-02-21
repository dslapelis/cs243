#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bst.h"

///create a new node and allocate it in memory
TreeNode* new_node( int num )
{
    TreeNode* node = NULL;
    //printf("Allocating mem...\n");
    node = malloc(sizeof(TreeNode));
    //printf("Setting data...\n");
    node->data = num;
    //printf("Setting left...\n");
    node->left = NULL;
    //printf("Setting right...\n");
    node->right = NULL;
    //printf("Returning...\n");
    return node;
}

///insert new nodes into tree to build it
void insert( int current_val, TreeNode* root )
{
   TreeNode* current = root;

   while( 1 )
   {
       /// if the current value is less than the node's value
       /// check to see if the left node is NULL. if NULL, create a new node
       /// otherwise, set new node to left node and rerun function.
       if( current_val < current->data )
       {
           if( current->left )
           {
               current = current->left;
           }//end if
	   else
	   {
		current->left = new_node(current_val);
		return;
	   }//end else
	}//end if
        /// of current val is greater than the nodes value
        /// check to see if the right node is NULL. if NULL, create a new node
        /// otherwise, set new node to the right node and rerun function
        else if( current_val > current->data )
        {
            if( current->right )
            {
                current = current->right;
            }//end if
            else
            {
                 current->right = new_node(current_val);
                 return;
            }//end else
        }//end elseif
        else
        {
            return;
        }//end else
    }// endwhiile
} // endif

void preorder_trans( const TreeNode* root )
{
    //preorder transversal algorithm
    if( root != NULL )
    {
        printf( "%d\n", root->data );
        preorder_trans( root->left );
        preorder_trans( root->right );
    }
}

void postorder_trans( const TreeNode* root )
{
    /// postorder transversal algorithm
    if( root != NULL )
    {
        postorder_trans( root->left );
        postorder_trans( root->right );
        printf( "%d\n", root->data );
    }
}

void inorder_trans( const TreeNode* root )
{
    ///inorder transversal algorithm
    if( root != NULL )
    {
        inorder_trans( root->left );
        printf("%d\n", root->data );
        inorder_trans( root->right );
    }
}

///---------- IMPLEMENTED FUNCTIONS ---------\\\
/// build_tree() 
///     Dynamically build BST by allocating nodes on the heap
///
/// args -
///        root - a pointer to the pointer to the root of the tree
///               to build this tree on to.
///        elements - array of integers to add to the tree
///        count - the number of integers in the array
void build_tree(TreeNode** root, const int elements[], const int count)
{
    //printf("%d\n", count);
    *root = new_node( elements[0] );

    //printf("Beginning insert loop.\n");
    for( int x = 1; x < count; x++ )
    {
	insert(elements[x], *root);
    }
}

/// traverse()
///    Recursively traverses the tree and prints the value of each
///    node.
///
/// args -
///        root - a pointer to the root of the tree to traverse
///        type - the traversal type
void traverse(const TreeNode* root, const TraversalType type)
{
    if( type == 0 )
    {
        preorder_trans( root );
    }
   
    if( type == 1 )
    {
        inorder_trans( root );
    }
    
    if( type == 2 )
    {
        postorder_trans( root );
    }
}

/// cleanup_tree()
///    Cleanup all memory management associated with the nodes on the heap
///
/// args
///      root - the current root of the tree
void cleanup_tree(TreeNode* root)
{
    if( root != NULL )
    {
       cleanup_tree( root->left );
       cleanup_tree( root->right );
       free(root);
    }
}

int main( int argc, char **argv )
{

    int size = (int)(argv[1][0] - '0');
    printf("Enter %d integer values to place in tree:\n", size);
    int elements[size-1];

    for( int x = 0; x < size; x++ )
    {
        scanf("%d\n", &elements[x]);
    }


    ///print vals
    printf("Input values:\n");
    for( int x = 0; x < size; x++ )
    {
       printf("%d\n", elements[x]);
    }

    TreeNode **root;
    root = malloc(sizeof(TreeNode));

    //build tree
    build_tree( root, elements, size);


    printf("Preorder:\n");
    traverse( *root, 0 );

    printf("\nInorder:\n");
    traverse( *root, 1 );

    printf("\nPostorder:\n");
    traverse( *root, 2 );
    
    cleanup_tree( *root );

   printf("\n");
   return 0;
}
