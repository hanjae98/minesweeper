/**  
 * <minesweeper.cpp>
 * A console-based minesweeper game
 *
 * Writer: Han Jaesung
 * 2021년 03월 24일 (수) 오후 12시 28분 54초
 * */

#include "minesweeper_header.h"

					/*	UP			DOWN		LEFT		RIGHT				*/						
point directions[] = {	{-1, 0},	{1, 0},		{0, -1},	{0, 1},
					/*	LEFT_UP		RIGHT_UP	LEFT_DOWN	RIGHT_DOWN			*/
						{-1, -1},	{-1, 1},	{1, -1},	{1, 1}		
};

struct termios oldval, newval;

int main(void){
	srand(time(NULL));
	int n;
	point p = {0, 0};
	cout <<	YELLOW_COLOR; 
	cout << "#######################" << endl;
	cout << "###   MINESWEEPER   ###" << endl;
	cout << "#######################" << endl;
	cout << COLOR_RESTORE;
	cout << "Game map would be n * n matrix" << endl;
	cout << "n?: ";
	cin >> n;
	setting_backup();
	int **map = new int* [n + 2];
	for(int i = 0; i < n + 2; i++)	
		map[i] = new int [n + 2];
	init_map(map, n);
//	print_testmap(map, n);

	while(true){
		print_map(map, n, p, false, "");
		read_input(map, n, p);
		/*if(p.r < 1 || p.r > n || p.c < 1 || p.c > n){
			system("clear");
			cout << RED_COLOR;
			cout << "#######################" << endl;
			cout << "###   INPUT ERROR   ###" << endl;
			cout << "#######################" << endl;
			cout << COLOR_RESTORE;
			continue;
		}*/
		update(map, p, n);
//		print_map(map, n);
		system("clear");
		if(is_gameover(map, n, p)){
			print_map(map, n, p, false, "");
			break;
		}
	}
	for(int i = 0; i < n + 2; i++)
		delete[] map[i];
	delete[] map;
	setting_restore();
	return 0;
}
