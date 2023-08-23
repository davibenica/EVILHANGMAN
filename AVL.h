#include <stdio.h>
#include <stdlib.h>



#include "generic.h"
#include "generic_vector.h"
#include "my_string.h"
#include "status.h"

typedef void* TREE;

TREE tree_init_default(void);
Status tree_push(TREE hTree, MY_STRING key, MY_STRING word);
void tree_destroy(TREE* phTree);
GENERIC_VECTOR get_largest_family(TREE hTree);
 
