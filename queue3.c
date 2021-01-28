#include <stdlib.h>
#include <stdio.h>

struct node
{
        int info;
        struct node *next;
};
typedef struct Node NODE;
NODE *start=NULL;


void create()
{
    NODE *temp,*ptr;
    temp=(NODE *)malloc(sizeof(NODE));
    if(temp==NULL)
    {
        printf("nOut of Memory Space:n");
        exit(0);
    }
    printf("nEnter the data value for the node:t");
    scanf("%d",&temp->info);
    temp->next=NULL;
    if(start==NULL)
    {
        start=temp;
    }
    else
    {
        ptr=start;
        while(ptr->next!=NULL)
        {
                ptr=ptr->next;
        }
        ptr->next=temp;
    }
}

void display()
{
    struct node *ptr;
    if(start==NULL)
    {
        printf("nList is empty:n");
        return;
    }
    else
    {
        ptr=start;
        printf("nThe List elements are:n");
        while(ptr!=NULL)
        {
                printf("%dt",ptr->info );
                ptr=ptr->next ;
        }
    }
}

void insert_end()
{
    struct node *temp,*ptr;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL)
    {
        printf("nOut of Memory Space:n");
        return;
    }
    printf("nEnter the data value for the node:t" );
    scanf("%d",&temp->info );
    temp->next =NULL;
    if(start==NULL)
    {
            start=temp;
    }
    else
    {
        ptr=start;
        while(ptr->next !=NULL)
        {
            ptr=ptr->next ;
        }
        ptr->next =temp;
    }
}

void delete_begin()
{
    struct node *ptr;
    if(ptr==NULL)
    {
            printf("nList is Empty:n");
            return;
    }
    else
    {
        ptr=start;
        start=start->next ;
        printf("nThe deleted element is :%dt",ptr->info);
        free(ptr);
    }
}

void delete_pos()
{
    int i,pos;
    struct node *temp,*ptr;
    if(start==NULL)
    {
        printf("nThe List is Empty:n");
        exit(0);
    }
    else
    {
        printf("nEnter the position of the node to be deleted:t");
        scanf("%d",&pos);
        if(pos==0)
        {
            ptr=start;
            start=start->next ;
            printf("nThe deleted element is:%dt",ptr->info  );
            free(ptr);
        }
        else
        {
            ptr=start;
            for(i=0;i<pos;i++) { temp=ptr; ptr=ptr->next ;
                if(ptr==NULL)
                {
                    printf("nPosition not Found:n");
                    return;
                }
            }
            temp->next =ptr->next ;
            printf("nThe deleted element is:%dt",ptr->info );
            free(ptr);
        }
    }
}