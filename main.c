/*****
@author: Nilay Chandra Barman (https://github.com/Nilaycb)
Copyright (c) 2019, All rights reserved.
Copyrights licensed under the GNU AGPLv3 License.
******/

#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
    int data;
    struct Node *next;
} node;

typedef struct SearchResultNode
{
    int isFound;
    int found;
    node *pos;
} s_r_node;


void insert(node *list, int data)
{
    node *temp=(node*)malloc(sizeof(node));
    temp->data=data;
    temp->next=NULL;

    while(list->next != NULL)
    {
        list=list->next;
    }
    list->next=temp;
}


int pop_list(node *list)
{
    node *temp;
    int r;

    if(list->next != NULL)
    {
        temp=list->next;
        list->next=list->next->next;
        r=temp->data;
        free(temp);
    }

    return r;
}


void del(node *list, int data)
{
    node *temp;

    if(list->next != NULL)
    {
        while(list->next->data != data)
        {
            list=list->next;
        }
        temp=list->next;
        list->next=list->next->next;
        free(temp);
    }
}


s_r_node* search(node *list, int data)
{
    int pos=0, found=0;
    s_r_node *res=(s_r_node*)malloc(sizeof(s_r_node));
    res->isFound=0;
    res->found=0;
    res->pos=(node*)malloc(sizeof(node));
    res->pos->next=NULL;

    if(list->next == NULL)
    {
        return res;
    }
    else
    {
        while(list->next != NULL)
        {
            pos++;

            if(list->next->data == data)
            {
                found++;
                insert(res->pos, pos);
            }

            list=list->next;
        }

        if(found > 0)
        {
            res->isFound=1;
            res->found=found;

            return res;
        }
        else
        {
            return res;
        }
    }
}


void display(node *list)
{
    if(list->next == NULL)
    {
        printf("\n\n\t\tPlatform Allocation Request List is EMPTY!");
        return;
    }

    printf("\n\n\t\tPlatform Allocation Request List\n\n\t\t");
    while(list->next != NULL)
    {
        printf("%d", list->next->data);

        if(list->next->next != NULL)
        {
            printf(" --> ");
        }

        list=list->next;
    }
}


int count(node *list)
{
    int c=0;

    while(list->next != NULL)
    {
        c++;
        list=list->next;
    }
    return c;
}


void sys_setup(char *sname, int *no_of_plt)
{
    printf("\n\n\t\t### Railway Automated Platform Allocation System Setup ###");
    printf("\n\n\t\tStation Name: ");
    scanf("%[^\n]", sname);
    printf("\t\tNo of Platform: ");
    scanf("%d", no_of_plt);
}


void plt_status(int no_of_plt, int plt_alloc_flag[], int plt_alloc[])
{
    printf("\n\n\t\tTotal Platform: %d\n", no_of_plt);

    int i;
    for(i=0; i<no_of_plt; i++)
    {
        if(plt_alloc_flag[i] == 0)
        {
            printf("\n\t\t\tPlatform %d is FREE.", i+1);
        }
        else
        {
            printf("\n\t\t\tPlatform %d is OCCUPIED by Train Number %d.", i+1, plt_alloc[i]);
        }
    }
}


int checkFreePlt(int no_of_plt, int plt_alloc_flag[])
{
    int i, reslt_index = -1; //negative index denotes no platform is available by default
    for(i=0; i<no_of_plt; i++)
    {
        if(plt_alloc_flag[i] == 0)
        {
            reslt_index = i;
            break;
        }
    }

    return reslt_index;
}


int searchPlt(int no_of_plt, int plt_alloc_flag[], int plt_alloc[], int train_no)
{
    int i, reslt_index = -1; //negative index denotes no platform is available by default
    for(i=0; i<no_of_plt; i++)
    {
        if((plt_alloc_flag[i] == 1) && (plt_alloc[i] == train_no))
        {
            reslt_index = i;
            break;
        }
    }

    return reslt_index;
}


int checkTrainNumAvail(int no_of_plt, int plt_alloc_flag[], int plt_alloc[], node *wlist, int train_no)
{
    int isAvailable=1; //0 means it is in use, 1 means not in use

    int i;
    for(i=0; i<no_of_plt; i++)
    {
        if((plt_alloc_flag[i] == 1) && (plt_alloc[i] == train_no))
        {
            isAvailable = 0;
        }
    }

    s_r_node *s=search(wlist, train_no);
    if(s->isFound == 1)
    {
        isAvailable = 0;
    }

    return isAvailable;
}


void alloc_plt(int no_of_plt, int plt_alloc_flag[], int plt_alloc[], node *wlist)
{
    int train_no, freePltRslt = -1, flag=1;

    printf("\n\n\t\tPlatform Allocation");
    while(1)
    {
        flag = 1;

        printf("\n\n\t\tEnter Train Number: ");
        scanf("%d", &train_no);

        if(checkTrainNumAvail(no_of_plt, plt_alloc_flag, plt_alloc, wlist, train_no) == 0)
        {
            printf("\n\t\tTrain Number %d already exists!", train_no);
            flag=0;
        }
        else
        {
            break;
        }
    }

    if((count(wlist) != 0) && (flag == 1))
    {
        printf("\n\t\t\t>> [info] Pending platform allocation request found.");
        insert(wlist, train_no);
        printf("\n\t\t\t>> [saved] Train Number %d added in the platform allocation request list.", train_no);

        printf("\n\t\t\t>> [processing] Pending platform allocation requests are being processed.\n");
        while(1)
        {
            freePltRslt = checkFreePlt(no_of_plt, plt_alloc_flag);

            if(freePltRslt >= 0)
            {
                int val = pop_list(wlist);
                plt_alloc[freePltRslt] = val;
                plt_alloc_flag[freePltRslt] = 1;

                printf("\n\t\t\t>> [allocation] Train Number %d allocated to platform %d.", val, freePltRslt+1);
            }
            else
            {
                break;
            }
        }

        printf("\n\n\t\t\t>> [Completed].");
    }
    else if((count(wlist) == 0) && (flag == 1))
    {
        printf("\n\t\t\t>> [info] No pending platform allocation request found.");
        printf("\n\t\t\t>> [processing] Platform allocation request is being processed.\n");

        freePltRslt = checkFreePlt(no_of_plt, plt_alloc_flag);

        if(freePltRslt >= 0)
        {
            plt_alloc[freePltRslt] = train_no;
            plt_alloc_flag[freePltRslt] = 1;

            printf("\n\t\t\t>> [allocation] Train Number %d allocated to platform %d.", train_no, freePltRslt+1);
        }
        else
        {
            insert(wlist, train_no);
            printf("\n\t\t\t>> [saved] Train Number %d added in the platform allocation request list.", train_no);
        }

        printf("\n\n\t\t\t>> [Completed].");
    }
}


void dealloc_plt(int no_of_plt, int plt_alloc_flag[], int plt_alloc[], node *wlist)
{
    int train_no, searchPltRslt = -1, flag=1;

    printf("\n\n\t\tPlatform Deallocation");
    while(1)
    {
        flag = 1;

        printf("\n\n\t\tEnter Train Number: ");
        scanf("%d", &train_no);

        if(checkTrainNumAvail(no_of_plt, plt_alloc_flag, plt_alloc, wlist, train_no) == 1)
        {
            printf("\n\t\tTrain Number %d doesn't exist!", train_no);
            flag = 0;
        }
        else
        {
            break;
        }
    }


    printf("\n\t\t\t>> [processing] Platform deallocation request is being processed.\n");
    if((count(wlist) == 0) && (flag == 1))
    {
        searchPltRslt = searchPlt(no_of_plt, plt_alloc_flag, plt_alloc, train_no);

        if(searchPltRslt >= 0)
        {
            plt_alloc[searchPltRslt] = -1;
            plt_alloc_flag[searchPltRslt] = 0;

            printf("\n\t\t\t>> [deallocation] Train Number %d deallocated from platform %d.", train_no, searchPltRslt+1);
        }
        else
        {
            printf("\n\t\t\t>> [error!] Train Number %d was not found!", train_no);
        }

        printf("\n\n\t\t\t>> [Completed].");
    }
    else if((count(wlist) != 0) && (flag == 1))
    {
        searchPltRslt = searchPlt(no_of_plt, plt_alloc_flag, plt_alloc, train_no);

        if(searchPltRslt >= 0)
        {
            plt_alloc[searchPltRslt] = -1;
            plt_alloc_flag[searchPltRslt] = 0;

            printf("\n\t\t\t>> [deallocation] Train Number %d deallocated from platform %d.", train_no, searchPltRslt+1);

            printf("\n\t\t\t>> [checking] Looking for allocation request from the waiting list.", train_no, searchPltRslt+1);
            int freePltRslt = -1;
            while(1)
            {
                freePltRslt = checkFreePlt(no_of_plt, plt_alloc_flag);

                if(freePltRslt >= 0)
                {
                    int val = pop_list(wlist);
                    plt_alloc[freePltRslt] = val;
                    plt_alloc_flag[freePltRslt] = 1;

                    printf("\n\t\t\t>> [allocation] Train Number %d allocated to platform %d.", val, freePltRslt+1);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            s_r_node *s=search(wlist, train_no);
            if(s->isFound == 1)
            {
                del(wlist, train_no);
            }
            else
            {
                printf("\n\t\t\t>> [error!] Train Number %d was not found!", train_no);
            }
        }

        printf("\n\n\t\t\t>> [Completed].");
    }
}


int main()
{
    int n, i;
    char sname[151];
    int no_of_plt=0;

    printf("\n\n\t\t-----------------------------------------------");
    printf("\n\t\t-----------------------------------------------");

    sys_setup(sname, &no_of_plt);

    int plt_alloc_flag[no_of_plt]; //boolean for platform allocation status
    int plt_alloc[no_of_plt]; //holds train no with respect to the platform number

    for(i=0; i<no_of_plt; i++)
    {
        plt_alloc_flag[i] = 0; //All Platforms are FREE by default
    }

    //initialization of waiting list for platform allocation
    node *wlist=(node*)malloc(sizeof(node));
    wlist->next=NULL;

    printf("\n\t\t-----------------------------------------------");
    printf("\n\t\t-----------------------------------------------");

    printf("\n\n\t\t### Railway Automated Platform Allocation System ###");
    printf("\n\n\t\t\t%s\n", sname);

    printf("\n\t\t\tMENU");
    printf("\n\t\t\t1 => Record Arrival");
    printf("\n\t\t\t2 => Record Departure");
    printf("\n\t\t\t3 => Platform Status");
    printf("\n\t\t\t4 => Platform Allocation Request List");
    printf("\n\t\t\t5 => System Setup");
    printf("\n\t\t\t6 => System Shutdown");

    while(1)
    {
        printf("\n");

        printf("\n\t\t-----------------------------------------------");

        printf("\n\t\t> ");
        scanf("%d", &n);

        if(n == 1)
        {
            printf("\t\t-----------------------------------------------");
            alloc_plt(no_of_plt, plt_alloc_flag, plt_alloc, wlist);
        }
        else if(n == 2)
        {
            printf("\t\t-----------------------------------------------");
            dealloc_plt(no_of_plt, plt_alloc_flag, plt_alloc, wlist);
        }
        else if(n == 3)
        {
            printf("\t\t-----------------------------------------------");
            plt_status(no_of_plt, plt_alloc_flag, plt_alloc);
        }
        else if(n == 4)
        {
            printf("\t\t-----------------------------------------------");
            display(wlist);
        }
        else if(n == 5)
        {
            printf("\t\t-----------------------------------------------");
            getchar();
            sys_setup(sname, &no_of_plt);
        }
        else if(n == 6)
        {
            break;
        }
        else
        {
            printf("\t\t-----------------------------------------------");
            printf("\n\n\t\tIncorrect selection! Try again.");
        }

        printf("\n\n\t\t-----------------------------------------------");
        printf("\n\t\t-----------------------------------------------");

        printf("\n\n\t\t### Railway Automated Platform Allocation System ###");
        printf("\n\n\t\t\t%s\n", sname);

        printf("\n\t\t\tMENU");
        printf("\n\t\t\t1 => Record Arrival");
        printf("\n\t\t\t2 => Record Departure");
        printf("\n\t\t\t3 => Platform Status");
        printf("\n\t\t\t4 => Platform Allocation Request List");
        printf("\n\t\t\t5 => System Setup");
        printf("\n\t\t\t6 => System Shutdown");
    }

    printf("\t\t-----------------------------------------------");
    printf("\n\t\t-----------------------------------------------");

    printf("\n\n\t\tThank You, see you again.");
    printf("\n\t\tSHUTTING DOWN...\n");

    return 0;
}
