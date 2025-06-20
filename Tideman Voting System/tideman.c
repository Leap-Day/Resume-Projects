#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void merge_sort(int array[], pair a[], int l, int r);
void merge(int array[], pair a[], int l, int midpoint, int r);
bool cycle_check(int loser, int original_loser, int i);
bool win_check(int potential_winner);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[j][i] > preferences[i][j])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                pair_count++;
                k++;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                pair_count++;
                k++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Create victory srength array
    int victory_strength[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        victory_strength[i] = preferences[pairs[i].winner][pairs[i].loser];
    }

    // Sort victory array in descending order with pairs array tied to it
    merge_sort(victory_strength, pairs, 0, pair_count - 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Populate locked array
    for (int i = 0; i < pair_count; i++)
    {
        if (cycle_check(pairs[i].winner, pairs[i].loser, i) == false)
        {
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
            }
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[i][j] == true && win_check(i) == true)
            {
                printf("%s\n", candidates[i]);
                return;
            }
        }
    }
    return;
}

// Merge Sort Function
void merge_sort(int array[], pair a[], int l, int r)
{
    // Base case and sort
    if (l < r)
    {
        int midpoint = (l + r) / 2;
        merge_sort(array, pairs, l, midpoint);
        merge_sort(array, pairs, midpoint + 1, r);

        // Merge
        merge(array, a, l, midpoint, r);
    }
}
// Merge Function
void merge(int array[], pair a[], int l, int midpoint, int r)
{
    // Create Temp Arrays
    int size_left = midpoint - l + 1;
    int size_right = r - midpoint;
    int arrayL[size_left];
    int arrayR[size_right];
    pair pairsL[size_left];
    pair pairsR[size_right];

    // Copy to temp arrays
    for (int i = 0; i < size_left; i++)
    {
        arrayL[i] = array[l + i];
        pairsL[i] = a[l + i];
    }
    for (int j = 0; j < size_right; j++)
    {
        arrayR[j] = array[midpoint + 1 + j];
        pairsR[j] = a[midpoint + 1 + j];
    }

    // Compare and copy back to original array
    int i = 0, j = 0, k = l;
    while (i < size_left && j < size_right)
    {
        if (arrayL[i] >= arrayR[j])
        {
            array[k] = arrayL[i];
            a[k] = pairsL[i];
            i++;
        }
        else
        {
            array[k] = arrayR[j];
            a[k] = pairsR[i];
            j++;
        }
        k++;
    }

    // Collect remaining elements
    while (i < size_left)
    {
        array[k] = arrayL[i];
        a[k] = pairsL[i];
        i++;
        k++;
    }
    while (j < size_right)
    {
        array[k] = arrayR[j];
        a[k] = pairsR[j];
        j++;
        k++;
    }
}

// Check if adding a locked pair will create a loop
bool cycle_check(int loser, int original_loser, int i)
{
    // Base case
    if (pairs[i].winner == original_loser)
    {
        return true;
    }
    else if (i < 0)
    {
        return false;
    }
    // Cycle check algorithim
    if (locked[pairs[i].winner][loser] == true)
    {
        loser = pairs[i].winner;
        return cycle_check(loser, original_loser, i - 1);
    }
    else
    {
        int j = i;
        while (j >= 0)
        {
            if (locked[pairs[j].winner][loser] == true && pairs[j].winner != original_loser)
            {
                loser = pairs[j].winner;
            }
            else if (locked[pairs[j].winner][loser] == true && pairs[j].winner == original_loser)
            {
                return true;
            }
            else
            {
                j--;
            }
        }
        return false;
    }
}

bool win_check(int potential_winner)
{
    // Check if potential winner ever loses
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[i][potential_winner] == true)
        {
            return false;
        }
    }
    return true;
}
