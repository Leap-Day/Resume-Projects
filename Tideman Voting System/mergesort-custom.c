#include <cs50.h>
#include <stdio.h>
#include <string.h>

void merge_sort(int array[], int l, int r);
void merge(int array[], int l, int midpoint, int r);

int test[] = {3, 1, 0, 4, 4, 8, 6, 7, 5, 9};

int main(void)
{
    merge_sort(test, 0, 10);
    for (int x = 0; x < 10; x++)
    {
        printf("%d ", test[x]);
    }
    printf("\n");
}


void merge_sort(int array[], int l, int r)
{
    // Base case and sort
    if (l < r)
    {
        int midpoint = (l + r) / 2;
        merge_sort(array, l, midpoint);
        merge_sort(array, midpoint + 1, r);
    // Merge
        merge(array, l, midpoint, r);
    }
}
void merge(int array[], int l, int midpoint, int r)
{
    // Create Temp Arrays
    int size_left = midpoint - l + 1;
    int size_right = r - midpoint;
    int arrayL[size_left];
    int arrayR[size_right];

    // Copy to temp arrays
    for (int i = 0; i < size_left; i++)
    {
        arrayL[i] = array[l + i];
    }
    for (int j = 0; j < size_right; j++)
    {
        arrayR[j] = array[midpoint + 1 + j];
    }

    // Compare and copy back to original array
    int i = 0, j = 0, k = l;
    while (i < size_left && j < size_right)
    {
        if (arrayL[i] >= arrayR[j])
        {
            array[k] = arrayL[i];
            i++;
        }
        else
        {
            array[k] = arrayR[j];
            j++;
        }
        k++;
    }

    // Collect remaining elements
    while (i < size_left)
    {
        array[k] = arrayL[i];
        i++;
        k++;
    }
    while (j < size_right)
    {
        array[k] = arrayR[j];
        j++;
        k++;
    }
}
