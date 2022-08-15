
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RED     0
#define BLACK   1

typedef int KET_TYPE;

int key_compare(KET_TYPE *a, KET_TYPE *b)
{
    return 0;
}

typedef struct _rbtree_node
{
    unsigned char color;
    struct _rbtree_node *left;
    struct _rbtree_node *right;
    struct _rbtree_node *parent;

    KET_TYPE key;       
    void *value;

} rbtree_node;

typedef struct _rbtree
{
    rbtree_node *root;
    rbtree_node *nil;

} rbtree;


void _left_rotate(rbtree *T, rbtree_node *x)
{
    rbtree_node *y = x->right;

    x->right = y->left;
    if (y->left != T->nil)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == T->nil)  // 如果x为根节点
    {
        T->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;

}

void _right_rotate(rbtree *T, rbtree_node *y)
{
    rbtree_node *x = y->left;

    y->left = x->right;
    if (x->right != T->nil)
    {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == T->nil)  // 如果x为根节点
    {
        T->root = x;
    }
    else if (y == y->parent->right)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;

}

#pragma region 插入

void rbtree_insert_fixup(rbtree *T, rbtree_node *z)
{
    while (z->parent->color == RED) // 父节点为红色，黑色不用调整
    {
        if (z->parent == z->parent->parent->left)  // 父节点为祖父节点的左节点
        {
            rbtree_node *y = z->parent->parent->right;

            if (y->color == RED) // 叔父节点为红色
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;  // 往上递归
            }
            else    // 叔父节点为黑色
            {              
                if (z == z->parent->right)  // 为父节点的右节点
                {
                    z = z->parent;
                    _left_rotate(T, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                _right_rotate(T, z->parent->parent);
            }          
        }
        else    // 父节点为祖父节点的右节点
        {
            rbtree_node *y = z->parent->parent->left;

            if (y->color == RED) // 叔父节点为红色
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;  // 往上递归
            }
            else    // 叔父节点为黑色
            {              
                if (z == z->parent->left)  // 为父节点的左节点
                {
                    z = z->parent;
                    _right_rotate(T, z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                // 变色后左子树的黑高降低，左旋平衡黑高
                _left_rotate(T, z->parent->parent);     
            }          
        }     
    }
    T->root->color = BLACK;
}

void rbtree_insert(rbtree *T, rbtree_node *z)
{
    rbtree_node *x = T->root;
    rbtree_node *y = T->nil;

    while (x != T->nil)
    {
        y = x;      // 父节点
        if (z->key < x->key)
        {
            x = x->left;
        }
        else if (z->key > x->key)
        {
            x = x->right;
        }
        else
        {
            return;
        }
        
    }

    z->parent = y;
    if (y == T->nil) // 空树
    {
        T->root = z;
    }
    else if (y->key > z->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    
    z->left = T->nil;
    z->right = T->nil;

    z->color = RED; // 设为黑色，影响黑高，调整麻烦
    rbtree_insert_fixup(T, z);
    
}

#pragma endregion