#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include <math.h>

#define SIZE 100
#define PIXEL 10
#define DNA_LENGTH 64

#define ST_ENERGY 50

struct bacteria{
	int dna[DNA_LENGTH];
	int energy;
	int is_alive;
	int command;
	int type;
	int was;
};

void show_grid(){
	for(int x=0; x<SIZE; x++){
		DrawRectangle(x*PIXEL-1, -1, 2, SIZE*PIXEL+1, DARKGRAY);
	}
	for(int y=0; y<SIZE; y++){
		DrawRectangle(-1, y*PIXEL-1, SIZE*PIXEL+1, 2, DARKGRAY);
	}
}

int bac_draw(struct bacteria bac[SIZE][SIZE]){
	int margin = 2;
	for(int x=0; x<SIZE; x++){
		for(int y=0; y<SIZE; y++){
			if(bac[x][y].is_alive==1){
				switch(bac[x][y].type){
					case 0:
						DrawRectangle(x*PIXEL+margin, y*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, GREEN);					
						break;
					case 1:
						DrawRectangle(x*PIXEL+margin, y*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, BROWN);
						break;
					case 2:
						DrawRectangle(x*PIXEL+margin, y*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, RED);
						break;
					default:
						DrawRectangle(x*PIXEL+margin, y*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, LIGHTGRAY);

						break;
				}
			}
		}
	}
}


void init_bac(struct bacteria bac[SIZE][SIZE]){
	while(1){
		int x = rand()%SIZE;
		int y = rand()%SIZE;
		if(bac[x][y].is_alive == 0){
			bac[x][y].energy = ST_ENERGY;
			bac[x][y].is_alive = 1;
			bac[x][y].command = 0;
			bac[x][y].type = rand()%3;
			bac[x][y].was = 0;
			for(int i=0; i<DNA_LENGTH; i++){
				bac[x][y].dna[i] = rand()%32;
			}
			break;
		}
	}
}
void side(int action, int *pos){
	for(int x=-1; x<2; x++){
		for(int y=-1; y<2; y++){
			if(x==0&&y==0){ continue; }
			action--;
			if(action<0){
				pos[0] = x;
				pos[1] = y;
			}
		}
		if(action<0){
			break;
		}
	}

}


void mov(struct bacteria bac[SIZE][SIZE], int x, int y, int act){
	int to[2];
	side(act, to);
	
	bac[x][y].command++;
	bac[x][y].energy--;
	if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command = bac[x][y].command - DNA_LENGTH; }
	bac[x][y].was = 1;
	
	if(x+to[0]<0){ to[0] = SIZE-1; }
	else if(x+to[0]==SIZE){ to[0] = -SIZE+1;}

	if(x+to[0]<0){ to[0] = SIZE-1; }
	else if(x+to[0]==SIZE){ to[0] = -SIZE+1;}
		
	if(bac[x+to[0]][y+to[1]].is_alive==0){
		bac[x+to[0]][y+to[1]] = bac[x][y];
		bac[x][y].is_alive = 0;	
	}

}

void divide(struct bacteria bac[SIZE][SIZE], int x, int y, int act){
}

void grab(struct bacteria ba[SIZE][SIZE], int x, int y, int act){
}

void turn(struct bacteria bac[SIZE][SIZE], int x, int y, int sun){
	int action = bac[x][y].dna[bac[x][y].command];
	if(action<8){
		if(bac[x][y].type!=0){
			mov(bac, x, y, action);
		}else {
			bac[x][y].energy--;
			bac[x][y].command++;
			if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command=0;}
			bac[x][y].was = 1;
		}
	} else if(action<16){
		if(bac[x][y].energy>1){ divide(bac, x, y, action-8);
	       	bac[x][y].energy--;}
		else { bac[x][y].energy--; }
		bac[x][y].command++;
		bac[x][y].was = 1;
		if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command = bac[x][y].command-DNA_LENGTH; }
	} else if(action<24){
		grab(bac, x, y, action-16);
		bac[x][y].command++;
		bac[x][y].energy--;
		bac[x][y].was =1;
		if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command = bac[x][y].command-DNA_LENGTH; }
	} else {
		bac[x][y].command = bac[x][y].command+action-24;
		bac[x][y].energy--;
		bac[x][y].was = 1;
		if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command = bac[x][y].command-DNA_LENGTH; }
	}
}


int main(){
	//initialize
	srand(time(NULL));
	struct bacteria map[SIZE][SIZE];
	for(int i=0; i<10; i++){
		init_bac(map);
	}


	InitWindow(SIZE*PIXEL, SIZE*PIXEL, "Epsteins Island");
	SetTargetFPS(30);
	//EnableEventWaiting();
	while(1){
		if(WindowShouldClose()) {break;}
		BeginDrawing();
		ClearBackground(BLACK);
		show_grid();
		bac_draw(map);
		EndDrawing();
		for(int i=0; i<2; i++){
			for(int x=0; x<SIZE; x++){
				for(int y=0; y<SIZE; y++){
					if(i==0){
						if(map[x][y].is_alive==1){
							if(map[x][y].was==0){
								turn(map, x, y, 0);
							}
						}	
					} else {
						if(map[x][y].energy<1){
							map[x][y].is_alive = 0;
						}
						map[x][y].was=0;
					}

				}
			}
		}
	}
	CloseWindow();

	return 0;
}
