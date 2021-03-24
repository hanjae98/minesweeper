/**  
 * <minesweeper.h>
 * Minesweeper game header file
 *
 * Writer: Han Jaesung
 * 2021년 03월 22일 (월) 오후 03시 09분 24초
 * */

#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>

#define BLOCK				-1
#define MINE				10
#define BLANK				0
#define BLANK_OPEN			BLANK + 20
#define MINE_OPEN			MINE + 20
#define MINE_RATE			0.3
#define SWAP(A,B,T)			{	T = A;	A = B;	B = T;	}
#define oops(s)				{	perror(s);	exit(1);	}
#define DELAY				100			//	in miliseconds.
#define COLOR_RESTORE		"\033[0m"	
#define RED_COLOR			"\033[31m"
#define YELLOW_COLOR		"\033[93m"
#define GRAY_BACKGRND		"\033[100m"
#define YELLOW_BACKGRND		"\033[43m"
#define CYAN_BACKGRND		"\033[106m"

using namespace std;

typedef struct Point{
	int r, c;
}point;

					
void read_input(int **map, int n, point& p);
void update(int **map, point p, int n);
void randomization(point *A, int n);
void init_map(int **map, int n);
void print_testmap(int **map, int n);
void print_map(int **map, int n, point p, bool highlight, string color);
void end_game(bool user_win);
bool is_gameover(int **map, int n, point p);
void setting_backup(void);
void setting_restore(void);
void f(int signum);
