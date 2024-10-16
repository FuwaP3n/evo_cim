#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include <math.h>

#define SIZE 100
#define PIXEL 10
#define DNA_LENGTH 64
#define MUTATION 75
#define WALL 1


#define ST_ENERGY 50

struct bacteria{
	int dna[DNA_LENGTH];
	int energy;
	int is_alive;
	int command;
	int type;
	int was;
	int age;
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
			bac[x][y].age = 0;
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


int mov(struct bacteria bac[SIZE][SIZE], int x, int y, int act){
	if(bac[x][y].type==0){
		return 0;
	}
	
	int to[2];
	side(act, to);
	
	bac[x][y].command++;
	bac[x][y].energy--;
	if(bac[x][y].command >= DNA_LENGTH){ bac[x][y].command = bac[x][y].command - DNA_LENGTH; }
	bac[x][y].was = 1;
	if(WALL==0){
		if(x+to[0]<0){ to[0] = SIZE-1; }
		else if(x+to[0]>=SIZE){ to[0] = -SIZE+1;}

		if(x+to[0]<0){ to[0] = SIZE-1; }
		else if(x+to[0]>=SIZE){ to[0] = -SIZE+1;}
	} else {
		if(x+to[0]<0||x+to[0]>=SIZE){ return 0;}
		if(y+to[1]<0||y+to[1]>=SIZE){ return 0;}
	}


	if(bac[x][y].type==1&&bac[x+to[0]][y+to[1]].type==0&&bac[x+to[0]][y+to[1]].is_alive==1){
		bac[x][y].energy = bac[x][y].energy+bac[x+to[0]][y+to[1]].energy;
		bac[x+to[0]][y+to[1]].is_alive = 0;
	}
	if(bac[x+to[0]][y+to[1]].is_alive==0){
		bac[x+to[0]][y+to[1]] = bac[x][y];
		bac[x][y].is_alive = 0;	
	}

}

void divide(struct bacteria bac[SIZE][SIZE], int x, int y, int act){
	int add_o = 0;
	int add_p = 0;
	int to[2];
	int skip = 0;
	side(act, to);
	if(bac[to[0]][to[1]].is_alive==0){
		skip++;
	}
	for(int o=-1; o<2; o++){
		for(int p=-1; p<2; p++){
			if(WALL==0){
				if(x+o<0){add_o=SIZE-1;}
				else if(x+o>=SIZE){ add_o=-SIZE+1; }
				if(y+p<0){add_p=SIZE-1;}
				else if(y+p>=SIZE){ add_p=-SIZE+1; }
			} else {
				if(x+o<0){continue;}
				else if(x+o>=SIZE){ continue; }
				if(y+p<0){continue;}
				else if(y+p>=SIZE){ continue; }
			
			}

			if(bac[x+o+add_o][y+p+add_p].is_alive==0){
				
				if(skip==1){
					o = x+to[0]; 
					p = x+to[0];
					if(o<0){ o=o+SIZE;}
					else if(o>=SIZE){o=o-SIZE;}
					if(p<0){ p=p+SIZE;}
					else if(p>=SIZE){p=p-SIZE;}
				} else {
					o = add_o+x+o;
					p = p+add_p+y;
				}
				bac[o][p].is_alive = 1;
				bac[o][p].energy = bac[x][y].energy/2;
				bac[x][y].energy = bac[o][p].energy;
				bac[o][p].was = 1;
				bac[o][p].command = 0;
				bac[o][p].age = 0;
				
				//Change type chance
				if(rand()%100>95){
					bac[o][p].type = bac[x][y].type+1;
					if(bac[o][p].type>2){ bac[o][p].type=2;}
				} else {
					bac[o][p].type = bac[x][y].type;
				}
				//Chance of mutation
				for(int i=0; i<DNA_LENGTH; i++){
					if(rand()%100>99-MUTATION){
						bac[o][p].dna[i]=rand()%32;
					} else { bac[o][p].dna[i] = bac[x][y].dna[i]; }
				}
				o = 99;
				p = 99;
			}
		}
	}
}

int grab(struct bacteria bac[SIZE][SIZE], int x, int y, int act){
	int to[2];
	side(act, to);
	if(WALL==0){
		if(x+to[0]<0){ to[0] = SIZE-1; }
		else if(x+to[0]>=SIZE){ to[0] = -SIZE+1;}

		if(x+to[0]<0){ to[0] = SIZE-1; }
		else if(x+to[0]>=SIZE){ to[0] = -SIZE+1;}
	} else {
		if(x+to[0]<0||x+to[0]>=SIZE){ return 0;}
		if(y+to[1]<0||y+to[1]>=SIZE){ return 0;}
	}

	

	switch(bac[x][y].type){
		case 0:
			bac[x][y].energy=bac[x][y].energy+5;
			break;	
		case 1:
			if(bac[x+to[0]][y+to[1]].type==0&&bac[x+to[0]][y+to[1]].is_alive==1){
				bac[x][y].energy = bac[x+to[0]][y+to[1]].energy + bac[x][y].energy;
				bac[x+to[0]][y+to[1]].is_alive = 0;
			}
			break;
		case 2:
			if(bac[x+to[0]][y+to[1]].type==1&&bac[x+to[0]][y+to[1]].is_alive==1){
				bac[x][y].energy = bac[x+to[0]][y+to[1]].energy + bac[x][y].energy;
				bac[x+to[0]][y+to[1]].is_alive = 0;
			}
			break;
		default:
			break;
	}

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
		if(bac[x][y].energy>1){ divide(bac, x, y, action-8);}
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
	int besties = 10;
	int alive = 0;
	struct bacteria best[besties];
	struct bacteria map[SIZE][SIZE];
	for(int i=0; i<10; i++){
		init_bac(map);
		alive++;
	}


	InitWindow(SIZE*PIXEL, SIZE*PIXEL, "Epsteins Island");
	SetTargetFPS(10);
	//EnableEventWaiting();
	while(1){
		if(WindowShouldClose()) {break;}
		BeginDrawing();
		ClearBackground(BLACK);
		//show_grid();
		bac_draw(map);
		EndDrawing();
		if(alive == 0){
			FILE * data = fopen("data", "w");
			fwrite(best, sizeof(struct bacteria), besties, data);
			fclose(data);
			break;
		}
		alive = 0;
		for(int i=0; i<2; i++){
			for(int x=0; x<SIZE; x++){
				for(int y=0; y<SIZE; y++){
					if(i==0){
						if(map[x][y].is_alive==1){
							if(map[x][y].was==0){
								struct bacteria best_buf[besties];
								
								for(int j=0; j<besties; j++){
									best_buf[j] = best[j];
								}
								int ch = 0;
								for(int j=0; j<besties; j++){
									if(ch==0){
										if(best_buf[j].age<map[x][y].age){
											best[j] = map[x][y];
											ch++;
										}
									} else {
										best[j] = best_buf[j-1];
									}
								}
								map[x][y].age++;
								turn(map, x, y, 0);
							}
						}	
					} else {
						if(map[x][y].energy<1){
							map[x][y].is_alive = 0;
						}
						if(map[x][y].energy>200){
							map[x][y].energy = 200;
						}
						if(map[x][y].is_alive==1){
							alive++;
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
