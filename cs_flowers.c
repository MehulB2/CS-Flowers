// CS Flowers
//
// This program was written by Mehul Bisht
// on 25/3/2024
//
/*
In this tactical game, players navigate a map to eradicate aggressive flowers 
threatening your house. They strategically cut bushes, defend against root attacks, 
and avoid dangers while progressing. Balancing offense and defense is crucial, 
requiring careful planning to outmaneuver flower defenses and save the 
environment from floral peril.*/

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define MAX_ROW 11
#define MAX_COL 11
#define TRUE 1
#define FALSE 0

//------------------------------------------------------------------------------
// Struct & Enum
//------------------------------------------------------------------------------

enum flower_state {
    NONE,
    DORMANT,
    DISTURBED,
    AWAKENED,
    ANXIOUS,
    ENERGETIC
};

struct flower {
    enum flower_state state;
    int bush_placed;
    int true_count;
    int roots_rem;
    int has_attacked;
    int in_range;
    int prot_called_this_turn;
};

enum tile_type {
    EMPTY,
    BUSH,
    BRANCH,
    DANGER,
    ROOT,
    POLLEN,
    SEEDLING,
    FLOWER,
};

struct tile {
    enum tile_type type;
    struct flower flower;
};

void add_foliage (
int num_foliage, 
struct tile map[MAX_ROW][MAX_COL]);

void add_flower (
int num_flower, 
struct tile map[MAX_ROW][MAX_COL]);

void command_loop(
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void cutting (
int player_row, 
int player_col, 
char direction, 
struct tile map[MAX_ROW][MAX_COL]);

void alert (
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void check_win (
struct tile map[MAX_ROW][MAX_COL]);

void flower_prot(
int row, 
int col, 
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void check_player(
int row, 
int col, 
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void root_attack (
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void chg_danger (
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

void check_attack_or_defense (
int player_row, 
int player_col, 
struct tile map[MAX_ROW][MAX_COL]);

int is_within_range (
int row1, 
int col1, 
int row2, 
int col2, 
int range);

void reset_protection_flags (
struct tile map[MAX_ROW][MAX_COL]);
void initialise_map(struct tile map[MAX_ROW][MAX_COL]);
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
);

void print_tile(struct tile tile);
void print_flower(struct flower flower);

//------------------------------------------------------------------------------
// Main Function
//------------------------------------------------------------------------------

int main(void) {
    struct tile map[MAX_ROW][MAX_COL];
    initialise_map(map);

    printf("Welcome to CS Flowers!\n");


    int player_row;
    int player_col;

    printf("Player's starting position: ");
    scanf(" %d %d", &player_row, &player_col);

    while (player_row != 0 && player_row != MAX_ROW - 1 
    && player_col != 0 && player_col != MAX_COL - 1) {
        printf("Invalid starting position! \n");
        printf("Re-enter starting position: ");
        scanf(" %d %d", &player_row, &player_col);
    }
    print_map(map, player_row, player_col);

    int num_fol;
    printf("How many tiles of foliage: ");
    scanf(" %d", &num_fol);
    add_foliage(num_fol, map);
    print_map(map, player_row, player_col);

    int num_flower;
    printf("How many flowers: ");
    scanf(" %d", &num_flower);
    if (num_flower == 0) {
        printf("All flowers are eradicated and UNSW has been saved! \n");
        return 0;
    }
    
    add_flower(num_flower, map);
    print_map(map, player_row, player_col);

    command_loop(player_row, player_col, map);
    return 0;
}

/*add_foliage: This function adds foliage (bushes or branches) to the game 
map based on user input. It checks for valid positions and 
updates the map accordingly.*/
void add_foliage(int num_foliage, struct tile map[MAX_ROW][MAX_COL]) {
    char type;
    int row;
    int col;
    char orientation;
    int length;
    for (int i = 0; i < num_foliage; i ++) {
        scanf(" %c", &type);
        if (type == 'u') {
            scanf(" %d %d %c %d", &row, &col, &orientation, &length);
            if (row == 0 || row == 10 ) {
                printf("Invalid foliage position! \n");
                //checking whether the branch is added within the boundaries
            }
            else if (col == 0 || col == MAX_COL - 1) {
                printf("Invalid foliage position! \n");
                //checking whether the branch is added within the boundaries
            }
            else {
                for (int j = 0; j < length; j++) {
                    if (orientation == 'h') {
                        map[row][col + j].type = BUSH;
                    }
                    else if (orientation == 'v') {
                        map[row + j][col].type = BUSH;
                    }
                }
                printf("Bush added! \n");
            }
        }
        else if (type == 'b') {
            scanf(" %d %d", &row, &col);
            if (row == 0 || row == MAX_ROW -1) {
                printf("Invalid foliage position! \n");
                //checking whether the branch is added within the boundaries
            }
            else if (col == 0 || col == MAX_COL - 1) {
                printf("Invalid foliage position! \n");
                //checking whether the branch is added within the boundaries
            }
            else {
                map[row][col].type = BRANCH;
                printf("Branch added! \n");
            }
            
        }
    }
}
/*add_flower: This function adds flowers to the game map based on user input. 
It ensures that the flowers are added in valid positions and initializes 
their properties.*/
void add_flower(int num_flower, struct tile map[MAX_ROW][MAX_COL]){
    int row;
    int col;
    if (num_flower >= 25) {
        for(row = 1; row < MAX_ROW - 1; row += 2) {
            col = 1;
            for (col = 1; col < MAX_COL - 1; col += 2) {
                if (map[row][col].type == BUSH 
                || map[row][col].type == BRANCH) {
                }
                else {
                    //initialise properties of the flowers
                    map[row][col].type = FLOWER;
                    map[row][col].flower.state = DORMANT;
                    map[row][col].flower.true_count = 0;
                    map[row][col].flower.bush_placed = TRUE;
                    map[row][col].flower.roots_rem = 0;
                    map[row][col].flower.has_attacked = FALSE;
                    map[row][col].flower.in_range = FALSE;
                    map[row][col].flower.prot_called_this_turn = FALSE;
                }
            }
        }
    }
    else {
        for (int i = 0; i < num_flower; i ++) {
            scanf(" %d %d", &row, &col);
            if (row % 2  != 1 || col % 2 != 1 || row <= 0 
            || col <= 0 || col > MAX_COL - 1 || row > MAX_ROW - 1) {
                printf("Invalid flower position! \n");
                // checking whether the bush is added within the right boundary
            }
            else {
                if (map[row][col].type == BUSH 
                || map[row][col].type == BRANCH) {
                    printf("Invalid flower position! \n");
                }
                else {
                    //initialise properties of the flower
                    map[row][col].type = FLOWER;
                    map[row][col].flower.state = DORMANT;
                    map[row][col].flower.bush_placed = TRUE;
                    map[row][col].flower.true_count = 0;
                    map[row][col].flower.roots_rem = 0;
                    map[row][col].flower.has_attacked = FALSE;
                    map[row][col].flower.in_range = FALSE;
                    map[row][col].flower.prot_called_this_turn = FALSE;
                }
            }
        }
    }
}
/*command_loop: This function handles the movement of the player within the 
game map. It takes user commands for movement and performs various actions 
such as cutting bushes, updating danger levels, and checking for win 
conditions.*/
void command_loop(int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    printf("Game Started! \n");
    printf("Enter command: ");
    char command;
    char direction;
    while (scanf(" %c", &command) == 1) {
        reset_protection_flags(map);
        if (command == 'c' ) {
            //scan new letter for direction
            scanf(" %c", &direction);
            //perform cutting function
            cutting(player_row, player_col, direction, map);
            chg_danger(player_row, player_col, map);
            print_map(map, player_row, player_col);
            check_win(map);
            printf("Enter command: ");
            //calling functions for cutting
        }
        else if (command == 'w') {
            if (player_row - 1 >= 0) {
                //check if player is not running through a flower,bush or root
                if (map[player_row - 1][player_col].type != BUSH 
                && map[player_row - 1][player_col].type != FLOWER 
                && map[player_row - 1][player_col].type != ROOT) {
                    player_row --;
                    //change all danger tiles to root tiles
                    chg_danger(player_row, player_col, map);
                    if (map[player_row][player_col].type == BRANCH) {
                        //alert all nearby flowers if stepped on branch
                        alert(player_row, player_col, map);
                        map[player_row][player_col].type = EMPTY;
                        //flower checks whether to attack or defend
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    else if (map[player_row][player_col].type == EMPTY) {
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
                else {
                    /*if it is a flower, root or bush then keep the player at
                    the same spot and run the corresponding functions*/
                    check_attack_or_defense(player_row, player_col, map);
                    chg_danger(player_row, player_col, map);
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }  
            }
            else {
                chg_danger(player_row, player_col, map);
                check_attack_or_defense(player_row, player_col, map);
                print_map(map, player_row, player_col);
                printf("Enter command: ");
            }
        }
        else if (command == 's') {
            if (player_row + 1 <= MAX_ROW - 1) {
                if (map[player_row + 1][player_col].type != BUSH 
                && map[player_row + 1][player_col].type != FLOWER 
                && map[player_row + 1][player_col].type != ROOT) {
                    player_row ++;
                    chg_danger(player_row, player_col, map);
                    if (map[player_row][player_col].type == BRANCH) {
                        alert(player_row, player_col, map);
                        map[player_row][player_col].type = EMPTY;
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    else if (map[player_row][player_col].type == EMPTY) {
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
                else {
                    check_attack_or_defense(player_row, player_col, map);
                    chg_danger(player_row, player_col, map);
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
            }
            else {
                chg_danger(player_row, player_col, map);
                print_map(map, player_row, player_col);
                printf("Enter command: ");
            }
        }
        else if (command == 'd') {
            if (player_col + 1 <= MAX_COL - 1) {
                if (map[player_row][player_col + 1].type != BUSH 
                && map[player_row][player_col + 1].type != FLOWER 
                && map[player_row][player_col + 1].type != ROOT) {
                    player_col ++;
                    chg_danger(player_row, player_col, map);
                    if (map[player_row][player_col].type == BRANCH) {
                        alert(player_row, player_col, map);
                        map[player_row][player_col].type = EMPTY;

                        check_attack_or_defense(player_row, player_col, map);
                    }
                    else if (map[player_row][player_col].type == EMPTY) {

                        check_attack_or_defense(player_row, player_col, map);
                    }
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
                else {
                    check_attack_or_defense(player_row, player_col, map);
                    chg_danger(player_row, player_col, map);
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
            }
            else {
                chg_danger(player_row, player_col, map);
                check_attack_or_defense(player_row, player_col, map);
                print_map(map, player_row, player_col);
                printf("Enter command: ");
            }
        }
        else if (command == 'a') {
            if (player_col - 1 >= 0) {
                if (map[player_row][player_col - 1].type != BUSH 
                && map[player_row][player_col - 1].type != FLOWER 
                && map[player_row][player_col - 1].type != ROOT) {
                    player_col --;
                    chg_danger(player_row, player_col, map);
                    if (map[player_row][player_col].type == BRANCH) {
                        alert(player_row, player_col, map);
                        map[player_row][player_col].type = EMPTY;
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    else if (map[player_row][player_col].type == EMPTY) {
                        check_attack_or_defense(player_row, player_col, map);
                    }
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
                else {
                    check_attack_or_defense(player_row, player_col, map);
                    chg_danger(player_row, player_col, map);
                    print_map(map, player_row, player_col);
                    printf("Enter command: ");
                }
                
            }
            else {
                check_attack_or_defense(player_row, player_col, map);
                chg_danger(player_row, player_col, map);
                print_map(map, player_row, player_col);
                printf("Enter command: ");
            }
        }
        else if (command == 'i') {
            check_attack_or_defense(player_row, player_col, map);
            chg_danger(player_row, player_col, map);
            print_map(map, player_row, player_col);
            printf("Enter command: "); 
        }
    }
}
/*cutting: This function handles the cutting action performed by the player. 
It checks for valid cutting directions and updates the map accordingly, 
removing bushes or flowers.*/
void cutting(int player_row, int player_col, char direction, 
struct tile map[MAX_ROW][MAX_COL]) {
    if (direction == 'w') {
        if (player_row - 1 >= 0) {
            //check the tile above the player
            //If bush then replace with empy tile
            //If flower then turn into empty and alert flowers
            //if root then turn into branch
            //Not checking for branch as it cannot be cut
            if (map[player_row - 1][player_col].type == BUSH) {
                check_attack_or_defense(player_row, player_col, map);
                map[player_row - 1][player_col].type = EMPTY;
            }
            else if (map[player_row - 1][player_col].type == FLOWER) {
                map[player_row - 1][player_col].type = EMPTY;
                alert(player_row - 1, player_col, map);
                check_attack_or_defense(player_row, player_col, map);
            }
            else if (map[player_row - 1][player_col].type == ROOT) {
                map[player_row - 1][player_col].type = BRANCH;
                check_attack_or_defense(player_row, player_col, map);
            }
            else {
                check_attack_or_defense(player_row, player_col, map);
            }
        }
    }
    else if (direction == 's') {
        if (player_row + 1 <= MAX_ROW - 1) {
            if (map[player_row + 1][player_col].type == BUSH) {
                check_attack_or_defense(player_row, player_col, map);
                map[player_row + 1][player_col].type = EMPTY;
            }
            else if (map[player_row + 1][player_col].type == FLOWER) {
                map[player_row + 1][player_col].type = EMPTY;
                alert(player_row + 1, player_col, map);
                check_attack_or_defense(player_row, player_col, map);
            } 
            else if (map[player_row + 1][player_col].type == ROOT) {
                map[player_row + 1][player_col].type = BRANCH;
                check_attack_or_defense(player_row, player_col, map);
            }
            else {
                check_attack_or_defense(player_row, player_col, map);
            }
        }
    }
    else if (direction == 'd') {
        if (player_col + 1 <=  MAX_COL - 1) {
            if (map[player_row][player_col + 1].type == BUSH) {
                check_attack_or_defense(player_row, player_col, map);
                map[player_row][player_col + 1].type = EMPTY;
                
            }
            else if (map[player_row][player_col + 1].type == FLOWER) {
                map[player_row][player_col + 1].type = EMPTY;
                alert(player_row, player_col + 1, map);
                check_attack_or_defense(player_row, player_col, map);
                
            }
            else if (map[player_row][player_col + 1].type == ROOT) {
                map[player_row][player_col + 1].type = BRANCH;
                check_attack_or_defense(player_row, player_col, map);
            }
            else {
                check_attack_or_defense(player_row, player_col, map);
            }
        }
    }
    else if (direction == 'a') {
        if (player_col - 1 >= 0) {
            if (map[player_row][player_col - 1].type == BUSH) {
                check_attack_or_defense(player_row, player_col, map);
                map[player_row][player_col - 1].type = EMPTY;
            }
            
            else if (map[player_row][player_col - 1].type == FLOWER) {
                map[player_row][player_col - 1].type = EMPTY;
                alert(player_row, player_col - 1, map);
                check_attack_or_defense(player_row, player_col - 1, map);
            }
            else if (map[player_row][player_col - 1].type == ROOT) {
                map[player_row][player_col - 1].type = BRANCH;
                check_attack_or_defense(player_row, player_col - 1, map);
            }
            else {
                check_attack_or_defense(player_row, player_col - 1, map);
            }
        }
    }
}
/*alert: This function alerts nearby flowers when a flower is cut or the 
player steps on a branch. It updates the state of nearby flowers 
based on their proximity to the cut location.*/
void alert(int player_row, int player_col, struct tile map[MAX_ROW][MAX_COL]) {
    int start_row = player_row - 2;
    int end_row = player_row + 2;
    int start_col = player_col - 2;
    int end_col = player_col + 2;
    //make sure the rows and cols are within the boundaries of the map
    if (start_row < 0) {
        start_row = 0;
    }
    if (end_row > MAX_ROW - 1) {
        end_row = MAX_ROW - 1;
    }
    if (start_col < 0) {
        start_col = 0;
    }
    if (end_col > MAX_COL - 1) {
        end_col = MAX_COL - 1;
    }

    for (int row = start_row; row <= end_row; row++) {
        // Check if the row is within the array boundaries
        for (int col = start_col; col <= end_col; col++) {
            // Check if the column is within the array boundaries
            if (map[row][col].type == FLOWER 
            && map[row][col].flower.state < AWAKENED) {
                //increasing the state of the flower as well as adding roots
                if (map[row][col].flower.state == DORMANT) {
                    map[row][col].flower.state = DISTURBED;
                    map[row][col].flower.roots_rem ++;
                }
                else if (map[row][col].flower.state == DISTURBED) {
                    map[row][col].flower.state = AWAKENED;
                    map[row][col].flower.roots_rem += 2;
                }
            }
            else if (map[row][col].flower.state == AWAKENED) {
                map[row][col].flower.roots_rem += 2;
            }
        }
    }
}
/*check_win: This function checks if all flowers have been eradicated 
from the map, indicating a win condition.*/
void check_win (struct tile map[MAX_ROW][MAX_COL]) {
    int flowers_left = 0;
    for (int row = 0; row < MAX_ROW; row ++) {
        for (int col = 0; col < MAX_COL; col ++) {
            //loops through and checks for flowers
            if (map[row][col].type == FLOWER) {
                flowers_left ++;
            }
        }
    }
    if (flowers_left == 0) {
        //if 0 flowers then player wins
        printf("All flowers are eradicated and UNSW has been saved! \n");       
        exit(0);
    }
}
/*flower_prot: This function determines the protection strategy of 
flowers against the player. It evaluates weak points and places bushes 
to adjacent squares to defend flowers.*/
void flower_prot(int row, int col, int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    // Check if the tile contains a flower that is not in DORMANT state
    if (map[row][col].type == FLOWER 
    && map[row][col].flower.state != DORMANT 
    && map[row][col].flower.prot_called_this_turn == FALSE) {
        // Check if the flower's weak points are available for placing bushes
        if ((row + 1 != player_row || col != player_col) 
        && map[row + 1][col].type != BUSH 
        && map[row + 1][col].type != FLOWER 
        && map[row + 1][col].type != ROOT) {
            if (map[row + 1][col].type == EMPTY 
            || map[row + 1][col].type == BRANCH) {
                // Check if the weak point is either EMPTY or BRANCH
                if (map[row][col].flower.bush_placed == TRUE) {
                    map[row][col].flower.true_count ++;
                    map[row][col].flower.prot_called_this_turn = TRUE;
                    if (map[row][col].flower.true_count == 2) {
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.true_count = 0;
                    }
                }
                if (map[row][col].flower.bush_placed == FALSE ) {
                    // Place a bush at the weak point
                    // If a bush was placed on a branch, defend the flower 
                    //again and place another bush if possible
                    if (map[row + 1][col].type == BRANCH) {
                        check_player(row + 1, col, player_row, 
                        player_col, map);
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.prot_called_this_turn = FALSE;
                        flower_prot(row, col, player_row, player_col, map);
                    }    
                    else {             
                        check_player(row + 1, col, player_row, 
                        player_col, map);
                        map[row][col].flower.prot_called_this_turn = TRUE;
                        map[row][col].flower.bush_placed = TRUE;
                    }
                }
            }
        }
        else if ((row != player_row || col + 1 != player_col) 
        && map[row][col + 1].type != BUSH 
        && map[row][col + 1].type != FLOWER 
        && map[row][col + 1].type != ROOT) {
            if (map[row][col + 1].type == EMPTY 
            || map[row][col + 1].type == BRANCH) {
                // Check if the weak point is either EMPTY or BRANCH
                if (map[row][col].flower.bush_placed == TRUE) {
                    map[row][col].flower.true_count ++;
                    map[row][col].flower.prot_called_this_turn = TRUE;
                    if (map[row][col].flower.true_count == 2) {
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.true_count = 0;
                    }
                }
                if (map[row][col].flower.bush_placed == FALSE ) {
                    // Place a bush at the weak point
                    // If a bush was placed on a branch, defend the flower 
                    //again and place another bush if possible
                    if (map[row][col + 1].type == BRANCH) {
                        check_player(row, col + 1, player_row, 
                        player_col, map);
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.prot_called_this_turn = FALSE;
                        flower_prot(row, col, player_row, player_col, map);
                    }                 
                    else {
                        check_player(row, col + 1, player_row, 
                        player_col, map);
                        map[row][col].flower.prot_called_this_turn = TRUE;
                        map[row][col].flower.bush_placed = TRUE;
                    }
                }
            }
        }
        else if ((row - 1 != player_row || col != player_col) 
        && map[row - 1][col].type != BUSH 
        && map[row - 1][col].type != FLOWER
        && map[row - 1][col ].type != ROOT) {
            if (map[row - 1][col].type == EMPTY 
            || map[row - 1][col].type == BRANCH) {
                // Check if the weak point is either EMPTY or BRANCH
                if (map[row][col].flower.bush_placed == TRUE ) {
                    map[row][col].flower.true_count ++;
                    map[row][col].flower.prot_called_this_turn = TRUE;
                    if (map[row][col].flower.true_count == 2) {
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.true_count = 0;
                    }
                }
                if (map[row][col].flower.bush_placed == FALSE) {
                    // Place a bush at the weak point
                    // If a bush was placed on a branch, defend the flower 
                    //again and place another bush if possible
                    if (map[row - 1][col].type == BRANCH) {
                        check_player(row - 1, col, player_row, 
                        player_col, map);
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.prot_called_this_turn = FALSE;
                        flower_prot(row, col, player_row, 
                        player_col, map);
                    }                 
                    else {
                        check_player(row - 1, col, player_row, 
                        player_col, map);
                        map[row][col].flower.prot_called_this_turn = TRUE;
                        map[row][col].flower.bush_placed = TRUE;
                    }
                }
            }
        }
        else if ((row != player_row || col - 1 != player_col) 
        && map[row][col - 1].type != BUSH 
        && map[row][col - 1].type != FLOWER 
        && map[row][col - 1].type != ROOT) {
            if (map[row][col - 1].type == EMPTY 
            || map[row][col - 1].type == BRANCH) {
                // Check if the weak point is either EMPTY or BRANCH
                if (map[row][col].flower.bush_placed == TRUE) {
                    map[row][col].flower.true_count ++;
                    map[row][col].flower.prot_called_this_turn = TRUE;
                    if (map[row][col].flower.true_count == 2) {
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.true_count = 0;
                    }
                }
                if (map[row][col].flower.bush_placed == FALSE) {
                    // Place a bush at the weak point
                    // If a bush was placed on a branch, defend the flower 
                    //again and place another bush if possible
                    if (map[row][col - 1].type == BRANCH) {
                        check_player(row, col - 1, player_row, 
                        player_col, map);
                        map[row][col].flower.bush_placed = FALSE;
                        map[row][col].flower.prot_called_this_turn = FALSE;
                        flower_prot(row, col, player_row, player_col, map);
                    }       
                    else {          
                        check_player(row, col - 1, player_row, 
                        player_col, map);
                        map[row][col].flower.prot_called_this_turn = TRUE;
                        map[row][col].flower.bush_placed = TRUE;
                    }
                }
            }
        }
        else {
            map[row][col].flower.bush_placed = FALSE;
        }
    }
}

/*check_player: This function checks if the player is at a specific location 
and updates the map accordingly, usually by placing bushes.*/
void check_player(int row, int col, int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    if (row == player_row && col == player_col) {
    }
    else {
        map[row][col].type = BUSH;
    }
}
/*reset_protection_flags: This function resets protection flags for flowers, 
indicating that they have not been protected during the current turn.
*/
void reset_protection_flags(struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row < MAX_ROW; row++) {
        for (int col = 0; col < MAX_COL; col++) {
            if (map[row][col].type == FLOWER 
            && map[row][col].flower.state != DORMANT) {
                map[row][col].flower.prot_called_this_turn = FALSE;
            }
        }
    }
}
/*root_attack: This function initiates a root attack from flowers on the 
player's position, killing the player if they are within range.*/
void root_attack (int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    int start_row = player_row - 1;
    int end_row = player_row + 1;
    int start_col = player_col - 1;
    int end_col = player_col + 1;
        
    if (start_row < 0) {
        start_row = 0;
    }
    if (end_row > MAX_ROW - 1) {
        end_row = MAX_ROW - 1;
    }
    if (start_col < 0) {
        start_col = 0;
    }
    if (end_col > MAX_COL - 1) {
        end_col = MAX_COL - 1;
    }


    for (int row = start_row; row <= end_row; row++) {
        // Check if the row is within the array boundaries
        for (int col = start_col; col <= end_col; col++) {
            // Check if the column is within the array boundaries
            if (map[row][col].type == FLOWER 
            && map[row][col].flower.roots_rem > 0 
            && map[row][col].flower.state > DORMANT 
            && map[row][col].flower.state < ENERGETIC) {
                if (map[player_row][player_col].type != DANGER) {
                    map[row][col].flower.in_range = TRUE;
                    map[player_row][player_col].type = DANGER;
                    map[row][col].flower.has_attacked = TRUE;
                    map[row][col].flower.roots_rem -= 1;
                }
                if (map[row][col].flower.roots_rem == 0) {
                    map[row][col].flower.has_attacked = FALSE;
                }
            }
        }
    }
}
/*check_attack_or_defense: This function checks whether flowers should 
initiate an attack or apply defensive measures based on the player's 
proximity and their current state.*/
void check_attack_or_defense (int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row <= 10; row ++) {
        for (int col = 0; col <= 10; col ++) {
            //looping through to check if any flowers are able to attack
            if (map[row][col].type == FLOWER 
            && map[row][col].flower.roots_rem > 0 
            && map[row][col].flower.state > DORMANT 
            && map[row][col].flower.state < ENERGETIC) {
                //checking if player is in range of that particular flower
                if (is_within_range(row, col, player_row, player_col, 1)) {
                    if (map[player_row][player_col].type != DANGER) {
                        root_attack(player_row, player_col, map);
                        map[row][col].flower.prot_called_this_turn = TRUE;
                    }
                }
                //if not flower should protect itself
                else if (!is_within_range(row, col, player_row, 
                player_col, 1)) {
                    flower_prot(row, col, player_row, player_col, map);
                }
            }
            //if flower has no roots resort to defending itself
            else if (map[row][col].flower.state > DORMANT 
            && map[row][col].flower.state < ENERGETIC) {
                if (map[row][col].flower.has_attacked == FALSE) {
                    flower_prot(row, col, player_row, player_col, map);
                }
            }
        }
    }
}
/*is_within_range: This function checks if two positions on the map are within 
a specified range of each other.*/
int is_within_range(int row1, int col1, int row2, int col2, int range) {
    //finding the distance between the player and flower by subtracting row
    //and column points
    int row_diff = row1 - row2;
    int col_diff = col1 - col2;
    //if within one row and one column then they are within range
    return (row_diff >= -range && row_diff <= range) && 
    (col_diff >= -range && col_diff <= range);
}
/*chg_danger: This function updates the danger level on the map, indicating 
that the player has been caught by flowers, leading to a game over.
*/
void chg_danger (int player_row, int player_col, 
struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row <= MAX_ROW - 1; row ++) {
        for (int col = 0; col <= MAX_COL - 1; col ++) {
            //loops through map and converts danger tiles to root tiles
            if (map[row][col].type == DANGER 
            && player_row == row && player_col == col) {
                //if player is on a danger tile when being converted they lose
                map[row][col].type = ROOT;
                print_map(map, player_row, player_col);
                printf("The flowers have beaten us, and UNSW ");
                printf("is lost forever! \n");
                exit(0);
            }
            else if (map[row][col].type == DANGER) {
                map[row][col].type = ROOT;
            }
        }
    }
}

/**
 * Initialises the game map with empty tiles, setting the type of each tile to 
 * `EMPTY` and initialising flower-related properties.
 *
 * @param map: The 2D array of `struct tile` representing the game map 
 *
 * @returns None
 *
**/
void initialise_map(struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row < MAX_ROW; row++) {
        for (int col = 0; col < MAX_COL; col++) {
            map[row][col].type = EMPTY;
            map[row][col].flower.state = NONE;
        }
    }
}

/**
 * Prints the game map 
 * 
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < MAX_COL;j++) {
            printf("|");
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            } else if (map[i][j].type == EMPTY) {
                printf(" P ");
            } else if (map[i][j].type == DANGER) {
                printf("<P>");
            } else if (map[i][j].type == ROOT) {
                printf("<^>");
            }
        }

        printf("|\n");
    }

    for (int j = 0; j < MAX_COL; j++) {
        printf("+---");
    }
    printf("+\n");
}

/**
 * Helper function which prints the representation of a single tile 
 * based on its type. 
 *
 * @params tile: The `struct tile` to be printed.
 *
 * @returns None
**/
void print_tile(struct tile tile) {
    if (tile.type == EMPTY) {
        printf("   ");
    } else if (tile.type == BUSH) {
        printf("###");
    } else if (tile.type == BRANCH) {
        printf("_/-");
    } else if (tile.type == DANGER) {
        printf("< >");
    } else if (tile.type == ROOT) {
        printf("<^>");
    } else if (tile.type == POLLEN) {
        printf(":::");
    } else if (tile.type == FLOWER) {
        print_flower(tile.flower);
    }
}

/**
 * Helper function which prints a visual representation of the 
 * flower face based on its state.
 *
 * @params flower: The `struct flower` containing the state of the flower.
 *
 * @returns None
 *
**/
void print_flower(struct flower flower) {
    if (flower.state == DORMANT) {
        printf("uwu");
    } else if (flower.state == DISTURBED) {
        printf("uwo");
    } else if (flower.state == AWAKENED) {
        printf("owo");
    } else if (flower.state == ANXIOUS) {
        printf("o~o");
    } else if (flower.state == ENERGETIC) {
        printf("*w*");
    }
}

