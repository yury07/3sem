#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main ()
{
    int status, amount_of_nodes = 2, amount_of_childs, number_of_node, result, node, parent = 1;
    int **tree = (int **) calloc (amount_of_nodes, sizeof (*tree));
    
    FILE *input;
    input = fopen ("in.txt", "r");
    
    
    fscanf (input, "%d", &number_of_node);
    node = number_of_node;
    do
    {
        while (number_of_node >= amount_of_nodes)
        {
            amount_of_nodes *= 2;
            tree = (int **) realloc (tree, amount_of_nodes * sizeof (*tree));
        };
        fscanf (input, "%d", &amount_of_childs);
        tree[number_of_node] = (int *) calloc (amount_of_childs + 1, sizeof (**tree));
        tree[number_of_node][0] = amount_of_childs;
        
        for (int i = 1; i <= amount_of_childs; i++)
        {
            fscanf (input, "%d", &(tree[number_of_node][i]));
        }
        
        status = fscanf (input, "%d", &number_of_node);
    }
    
    while (status == 1);
    
    fclose (input);
    
    do
    {
        result = parent;
        amount_of_childs = tree[node][0];
        for (int i = 1; i <= amount_of_childs; i++)
        {
            result = fork ();
            if (result == 0)
            {
                node = tree[node][i];
                parent++;
                break;
            }
            else
            {
                for (int j = 1; j < parent; j++)
                    printf ("             ");
                printf ("%d(pid=%d)->%d(pid=%d)\n", node, getpid (), tree[node][i], result);
         
            }
        }
    }
    
    while (result == 0);
    
    
    for (int i = 0; i < amount_of_nodes; i++)
        if (tree[i] != NULL)
            free (tree[i]);
    free (tree);
    
    return 0;
}








/* Пример in.txt первый столбец - номер узла, остальные - его дети.
 
 1 2 3 4
 2 5 6
 3 7
 4 8 9 10 11
 5 12 13 14
 6 15
 7 0
 8 0
 9 16 17
 10 0
 11 0
 12 0
 13 0
 14 0
 15 0
 16 18
 17 0
 18 0
 
 */
