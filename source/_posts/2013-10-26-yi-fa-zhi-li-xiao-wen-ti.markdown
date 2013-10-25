---
layout: post
title: "一发智力小问题"
date: 2013-10-26 09:48
comments: true
categories: 
---

前几天从同事那里听到了一个面试的小智力题，来给大家分享一下
> 翻转硬币的游戏。有一张方形的桌子，桌子四角放了四枚一模一样的硬币。在每一轮操作中，玩家可以任意反转这些硬币。一轮操作之后
> 桌子会被水平旋转90度的倍数，但是玩家并不知道它被转了多少度。某一轮操作之后所有硬币都是正面或者都是反面朝上，则玩家获胜，游戏结束。
> 求一个必胜策略。

提问者（一个印度人）说这个问题是面试题，大家五分钟之内就能想出来答案。我只能表示智商需要充值了。


<!-- more -->
-------
<center>最喜欢分割线了</center>
-------

看到这个问题，我第一个反映是非常naive的“真的有必胜策略吗？好神奇的样子。”仔细考虑之。因为每次操作之后桌子都会被随机旋转，所以
必胜策略肯定不能基于硬币的绝对位置。于是循环移动后相等的序列应该被认为是同一个序列。这样4个硬币的排列只能有三种不同的序列：

* ABAB
* AABB（和ABBA）
* AAAB（和AABA，ABAA，BAAA）

其中A代表正面，B代表反面。当然我排除了AAAA的初始情况。

观察一下，第一个序列最有意思，翻转0号位和2号位一定可以让这个序列变成AAAA。第二个和第三个序列经过翻转0，2号位之后变成了：

* BAAB（和BBAA）
* BABB（和BAAA，BBBA，AABA）

还是在他们原来的类别中。非常不错的性质。

接下来处理第二类的BBAA，粗暴地翻转0号位和1号位，得到结果

* ABAB（和AAAA）
* ABBB（和ABAA，AABA，BBBA）

第二类的一半变成了获胜的局面，游戏结束；另外一半变成了第一类。第三类依旧还是第三类。说到这里，下一步就很明显了，翻转0号和2号位。
第一步中已经说明翻转0号和2号位之后第三类还是第三类。接下来专心处理第三类。

第四步策略也很简单，直接翻转0号位。反转之后的情况是

* BBBB（和BBAA，BABA，ABBA）

变成了获胜、第一类和第二类。于是重复步骤1－3完成游戏。

好久不写题了，贴个代码验证一下想法吧。希望我能早日回归，做上Topcoder。

``` cpp Code for Verification
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
```