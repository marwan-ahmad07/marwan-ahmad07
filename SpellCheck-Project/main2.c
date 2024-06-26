#include <stdio.h>
#include <stdlib.h>


typedef struct node node;
struct node {
    int q;
    int b;
    int h;
    node* left;
    node * right;
};

node* newNode(int s){
    node* n = malloc(sizeof(node ));
    n->q = s; n->left = n->right = NULL;
    n->b = n->h = 0;
    return n;
}
node *insert(node *n, node *s);
void leftRotation(node*n)
{
    node* a,*b,*c;
    int t;
    a = n->left;
    b = n->right->right;
    c = n->right->left;
    t = n->q;
    n->q = n->right->q;
    n->right->q = t;
    n->right->right = NULL;
    n->right->left = NULL;
    n->right->h = n->right-> b  = 0;
    n->left = n->right; n->right = NULL;
    insert(n, a);
    insert(n, b);
    insert(n, c);
}
void RightRotation(node*n)
{
    node* a,*b,*c;
    int t;
    a = n->right;
    b = n->left->right;
    c = n->left->left;
    t = n->q;
    n->q = n->left->q;
    n->left->q = t;
    n->left->right = NULL;
    n->left->left = NULL;
    n->left->h = n->left-> b  = 0;
    n->right = n->left; n->left = NULL;
    insert(n, a);
    insert(n, b);
    insert(n, c);
}
node * insert(node *n, node *s) {
    if (!n)
        return NULL;
    if (n->q > s->q) {
        if (!insert(n->left, s)) {
            //node *t = newNode(s);
            n->left = s;
        }
    } else {
        if (!insert(n->right, s)) {
            //node *t = newNode(s);
            n->right = s;
        }
    }
    if(n->right && !n->left) {
        n->h = n->right->h + 1;
        n->b = -n->right->h;
    }
    else if (!n->right && n->left) {
        n->h = n->left->h + 1;
        n->b = n->left->h;
    }
    else {
        n->h = (n->left->h > n->right->h?n->left->h:n->right->h)+1;
        n->b = n->left->h - n->right->h;
    }
    if (n->b>1) {
        if (n->left->b > 0)
            RightRotation(n);
        else {
            leftRotation(n->left);
            RightRotation(n);
        }
    }else if (n->b<-1){
        if (n->left->b < 0)
            leftRotation(n);
        else {
            RightRotation(n->left);
            leftRotation(n);
        }
    }
    return n;
}
node* successor(node* n, int o)
{
    if(!n)
        return NULL;
    if(n->q == o)
    {
        if (n->right) {
            n = n->right;
            while (n->left){
                n = n->left;
            }
            return n;
        }
        return NULL;
    }
    if(n->q>o)
    {
        node *t = successor(n->left, o);
        if (!t)
            return n;
        return t;
    }
    else
        return successor(n->right, o);
}
node* predecessor(node* n, int o)
{
    if(!n)
        return NULL;
    if(n->q == o)
    {
        if (n->left) {
            n = n->left;
            while (n->right){
                n = n->right;
            }
            return n;
        }
        return NULL;
    }
    if(n->q<o)
    {
        node *t = predecessor(n->right, o);
        if (!t)
            return n;
        return t;
    }
    else
        return predecessor(n->left, o);
}
void printTree(node* N)
{
    if(!N)
        return;
    printTree(N->left);
    printf("%d ", N->q);
    printTree(N->right);
}
int main(void){
    int a[] = {7,6,8,4,12,9,2, 11, 50, 20};
    node *n = newNode(a[0]), *t;
    for(int i = 1;i<10;i++)
    {
        t = newNode(a[i]);
        insert(n, t);
    }
    printTree(n);
    printf("\n%d", n->b);
}