#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

// define the type which nodes of tree will store
typedef int element_type;

// the struct of each node of the binary search(node * root, ) tree
typedef struct binary_tree
{
	element_type key;
	struct binary_tree * left;
	struct binary_tree * right;
	int height;
} node;

// get the height of the current node
int get_height(node *input)
{
	// if the input is NULL, the function will return zero to prevent memory errors
	return ((input == NULL) ? 0 : (((node *)(input))->height));
}

node * create_node(element_type key, node * left, node * right)
{
	node * new_node = (node *)malloc(sizeof(node));
	
	// fail to malloc
	if (new_node == NULL)
	{
		printf("fail to create a node\n");
		return NULL;
	}

	new_node->key = key;
	new_node->left = left;
	new_node->right = right;

	return new_node;
}

// get the node with the maxinum key
node * max_node(node * root)
{
	if (root->right != NULL)
	{
		return max_node(root->right);
	} else {
		return root;
	}
}

// get the node with the mininum key
node * min_node(node * root)
{
	if (root->left != NULL)
	{
		return min_node(root->left);
	}
	else 
	{
		return root;
	}
}

// four conditions of rotation
// 1. left left
// 2. right right
// 3. left right
// 4. right left

// left rotation
node * LL_rotation(node * root)
{
	node * left = root->left;
	root->left = left->right;
	left->right = root;

	// update the height
	root->height = MAX(get_height(root->left), get_height(root->right)) + 1;
	left->height = MAX(get_height(left->left), get_height(left->right)) + 1;

	return left;
}

// right rotation
node * RR_rotation(node * root)
{
	node * right = root->right;
	root->right = right->left;
	right->left = root;

	// update the height
	root->height = MAX(get_height(root->left), get_height(root->right)) + 1;
	right->height = MAX(get_height(right->left), get_height(right->right)) + 1;

	return right;
}

// 1. left rotation
// 2. right rotation
node * LR_rotation(node * root)
{
	root->left = RR_rotation(root->left);
	root = LL_rotation(root);

	return root;
}

// 1. right rotation
// 2. left rotation
node * RL_rotation(node * root)
{
	root->right = LL_rotation(root->right);
	root = RR_rotation(root);

	return root;
}

node * insert(node * root, element_type key)
{
	
	if (root == NULL)
	{
		root = create_node(key, NULL, NULL);
	}
	else if (key > root->key)
	{
		root->right = insert(root->right, key);
		
		if (get_height(root->right) - get_height(root->left) == 2)
		{
			if (key > root->right->key)
			{
				root = RR_rotation(root);
			}
			else
			{
				root = RL_rotation(root);
			}
		}
	} 
	else if (key < root->key)
	{
		root->left = insert(root->left, key);

		if (get_height(root->left) - get_height(root->right) == 2)
		{
			if (key < root->left->key)
			{
				root = LL_rotation(root);
			}
			else
			{
				root = LR_rotation(root);
			}
		}
	} 
	else if (key == root->key) 
	{
		printf("Invaild Insert\n");
	}

	root->height = MAX(get_height(root->left), get_height(root->right)) + 1;

	return root;
}

node * search(node * root, element_type key)
{
	if (root == NULL || root->key == key)
	{
		return root;
	}
	else if (key < root->key)
	{
		search(root->left, key);
	}
	else if (key > root->key)
	{
		search(root->right, key);
	}
	
	return NULL;
}

node * delete_node(node * root, element_type key)
{
	node * deleted_node = search(root, key);

	if (root == NULL || deleted_node == NULL)
	{
		return root;
	}

	if (key < root->key)
	{
		root->left = delete_node(root->left, key);

		if (get_height(root->right) - get_height(root->left) == 2)
		{
			if (key < root->right->key)
			{
				root = RL_rotation(root);
			}
			else
			{
				root = RR_rotation(root);
			}
		}
	}
	else if (key > root->key)
	{
		root->right = delete_node(root->right, key);

		if (get_height(root->left) - get_height(root->right) == 2)
		{
			if (key < root->left->key)
			{
				root = LL_rotation(root);
			}
			else
			{
				root = LR_rotation(root);
			}
		}
	}
	else
	{
		if (root->left && root->right)
		{
			node * mininum = min_node(root->right);
			root->key = mininum->key;
			root->right = delete_node(root->right, mininum->key);
		}
		else
		{
		root = root->left ? root->left : root->right;
		}

	}
	

	if (root)
	{
		root->height = MAX(get_height(root->left), get_height(root->right)) + 1;
	}

	return root;
}

// Preorder Traversal(DLR)
void preorder(node * root, int isWriting, FILE * fp)
{
	// start recursion
	if (root != NULL)
	{
		// opeartion
		int flag = 0;
		char flag_c = 0;
		if (root->left != NULL)
		{
			flag += 2;
			flag_c += 2;
		}
		if (root->right != NULL)
		{
			flag += 1;
			flag_c += 1;
		}
		printf("%d %d %c\n", root->key, flag, flag_c);

		if (isWriting == 1)
		{
			// fprintf(fp, "%d%c", root->key, flag_c);
			fwrite(&root->key, sizeof(int), 1, fp);
			fwrite(&flag_c, sizeof(char), 1, fp);
		}

		preorder(root->left, isWriting, fp);
		preorder(root->right, isWriting, fp);

		if (isWriting == 2)
		{
			free(root);
		}
	}
}

void inorder(node * root, element_type * arr, int * index)
{
	if (root != NULL)
	{
		inorder(root->left, arr, index);

		arr[*index] = root->key;
		*index = *index + 1;

		inorder(root->right, arr, index);
	}
}

int print_to_file(node * root, const char * filename)
{
	FILE * fp = fopen(filename, "wb");

	if (fp == NULL)
	{
		printf("Fail to write the binary tree.\n");
		return 0;
	}

	preorder(root, 1, fp);

	fclose(fp);

	return 1;
}


node * read_from_file(const char * filename)
{
	FILE * fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("The filename is Invaild!\n");
		return NULL;
	}

	int operation_num;

	fseek(fp, 0, SEEK_END);
	long binary_length = ftell(fp);
	operation_num = binary_length / (sizeof(int) + sizeof(char));
	fseek(fp, 0, SEEK_SET);
	
	node * root = NULL;

	for (int i = 0; i < operation_num; i++)
	{
		int input_key[1];
		char opeartion[1];

		if(!fread(input_key, sizeof(int), 1, fp))
		{
			printf("reading error\n");
		}
		if(!fread(opeartion, sizeof(char), 1, fp))
		{
			printf("reading error\n");
		}
		
		// printf("%d %c\n", input_key[0], opeartion[0]);

		if (opeartion[0] == 'i')
		{
			root = insert(root, input_key[0]);
		}
		else if (opeartion[0] == 'd')
		{
			root = delete_node(root, input_key[0]);
		}
	}

	fclose(fp);

	return root;
}

node * build_tree(node * root, element_type * key, char * flag, int * index)
{
	element_type key_ = key[*index];
	char flag_ = flag[*index];

	*index = *index + 1;
	node * new_node = create_node(key_, NULL, NULL);

	if (flag_ == 1)
	{
		new_node->right = build_tree(new_node->right, key, flag, index);
	}
	if (flag_ == 2)
	{
		new_node->left = build_tree(new_node->left, key, flag, index);
	}
	if (flag_ == 3)
	{
		// printf("Test3\n");
		new_node->left = build_tree(new_node->left, key, flag, index);
		new_node->right = build_tree(new_node->right, key, flag, index);
	}

	if (new_node)
	{
		new_node->height = MAX(get_height(new_node->left), get_height(new_node->right)) + 1;
	}

	return new_node;
}

node * build_tree_from_file(const char * filename, int * size_)
{
	FILE * fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("-1\n");
		exit(EXIT_FAILURE);
	}

	int operation_num;

	// get the number of nodes
	fseek(fp, 0, SEEK_END);
	long binary_length = ftell(fp);
	operation_num = binary_length / (sizeof(int) + sizeof(char));
	fseek(fp, 0, SEEK_SET);
	*size_ = operation_num;

	int * keys = malloc(operation_num * sizeof(int));
	char * leaves = malloc(operation_num * sizeof(char));

	int * keys_pointer = keys;
	char * leaves_pointoer = leaves;

	printf("%d\n", operation_num);

	for (int i = 0; i < operation_num; ++i)
	{
		if(!fread(keys_pointer, sizeof(int), 1, fp))
		{
			printf("reading error\n");
		}
		if(!fread(leaves_pointoer, sizeof(char), 1, fp))
		{
			printf("reading error\n");
		}

		printf("%d %c\n", keys[i], leaves[i]);

		keys_pointer += 1;
		leaves_pointoer += 1;
	}

	keys_pointer = keys;
	leaves_pointoer = leaves;
	int i = 0;

	node * root = build_tree(NULL, keys_pointer, leaves_pointoer, &i);

	fclose(fp);

	return root;
}

int isBST(node * root, int size)
{
	int i = 0;
	element_type * array = malloc(size * sizeof(element_type));

	inorder(root, array, &i);

	for (int j = 0; j < size - 1; ++j)
	{
		if (array[j] > array[j+1])
		{
			return 0;
		}
	}

	return 1;
}

int isAVL(node * root)
{
	int factor = get_height(root->left) - get_height(root->right);

	if (factor <= -2 || factor >= 2)
	{
		return 0;
	}

	return 1;
}

int main(int argc, char const *argv[])
{
	// The programme cannot read the operation normally
	if (strcmp(argv[1], "-b") != 0 && strcmp(argv[1], "-e") != 0)
	{
		printf("Please input opeartion: -b or -e\n");
		return 0;
	}

	if (strcmp(argv[1], "-b") == 0)
	{
		if (!(argv[2] && argv[3]))
		{
			printf("Please input filename: proc (input) (output)\n");
			return 0;
		}
		node * tree = read_from_file(argv[2]);
		
		printf("Tree:\n");
		print_to_file(tree, argv[3]);
		printf("\n");
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		int size = 0;
		int is_bst;
		int is_avl;

		node * tree = build_tree_from_file(argv[2], &size);
		preorder(tree, 0, NULL);

		is_bst = isBST(tree, size);
		is_avl = isAVL(tree);

		printf("1,%d,%d\n", is_bst, is_avl);
	}

	return 0;
}