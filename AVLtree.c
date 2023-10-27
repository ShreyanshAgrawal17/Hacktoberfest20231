#include <stdio.h>
#include <stdlib.h>

typedef struct tree
{
    int data;
    struct tree *left;
    int height;
    struct tree *right;
} tree;

int height(tree *root)
{
    int hl, hr;
    tree *p = root;

    if (p && p->left)
    {
        hl = p->left->height;
    }
    else
    {
        hl = 0;
    }

    if (p && p->right)
    {
        hr = p->right->height;
    }
    else
    {
        hr = 0;
    }
    if (hl > hr)
    {
        return hl + 1;
    }
    else
    {
        return hr + 1;
    }
}

int balancefactor(tree *root)
{
    int hl, hr;
    tree *p = root;
    if (p && p->left)
    {
        hl = p->left->height;
    }
    else
    {
        hl = 0;
    }

    if (p && p->right)
    {
        hr = p->right->height;
    }
    else
    {
        hr = 0;
    }
    return hl - hr;
}

tree *check(tree *root)
{
    if (balancefactor(root) == 1 || balancefactor(root) == 0 || balancefactor(root) == -1)
    {
        printf("Tree is AVL tree\n");
    }
    else
    {
        printf("Not AVL tree\n");
    }
}

tree *LLRotation(tree *root)
{
    tree *p = root;
    tree *pl = p->left;
    tree *plr = pl->right;
    pl->right = p;
    p->left = plr;
    p->height = height(p);
    pl->height = height(pl);
    if (root == p)
    {
        root = pl;
    }
    return pl;
}

tree *LRRotation(tree *root)
{
    tree *p = root;
    tree *pl = p->left;
    tree *plr = pl->right;
    plr->left = pl;
    plr->right = p;
    pl->right = plr->left;
    p->left = plr->right;
    plr->height = height(plr);
    p->height = height(p);
    if (root == p)
    {
        root = plr;
    }
    return plr;
}

tree *RRRotation(tree *root)
{
    tree *p = root;
    tree *pr = p->right;
    tree *prl = pr->left;
    pr->left = p;
    p->right = prl;
    p->height = height(p);
    pr->height = height(pr);
    if (root == p)
    {
        root = pr;
    }
    return pr;
}

tree *RLRotation(tree *root)
{
    tree *p = root;
    tree *pr = p->right;
    tree *prl = pr->left;
    prl->right = pr;
    prl->left = p;
    p->right = prl->left;
    pr->left = prl->right;
    prl->height = height(prl);
    p->height = height(p);

    if (root == p)
    {
        root = prl;
    }
    return prl;
}

tree *insert(tree *root, int value)
{
    tree *p = root;
    tree *t = NULL;
    if (root == NULL)
    {
        t = malloc(sizeof(tree));
        t->data = value;
        t->height = 0;
        t->left = NULL;
        t->right = NULL;
        return t;
    }
    else
    {
        if (value < root->data)
        {
            root->left = insert(root->left, value);
        }
        else if (value > root->data)
        {
            root->right = insert(root->right, value);
        }
    }
    p->height = height(p);
    if (balancefactor(p) == 2 && balancefactor(p->left) == 1)
    {
        return LLRotation(p);
    }
    if (balancefactor(p) == 2 && balancefactor(p->right) == -1)
    {
        return LRRotation(p);
    }
    if (balancefactor(p) == -2 && balancefactor(p->left) == 1)
    {
        return RRRotation(p);
    }
    if (balancefactor(p) == -2 && balancefactor(p->right) == -1)
    {
        return RLRotation(p);
    }
    return p;
}

tree *search(tree *root, int key)
{
    if (root == NULL)
    {
        return 0;
    }
    else if (key < root->data)
    {
        return search(root->left, key);
    }
    else if (key > root->data)
    {
        return search(root->right, key);
    }
    else if (key == root->data)
    {
        return root;
    }
}

tree *inorder_successor(tree *root)
{
    tree *y;
    if (root == NULL)
        return NULL;

    if (root->right == NULL)
        return root;
    y = root->right; // inorder successor is leftmost son of right subtree of x
    while (y->left != NULL)
        y = y->left;
    return y;
}

tree *delete(tree *root, int key)
{
    tree *insucc;

    if (root == NULL)
    {
        return NULL;
    }
    else if (key < root->data)
    {
        root->left = delete (root->left, key); // node to be deleted is in left

        if (balancefactor(root) == -2) //  Need to Rebalance in right side
            if (balancefactor(root->right) <= 0)
                root = RRRotation(root);
            else
                root = RLRotation(root);
    }
    else if (key > root->data) // Node to be deleted is in right
    {
        root->right = delete (root->right, key);

        if (balancefactor(root) == 2) // Need to rebalance in left side
            if (balancefactor(root->left) >= 0)
                root = LLRotation(root);
            else
                root = LRRotation(root);
    }
    else
    { // key to be deleted is found

        if (root->right != NULL) // that means the node to be deleted has two children
        {
            // find its inorder succesor
            insucc = inorder_successor(root);
            root->data = insucc->data; // exchange key of inorder successor
            root->right = delete (root->right, insucc->data);
            if (balancefactor(root) == 2)           // Need to rebalance in left side
                if (balancefactor(root->left) >= 0) // to check for left to left case
                    root = LLRotation(root);
                else
                    root = LRRotation(root);
        }
        else                   // when the root has only left child
            return root->left; // root->Left  become new root
    }
    root->height = height(root);
    return root; // return the modified root after deletion
}

void inorder(tree *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main()
{
    tree *root = NULL;
    int value;
    int choice;
    int key;
    while (1)
    {
        printf("******MAIN MENU******\n");
        printf("1.Insert\n");
        printf("2.Display\n");
        printf("3.Delete\n");
        printf("4.Search key\n");
        printf("5.Given tree is AVL or not\n");
        printf("6.Exit\n");
        printf("Enter you choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            while (value != -1)
            {
                printf("Enter positive integer(or -1): ");
                scanf("%d", &value);
                if (value != -1)
                    root = insert(root, value);
            }
            break;
        case 2:
            inorder(root);
            printf("\n");
            if (root == NULL)
                printf("BST is empty");
            break;
        case 3:
            printf("Enter key to delete: ");
            scanf("%d", &key);
            root = delete (root, key);
            printf("\n");
            break;
        case 4:
            printf("Enter key: ");
            scanf("%d", &key);
            if (search(root, key))
            {
                printf("Key exists");
            }
            else
            {
                printf("Key not found");
            }
            printf("\n");
            break;
        case 5:
            check(root);
            break;
        case 6:
            exit(1);
            break;
        }
    }
    return 0;
}
