 
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "AVL.h"
 
 typedef struct node Node;
 
 struct node
 {
	Node* right;
	Node* left;
	MY_STRING key;
	GENERIC_VECTOR words;
	int height;
	
 };
 
 struct tree
 {
	Node* root;
};

typedef struct tree Tree;

// Helper funtions
void destroy_nodes(Node* root);
Node* get_largest_node(Node* root, Node* previous_max);
int get_height(Node* root);
int get_balance(Node* root);
Node* rotate_right(Node* y);
Node* rotate_left(Node* x);
Node* insert_new_node(Node* root, MY_STRING key, MY_STRING word);
int get_balance_factor(Node* root);
int max(int a, int b);

int max(int a, int b)
{
	return (a > b) ? a : b;
}


TREE tree_init_default(void)
{
	Tree* pTree = (Tree*) malloc(sizeof(Tree));
	
	if (pTree != NULL)
	{
		pTree->root = NULL;
	}
	return pTree;
}

Status tree_push(TREE hTree, MY_STRING key, MY_STRING word)
{
	Tree* pTree = (Tree*) hTree;
	
	pTree->root = insert_new_node(pTree->root, key, word);

	return SUCCESS;
}

GENERIC_VECTOR get_largest_family(TREE hTree)
{
	Tree* pTree = (Tree*) hTree;
	Node* largest_node = get_largest_node(pTree->root, pTree->root);
	
	return largest_node->words;
	
}

Node* get_largest_node(Node* root, Node* previous_max)
{
	if (root != NULL)
	{
	
	if (previous_max == NULL || generic_vector_get_size(root->words) > generic_vector_get_size (previous_max->words))
	{
		previous_max = root;
	}
	
	Node* max_left = get_largest_node(root->left, previous_max);
	Node* max_right = get_largest_node(root->right, previous_max);

	if (generic_vector_get_size(max_left->words) > generic_vector_get_size(previous_max->words))
	{
    	previous_max = max_left;
	}

	if (generic_vector_get_size(max_right->words) > generic_vector_get_size(previous_max->words))
	{
    	previous_max = max_right;
	}
	
	}
	return previous_max;
	
}

void tree_destroy(TREE* phTree)
{
	Tree* pTree = (Tree*) *phTree;
	if (pTree->root != NULL)
	{
		destroy_nodes(pTree->root);
	}
	free(pTree);
	*phTree = NULL;

}

void destroy_nodes(Node* root)
{
	
	// Base case
	if (root == NULL)
	{
		return;	
	}
	
	destroy_nodes(root->left);
	destroy_nodes(root->right);
	
	my_string_destroy(&(root->key));
	generic_vector_destroy(&(root->words));
	free(root);
	
}

int get_height(Node* root)
{
	if (root == NULL)
	{
		return 0;
	}
	return root->height;
}

int get_balance_factor(Node* root)
{
	if (root == NULL)
	{
		return 0;
	}

	return get_height(root->right) - get_height(root->left);
}

Node* rotate_right(Node* y)
{
	// On right rotation, the left child of y becomes the new root
	Node* x = y->left;

	// The right child of x becomes the left child of y
	Node* temp = x->right;
	
	// Perform rotation
	x->right = y;
	y->left = temp;
	
	// Update heights
	y->height = max(get_height(y->left), get_height(y->right)) + 1;
	x->height = max(get_height(x->left), get_height(x->right)) + 1;
	
	return x;
}

Node* rotate_left(Node* x)
{
	// On left rotation, the right child of x becomes the new root
	Node* y = x->right;

	// The left child of y becomes the right child of x
	Node* temp = y->left;
	
	// Perform rotation
	y->left = x;
	x->right = temp;
	
	// Update heights
	x->height = max(get_height(x->left), get_height(x->right)) + 1;
	y->height = max(get_height(y->left), get_height(y->right)) + 1;
	
	return y;
}

Node* insert_new_node(Node* root, MY_STRING key, MY_STRING word)
{
	if (root == NULL)
	{
		root = (Node*) malloc(sizeof(Node));
		if (root == NULL)
		{
			return NULL;	
		}
		
		root->left = NULL;
		root->right = NULL;
		
		root->words = generic_vector_init_default(my_string_init_copy, my_string_destroy);
		
		root->key = my_string_init_copy(key);
		
		generic_vector_push_back(root->words, word);
		
		root->height = 1;
		return root;
	}

	// Traverse the tree to place the new node
	int result = my_string_compare(root->key, key);

	// greater than
	if (result > 0)
	{
		root->right = insert_new_node(root->right, key, word);
	}

	// less than
	else if (result < 0)
	{
		root->left = insert_new_node(root->left, key, word);
	}
	// We are at the correct
	else
	{
		generic_vector_push_back(root->words, word);
	}

	// Update the height of the ancestor node
	root->height = 1 + max(get_height(root->left), get_height(root->right));

	// Peform AVL self balancing

	// Get the balance factor of the node to check if it is unbalanced
	int balance = get_balance_factor(root);

	// Right heavy
	if (balance > 1)
	{
		// Check for double rotation
		if (get_balance_factor(root->right) < 0)
		{
			root->right = rotate_right(root->right);
			return rotate_left(root);
		}
		else
		{
			return rotate_left(root);
		}
		
	}

	// Left heavy
	if (balance < -1)
	{
		// Check for double rotation
		if (get_balance_factor(root->left) > 0)
		{
			root->left = rotate_left(root->left);
			return rotate_right(root);
		}
		else
		{
			return rotate_right(root);
		}

	}
	
	return root;

}