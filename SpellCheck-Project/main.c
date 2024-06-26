#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Node Node;
struct Node{
    int h;
    char* word;
    Node* left;
    Node* right;
};
Node* newNode(char* s){
    Node* n = malloc(sizeof(Node ));
    char* st = malloc(strlen(s)+1);
    strcpy(st, s);
    n->word = st; n->left = n->right = NULL;
    n->h = 0;
    return n;
}
Node* insert(Node *n, char *s){
    if(!n)
        return NULL;
    if (strcasecmp(n->word, s) > 0)
    {
        if(!insert(n->left,s))
        {
            Node *ne= newNode(s);
            n->left = ne;
        }
    }
    else {
        if(!insert(n->right, s))
        {
            Node *ne= newNode(s);
            n->right = ne;
        }
    }
    if(n->right && !n->left) {
        n->h = n->right->h + 1;
    }
    else if (!n->right && n->left) {
        n->h = n->left->h + 1;
    }
    else {
        n->h = (n->left->h > n->right->h?n->left->h:n->right->h)+1;
    }
    return n;
}
void printTree(Node* N)
{
    if(!N)
        return;
    printTree(N->left);
    printf("%s\n", N->word);
    printTree(N->right);
}
Node* search(Node* n, char*s){
    if (!n)
        return NULL;
    if (!strcasecmp(n->word, s))
        return n;
    if (strcasecmp(n->word, s)>0)
    {
        if(!n->left)
            return n;
        return search(n->left, s);
    }
    else
    {
        if(!n->right)
            return n;
        return search(n->right, s);
    }
}
Node* successor(Node* n, char*s)
{
    if(!n)
        return NULL;
    if(!strcasecmp(n->word, s))
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
    if(strcasecmp(n->word, s)>0)
    {
        Node *t = successor(n->left, s);
        if (!t)
            return n;
        return t;
    }
    else
        return successor(n->right, s);
}
Node* predecessor(Node* n, char*s)
{
    if(!n)
        return NULL;
    if(!strcasecmp(n->word, s))
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
    if(strcasecmp(n->word, s)<0)
    {
        Node *t = predecessor(n->right, s);
        if (!t)
            return n;
        return t;
    }
    else
        return predecessor(n->left, s);
}
Node* Load(char *File)
{
    int i;
    char s[101];
    FILE *f = fopen(File, "r");
    if (!f)
    {
        printf("File not found\n");
        exit(1);
    }
    fgets(s, 100, f);
    s[strlen(s)-1] = '\0';
    Node *n = newNode(s);
    for(i = 1;fgets(s, 100, f); i++){
        s[strlen(s)-1] = '\0';
        insert(n, s);
    }
    fclose(f);
    printf("Dictionary Loaded Successfully..!\n");
    printf(".................................\n");
    printf("Size = %d\n", i);
    printf(".................................\n");
    printf("Height = %d\n", n->h);
    printf(".................................\n");
    return n;
}
void destTree(Node*n)
{
    if (!n)
        return;
    destTree(n->left);
    destTree(n->right);
    free(n->word);
    free(n);
}
int main() {
    char s[100];
    Node *n = Load("Dictionary.txt"), *f, *p, *S;
    printf("Enter a sentence: \n");
    fgets(s, 100, stdin);
    s[strlen(s)-1] = '\0';
    char*t = strtok(s, " ");
    do{
        f = search(n, t);
        if(!strcasecmp(t, f->word))
        {
            printf("%s - CORRECT\n", t);
            continue;
        }
        printf("%s - INCORRECT, Suggestion: %s", t, f->word);
        if(!(p = predecessor(n, f->word))) {
            S = successor(n, f->word);
            printf(" %s %s\n", S->word, successor(n,S->word)->word);
            continue;
        }else printf(" %s", p->word);
        if (!(S = successor(n, f->word)))
        {
            S = predecessor(n, p->word);
            printf(" %s\n", S->word);
        }else printf(" %s\n" , S->word);
    } while ((t = strtok(NULL, " ")));
    destTree(n);
    return 0;
}
