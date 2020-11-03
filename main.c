#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PLAYERS 32
#define MAX_GROUPS 8
#define MAX_WEEKS 10
#define PLAYERS_PER_GROUP 4
#define MAX_GROUP_COMBINATIONS 35960 // Max possible player combinations in a group without repetition 32!/(4!*(32-4)!) = 35960

void binary_string(char *str, const unsigned int *data, unsigned int len) {
    unsigned int i, e = len - 1;
    for (i = 1 << (len - 1); i > 0; i = i / 2)
        str[e--] = (*data & i) ? '1' : '0';
    str[len] = '\0';
}

void print_groups(unsigned int *groups) {
    for (int i = 0; i < MAX_GROUPS; i++) {
        char str[MAX_PLAYERS + 1] = "";
        binary_string(str, &groups[i], MAX_PLAYERS);
        printf("Group %d: %s\n", i + 1, str);
    }
}

void print_weeks(unsigned int *weeks, unsigned int max_weeks_solved) {
    for (int w = 0; w < max_weeks_solved; w++) {
        printf("Week %d\n", w + 1);
        print_groups(&weeks[w]);
    }
}

void shuffle(unsigned int *array, int n) {
    if (n == 0) return;
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        unsigned int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void update_constraints(unsigned int *constraints, unsigned int group_mask, bool apply) {
    unsigned int constraint_mask = 0, i = 0;
    int players_indexes[PLAYERS_PER_GROUP] = {0};

    for (unsigned int player_index = 0, player_mask = 1;
         (player_mask <= (1 << (MAX_PLAYERS - 1))) && (i < PLAYERS_PER_GROUP); player_mask = player_mask * 2) {

        // If player is present in the group then add the player to the constraint mask
        if (group_mask & player_mask) {
            constraint_mask |= player_mask;
            players_indexes[i] = player_index;
            i++;
        }

        player_index++;
    }

    for (i = 0; i < PLAYERS_PER_GROUP; i++) {
        if (apply) constraints[players_indexes[i]] |= constraint_mask; // Apply with OR
        else constraints[players_indexes[i]] ^= constraint_mask; // Undo with XOR
    }
}

unsigned int generate_group_combinations(unsigned int position, int offset, unsigned int *combinations,
                                         unsigned int combinations_count, unsigned int available_players_mask,
                                         unsigned int *constraints) {
    unsigned int current_combination_mask = combinations[combinations_count];

    if (position >= PLAYERS_PER_GROUP) {
        // Trivial case
        // New valid combination found. Update and return the total amount of combinations found
        combinations_count++;
        combinations[combinations_count] = current_combination_mask;
    } else {
        // Recursive case
        unsigned int player = 0;
        player |= 1UL << offset;

        // Search next available player for current combination
        for (int i = offset; i < MAX_PLAYERS; i++) {
            // Check if the player is available in the current week. And check if player is not present in the current combination. And check if player did not joined any group with any of the players of the current combination.
            unsigned int player_constraints = constraints[i]; // Get player constraints
            if ((player & available_players_mask) && !(player & current_combination_mask) &&
                !(player_constraints & current_combination_mask)) {
                current_combination_mask = player | current_combination_mask; // Add current player into the current group combination
                combinations[combinations_count] = current_combination_mask;
                available_players_mask &= ~(1UL << i); // Remove the current player from the available players mask
                combinations_count = generate_group_combinations(position + 1, i + 1, combinations, combinations_count, available_players_mask, constraints);
                available_players_mask |= 1UL << i; // Add the current player to the available players mask again
                current_combination_mask &= ~(1UL << i); // Remove the current player from the current combination
            }

            player *= 2; // Iterate to the next player just by shifting one bit
        }
    }

    return combinations_count;
}

void solve_group(unsigned int *weeks, unsigned int *constraints, int week_index, int group_index, int *max_week_solved) {

    if (week_index >= MAX_WEEKS) {
        // Solved the Social Golfer Problem by completing the 10 weeks!
        printf("Solved the Social Golfer Problem by completing the 10 weeks!\n");
        print_weeks(weeks, MAX_WEEKS);
        exit(0);
    }

    if (group_index >= MAX_GROUPS) {
        // Trivial case
        // Solved the Social Golfer Problem up to the current week!

        // Display the solution found for the current week
        if (week_index > *max_week_solved) {
            printf("\n\nFound a new solution with %d weeks:\n", week_index + 1);
            *max_week_solved = week_index;
            print_weeks(weeks, week_index + 1);
        }

        // Continue solving the next week
        solve_group(weeks, constraints, week_index + 1, 0, max_week_solved);
    } else {
        // Recursive case
        unsigned int *week_groups = &weeks[week_index];
        week_groups[group_index] = 0; // Reset current group

        // Now we need to get all the available players that can join this group in the current state.
        // Rule 1) Use only available players within the current week.

        // Apply a NOR operation to all the groups will give us the available players in the current week
        unsigned int available_players_mask = 0;
        for (int i = 0; i < group_index; i++)
            available_players_mask = available_players_mask | week_groups[i]; // OR
        available_players_mask = ~available_players_mask; // NOT

        // Now we will generate all non-repetitive players combinations with the available players in the current group
        // Rule 2) Combinations cannot contain players that are in the same group in previous weeks.

        // 143840 bytes of memory needed to store all the possible combinations. MAX_GROUP_COMBINATIONS * sizeof(unsigned int)
        unsigned int group_combinations[MAX_GROUP_COMBINATIONS] = {0};

        // Generate non-repetitive combinations of 4 players using all current available players (32 at max)
        unsigned int combinations_count = generate_group_combinations(0, 0, group_combinations, 0, available_players_mask, constraints);
        shuffle(group_combinations, combinations_count);

        for (int c = 0; c < combinations_count; c++) {
            // Use next group combination available
            week_groups[group_index] = group_combinations[c];

            // Apply restrictions for this combination
            update_constraints(constraints, group_combinations[c], true);


            // Solve next group recursively
            solve_group(weeks, constraints, week_index, group_index + 1, max_week_solved);

            // Undo restrictions for this combination
            update_constraints(constraints, group_combinations[c], false);
        }
    }
}

int main(__unused int argc, __unused char *argv[]) {
    printf("Social Golfer Problem solver\n\n");
    srand(time(0));

    // Weeks definition
    unsigned int weeks[MAX_WEEKS][MAX_GROUPS] = {0};

    // Definition of pairs of players taken
    unsigned int constraints[MAX_PLAYERS] = {0};

    // Store the max number of weeks solved
    int max_week_solved = 0;

    // Start solving from group 0 of week 0
    solve_group(weeks, constraints, 0, 0, &max_week_solved);
}
