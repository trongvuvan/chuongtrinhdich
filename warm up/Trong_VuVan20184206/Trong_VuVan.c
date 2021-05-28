#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isName(char* word)
{
	int i;
	for (i = 0; i < strlen(word); i++)
		if (word[0] >= 'A' && word[0] <= 'Z') return 1;
	return 0;
}

int isStopword(const char*filename,char* word)
{
	FILE *fptr;
	fptr = fopen(filename,"r");
	char* key = (char*)malloc(255*sizeof(char));
	while (fscanf(fptr,"%s",key)!= EOF)
	{
		if(strcmp(word,key)==0)
		{
			fclose(fptr);
			return 1;
		}
	}
	fclose(fptr);
	return 0;
}

struct node {
    char* word;
	int repeatNumber;
	int line[100];
    struct node *left, *right;
};
struct node* newNode(char* item)
{
    struct node* temp = (struct node*)malloc(5000*sizeof(struct node));
    temp->word = (char*)malloc(255*sizeof(char));
    strcpy(temp->word, item);
	temp->repeatNumber = 1;
	temp->line[0] = 1;
    temp->left = temp->right = NULL;
    return temp;
}
struct node* insert(struct node* node, char *key)
{
    if (node == NULL)
        return newNode(key);
    else{
		if (strcmp(node->word,key) > 0 )
        	{
				node->left = insert(node->left, key);
			}
    	else if (strcmp(node->word,key) < 0 )
        	{
				node->right = insert(node->right, key);
			}
	}
	return node;
}
void inorder(struct node* root)
{
	int i;
    if (root != NULL) {
        inorder(root->left);
        printf(" %s: %d :", root->word,root->repeatNumber);
		for (i = 0; i < root->repeatNumber; i++)
			printf(" %d",root->line[i]);
		printf("\n");
        inorder(root->right);
    }
}

struct node* searching(struct node* node,char* key, int line)
{
	if (node != NULL)
	{
		if(strcmp(node->word,key)==0)
		{
			node->line[node->repeatNumber] = line;
			node->repeatNumber++;
			return node;
		}
		if(strcmp(node->word,key) > 0)
		{
			searching(node->left,key, line);
		}
		if(strcmp(node->word,key) < 0)
		{
			searching(node->right,key, line);
		}
	}
	return NULL;
}

struct node* readcontentfromfile(const char *filename,struct node* node)
{
	int count = 1;
	char *strcontent;
	char ch;

	strcontent = (char*)malloc(255*sizeof(char));
	FILE *fp;
	fp = fopen(filename,"r");
	while (fscanf(fp,"%s%c",strcontent,&ch) != EOF)
	{
		char *string = (char*)malloc(255*sizeof(char));
		strcpy(string,strcontent);
		char *tmp = (char*)malloc(255*sizeof(char));
		tmp = strtok(string," ,.\t\n\0)(-");
		if(isStopword("stopword.txt", tmp)==0 && isName(tmp) == 0)
		{
			searching(node,tmp,count);
			node = insert(node,tmp);
		}
		if(ch == '\n') count++;
	}
	
	fclose(fp);
	return node;
}

int main()
{
    struct node* root = NULL;
    root = readcontentfromfile("essay.txt",root);
    inorder(root);
    return 0;
}