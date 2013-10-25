//
//  main.cpp
//  TryXcode
//
//  Created by ditsing on 10/19/13.
//  Copyright (c) 2013 ditsing. All rights reserved.
//

#include <iostream>
#include <cstring>

using namespace std;
const int STEPS_L = 7;
int steps[STEPS_L][4] = {
    { 1, 0, 1, 0},
    { 1, 1, 0, 0},
    { 1, 0, 1, 0},
    { 1, 0, 0, 0},
    { 1, 0, 1, 0},
    { 1, 1, 0, 0},
    { 1, 0, 1, 0},
};

int coins[4];
int table[STEPS_L+1][4];
int tmp_table[4];
int rotat[STEPS_L];

int fail_count;

bool check_win( int coins[])
{
    return coins[0] == coins[1] && coins[1] == coins[2] &&
    coins[2] == coins[3];
}

int search_coins( int step)
{
    if ( step == STEPS_L)
    {
        printf("Fails on Initial situation of %d %d %d %d.\n",
               coins[0], coins[1], coins[2], coins[3]);
        printf("Ends up with %d %d %d %d.\n",
               table[step][0], table[step][1], table[step][2], table[step][3]);
        printf("With rotation as %d %d %d %d %d %d.\n",
               rotat[0], rotat[1], rotat[2], rotat[3],
               rotat[4], rotat[5]);

        return 1;
    }

    for ( int i( 0); i < 4; i++)
    {
        table[step+1][i] = table[step][i] ^ steps[step][i];
    }

    if ( check_win( table[step+1]))
    {
        printf("Wins on Initial situation of %d %d %d %d.\n",
               coins[0], coins[1], coins[2], coins[3]);
        printf("Ended up with %d %d %d %d at step %d.\n",
               table[step+1][0], table[step+1][1], table[step+1][2], table[step+1][3],
               step + 1);
        printf("With rotation as %d %d %d %d %d %d %d.\n",
               rotat[0], rotat[1], rotat[2], rotat[3],
               rotat[4], rotat[5], rotat[6]);
        printf("\n");

        return 0;
    }

    int fail_count = 0;
    for ( int ro = 0; ro < 2; ro++)
    {
        rotat[step] = ro;
        for ( int i( 0); i < 4; i++)
        {
            tmp_table[(i+ro)&3] = table[step+1][i];
        }
        memcpy( table[step+1], tmp_table, sizeof( tmp_table));
        fail_count += search_coins( step + 1);
    }
    rotat[step] = -1;

    return fail_count;
}

int main(int argc, const char * argv[])
{
    fail_count = 0;
    for ( coins[0] = 0; coins[0] < 2; coins[0]++)
    {
        for ( coins[1] = 0; coins[1] < 2; coins[1]++)
        {
            for ( coins[2] = 0; coins[2] < 2; coins[2]++)
            {
                for ( coins[3] = 0; coins[3] < 2; coins[3]++)
                {
                    memcpy( table, coins, sizeof( coins));
                    memset( rotat, 255, sizeof( rotat));
                    if ( !check_win( coins))
                    {
                        fail_count += search_coins( 0);
                    }
                }
            }
        }
    }

    printf("Fails on %d cases.\n", fail_count);
    return 0;
}

