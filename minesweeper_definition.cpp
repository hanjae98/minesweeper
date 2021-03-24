/**  
 * <minesweeper_definition.cpp>
 * Define functions
 *
 * Writer: Han Jaesung
 * 2021년 03월 24일 (수) 오후 12시 29분 17초
 * */

#include "minesweeper_header.h"

extern struct termios oldval, newval;
extern point directions[];

void f(int signum){
	cout << "== restore the setting ==" << endl;
	setting_restore();	
	exit(0);
}

void setting_restore(void){
    if(tcsetattr(STDIN_FILENO, TCSANOW, &oldval) == -1)
		oops("tcsetattr");
}

void setting_backup(void){
	/*	backup the current setting	*/
	if(tcgetattr(STDIN_FILENO, &oldval) == -1) 
		oops("tcgetattr");
	newval = oldval;
	newval.c_lflag &= ~ECHO;
    newval.c_lflag &= ~ICANON;
	
	/*	set the terminal non-blocking mode	*/
    if(tcsetattr(STDIN_FILENO, TCSANOW, &newval) == -1)
		oops("tcsetattr");

	signal(SIGINT, f);
}

void read_input(int **map, int n, point& p){
	char key;
	string color = CYAN_BACKGRND; 
	while( (key = getchar()) ){
		if(map[p.r][p.c] >= BLOCK && map[p.r][p.c] <= MINE && key == '\n')	
			break;
		system("clear");
		switch(key){
			case 'w':	// UP
				if(p.r > 1)	p.r--;
				break;
			case 'a':	// LEFT
				if(p.c > 1)	p.c--;
				break;
			case 's':	// DOWN
				if(p.r < n) p.r++;
				break;
			case 'd':	// RIGHT
				if(p.c < n) p.c++;
				break;
			default:
				break;
		}
		print_map(map, n, p, true, color);
	}
}

void update(int **map, point p, int n){
	if(map[p.r][p.c] == MINE){
		for(int r = 1; r <= n; r++){
			for(int c = 1; c <= n; c++){
				if(map[r][c] == MINE)
					map[r][c] = MINE_OPEN;
			}
		}
	}else if(map[p.r][p.c] >= BLANK && map[p.r][p.c] < MINE){	
		queue<point> Q;
		point dir;
		if(map[p.r][p.c] > BLANK){				/*	not a blank	*/
			Q.push(p);
			map[p.r][p.c] += 20;
		}else if(map[p.r][p.c] == BLANK){								/*	blank		*/
			/*		BFS		*/
			Q.push(p);
			map[p.r][p.c] = BLANK_OPEN;
			while(!Q.empty()){
				point e = Q.front();
				for(int i = 0; i < 4; i++){
					dir = directions[i];
					dir.r += e.r;
					dir.c += e.c;
					if(map[dir.r][dir.c] == BLANK){
						Q.push(dir);
						map[dir.r][dir.c] = BLANK_OPEN;
					}else if(map[dir.r][dir.c] > BLANK && map[dir.r][dir.c] < MINE){
						map[dir.r][dir.c] += 20;
					}
				}
				system("clear");
				print_map(map, n, p, false, "");
//				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				usleep(DELAY * 1000);
				system("clear");
				Q.pop();
			}
		}
	}
}

void randomization(point *A, int n){
	int n_randomized = 0, n_origin = n * n - 1;
	int idx;
	point temp;
	while(n_randomized < n * n - 2){
		idx = rand() % n_origin; 
		if(idx != n_origin)
			SWAP(A[idx], A[n_origin], temp);
		n_randomized++;
		n_origin--;
	}
}

/* initialization */
void init_map(int **map, int n){

	/*	create a 2-dim. array of points	*/
	point *random_indices = new point [n * n];
	int row, col, mine, n_mines, rand_idx = 0;
	point p;
	row = col = 1;

	/*	initialize all points	*/
	/*	(1,1), ..., (n,n)		*/
	for(int i = 0; i < n * n; i++){
		random_indices[i].r = row;
		random_indices[i].c = col;
		col++;
		if(col == n + 1){
			row++;
			col = 1;
		}
	}

	n_mines = MINE_RATE * n * n;
	for(int r= 0; r < n + 2; r++)
		for(int c = 0; c < n + 2; c++)
			if(r == 0 || r == n + 1 || c == 0 || c == n + 1){
				map[r][c] = BLOCK;
			}else{
				/* randomization of (1,1) ~ (n,n) */
				randomization(random_indices, n);
				/* rand() % 2 */
				mine = rand() % 2;
				/* if 1, map[r][c] = MINE, else map[r][c] = BLANK */
				p = random_indices[rand_idx++];
				if(mine && n_mines > 0){
					map[p.r][p.c] = MINE;
					n_mines--;
				}else{
					map[p.r][p.c] = BLANK; 
				}
			}
	/*	free a index array	*/
	delete[] random_indices;

	/* record how many mines are near the blank */
	for(int r = 1; r <= n; r++){
		for(int c = 1; c <= n; c++){
			if(map[r][c] != MINE){
				for(int i = 0; i < 8; i++){
					p = directions[i];
					if(map[r + p.r][c + p.c] == MINE)
						map[r][c]++;
				}
			}
		}
	}
}

void print_testmap(int **map, int n){
	for(int r = 0; r < n + 2; r++){
		for(int c = 0; c < n + 2; c++){
			if(r == 0 || r == n + 1 || c == 0 || c == n + 1){	// edge
				cout << "■ ";
				continue;
			}
			if(map[r][c] == MINE){
				cout << "# ";
			}else if(map[r][c] == BLANK){
				cout << ". ";
			}else{
				cout << map[r][c] << " ";
			}
		}
		cout << endl;
	}
}


void print_map(int **map, int n, point p, bool highlight, string color){
	for(int r = 0; r < n + 2; r++){
		for(int c = 0; c < n + 2; c++){
			if(r == 0 || r == n + 1 || c == 0 || c == n + 1){	// edge
				cout << "■ ";
				continue;
			}
			if(r == p.r && c == p.c && highlight)
				cout << color;

			if(map[r][c] < 20){		/*	NOT OPEN	*/
				cout << "□ ";
			}else{
				if(map[r][c] == MINE_OPEN){
					cout << RED_COLOR << "# " << COLOR_RESTORE;
				}else if(map[r][c] == BLANK_OPEN){
					cout << ". ";
				}else{
					cout << map[r][c] - 20 << " ";
				}
			}
			if(r == p.r && c == p.c && highlight)
				cout << COLOR_RESTORE;
			
		}
		cout << endl;
	}
}

void end_game(bool user_win){
	cout << RED_COLOR;
	cout << "#######################\n";
	user_win == true? cout << "###   YOU WIN!!!    ###" : cout << "###   YOU LOSE...   ###";
	cout << "\n#######################\n";
	cout << COLOR_RESTORE << endl;
}

bool is_gameover(int **map, int n, point p){
	bool user_win = true;
	if(map[p.r][p.c] == MINE_OPEN){
		user_win = false;
		end_game(user_win);
		return true;
	}
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= n; j++){
			if( (map[i][j] >= BLANK_OPEN && map[i][j] <= MINE_OPEN) 
					|| map[i][j] == MINE ){
				user_win = user_win && true;
			}else{
				return false;
			}
		}
	}
	/*	user win	*/
	end_game(user_win);
	return true;
}
