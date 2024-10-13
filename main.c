#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>
#include <math.h>


#define SIZE 100
#define PIXEL 10
#define FIRST_FAM 10


void show_grid(){
	for(int x=0; x<SIZE; x++){
		DrawRectangle(x*PIXEL-1, -1, 2, SIZE*PIXEL+1, DARKGRAY);
	}
	for(int y=0; y<SIZE; y++){
		DrawRectangle(-1, y*PIXEL-1, SIZE*PIXEL+1, 2, DARKGRAY);
	}
}


struct bacteria{
	int dna[64];
	int pos[2];
	int aim[2];
	int energy;
	int type;
	int is_alive;
	int command;	
};


void init_dna(int *dna, int length){
	for(int i=0; i<length; i++){
		dna[i] = rand()%4;
	}
}


void init_bacteria(struct bacteria * bac, int max_alive){
	for(int i=0; i<max_alive; i++){
		if(bac[i].is_alive != 1){
			init_dna(bac[i].dna, 64);
			bac[i].pos[0] = rand()%SIZE;
			bac[i].pos[1] = rand()%SIZE;
			bac[i].energy = 100;
			bac[i].type = rand()%3;
			bac[i].is_alive = 1;
			bac[i].command = 0;
			break;
		}
	}
}

void draw_em_all(struct bacteria * bac, int max_alive, int living){
	int margin = 2;
	for(int i=0; i<max_alive; i++){
		if(living==0){break;}
		if(bac[i].is_alive==1){
			DrawRectangle(bac[i].pos[0]*PIXEL+margin, bac[i].pos[1]*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, RED);
			living--;
		}
	}
}

void draw_sun(int x){
	DrawCircleGradient(x*PIXEL+PIXEL/2, SIZE/2*PIXEL+PIXEL/2, SIZE/2*PIXEL, YELLOW, BLANK);
}

void mov_bac(struct bacteria * bac, int ptr, int living){
	int max_living = SIZE*SIZE;
	int pos_x = bac[ptr].pos[0] + (rand()%3-1);
	int pos_y = bac[ptr].pos[1] + (rand()%3-1);

	if(pos_x>=SIZE){ pos_x = 0;}
	else if(pos_x<0){ pos_x = SIZE-1;}

	if(pos_y>=SIZE){ pos_y = 0;}
	else if(pos_y<0){ pos_y = SIZE-1;}

	for(int i=0; i<max_living; i++){
		if(living==0){ break;}
		if(bac[i].is_alive){
			living--;
			if(bac[i].pos[0]!=pos_x && bac[i].pos[1]!=pos_y){
				bac[ptr].pos[0] = pos_x;
				bac[ptr].pos[1] = pos_y;
				break;
			}
		}
	}
}

void mov_dir(struct bacteria * bac, int ptr, int living){
	int max_living = SIZE*SIZE;
	int pos_x = bac[ptr].pos[0];
	int pos_y = bac[ptr].pos[1];
	int aim_x = bac[ptr].aim[0];
	int aim_y = bac[ptr].aim[1];
	
	int mov_x;
	int mov_y;
	
	/*
	if(aim_x-pos_x<0){ mov_x=-1;}
	else if(aim_x-pos_x>0){ mov_x=1;}
	else { mov_x=0;}
	
	if(aim_y-pos_y<0){ mov_y=-1;}
	else if(aim_y-pos_y>0){ mov_y=1;}
	else { mov_y=0;}
	*/
	
	if(abs(aim_x-pos_x)>abs(aim_x+SIZE-pos_x)){ 
		if(aim_x+SIZE-pos_x<0){ mov_x=pos_x-1;}
		else if(aim_x+SIZE-pos_x>0){ mov_x=pos_x+1;}
		else { mov_x=pos_x;}
	}
	else if(abs(aim_x-pos_x)>abs(aim_x-SIZE-pos_x)){ 
		if(aim_x-SIZE-pos_x<0){ mov_x=pos_x-1;}
		else if(aim_x-SIZE-pos_x>0){ mov_x=pos_x+1;}
		else { mov_x=pos_x;}
	}
	else { 
		if(aim_x-pos_x<0){ mov_x=pos_x-1;}
		else if(aim_x-pos_x>0){ mov_x=pos_x+1;}
		else { mov_x=pos_x;}
	}
	
	if(abs(aim_y-pos_y)>abs(aim_y+SIZE-pos_y)){ 
		if(aim_y+SIZE-pos_y<0){ mov_y=pos_y-1;}
		else if(aim_y+SIZE-pos_y>0){ mov_y=pos_y+1;}
		else { mov_y=pos_y;}
	}
	else if(abs(aim_y-pos_y)>abs(aim_y-SIZE-pos_y)){ 
		if(aim_y-SIZE-pos_y<0){ mov_y=pos_y-1;}
		else if(aim_y-SIZE-pos_y>0){ mov_y=pos_y+1;}
		else { mov_y=pos_y;}
	}
	else { 
		if(aim_y-pos_y<0){ mov_y=pos_y-1;}
		else if(aim_y-pos_y>0){ mov_y=pos_y+1;}
		else { mov_y=pos_y;}
	}

	if(mov_x>=SIZE){ mov_x = 0;}
	else if(mov_x<0){ mov_x = SIZE-1;}

	if(mov_y>=SIZE){ mov_y = 0;}
	else if(mov_y<0){ mov_y = SIZE-1;}
	
	int wall = 0;
	for(int i=0; i<max_living; i++){
		if(living==0){ break;}
		if(bac[i].is_alive==1){
			living--;
			if(bac[i].pos[0]==mov_x && bac[i].pos[1]==mov_y){
				wall++;
			}
		}
	}
	if(wall==0){
		bac[ptr].pos[0] = mov_x;
		bac[ptr].pos[1] = mov_y;
	}

}



void find_aim(struct bacteria * bac, int ptr, int living, int sun){
	int max_living = SIZE*SIZE;
	int best[2] = {SIZE*10, SIZE*10};
	switch(bac[ptr].type){
		case 0:
			bac[ptr].aim[0]=sun;
			bac[ptr].aim[1]=SIZE/2;
			break;
		case 1:
			for(int i=0; i<max_living; i++){
				if(bac[i].is_alive == 1){
					if(living==0){ break;}
					if(bac[i].type == 0){
						if(pow(bac[i].pos[0]-bac[ptr].pos[0],2)+pow(bac[i].pos[1]-bac[ptr].pos[1],2)<pow(best[0],2)+pow(best[1],2)){
							best[0]=bac[i].pos[0];
							best[1]=bac[i].pos[1];
						}
					}
					living--;
				}
			}
			bac[ptr].aim[0] = best[0];
			bac[ptr].aim[1] = best[1];
			break;
		case 2:
			for(int i=0; i<max_living; i++){
				if(bac[i].is_alive == 1){
					if(living==0){ break;}
					if(bac[i].type == 1){
						if(pow(bac[i].pos[0]-bac[ptr].pos[0],2)+pow(bac[i].pos[1]-bac[ptr].pos[1],2)<pow(best[0],2)+pow(best[1],2)){
							best[0]=bac[i].pos[0];
							best[1]=bac[i].pos[1];
						}
					}
					living--;
				}
			}
			bac[ptr].aim[0] = best[0];
			bac[ptr].aim[1] = best[1];
			break;
		default:
			break;
	
	}

}

void consume(struct bacteria * bac, int ptr, int living, int sun){
	int max_living = SIZE*SIZE;
	
	switch(bac[ptr].type){
		case 0:
			int k = 1;
			int max=10;
			bac[ptr].energy = bac[ptr].energy + (sqrt(pow(sun-bac[ptr].pos[0],2)-pow(SIZE/2-bac[ptr].pos[1],2))/max);
			break;
		case 1:
			for(int i=0; i<max_living; i++){
				if(living==0){ break;}
				if(bac[i].is_alive==1){
					living--;
					if(bac[i].type!=0){ continue;}
					if(pow(bac[i].pos[0]-bac[ptr].pos[0], 2)+pow(bac[i].pos[1]-bac[ptr].pos[1], 2)<2){
					bac[i].is_alive=0;
					bac[ptr].energy = bac[ptr].energy+bac[i].energy;
					break;
					}
				}
			}
			break;
		case 2:
			for(int i=0; i<max_living; i++){
				if(living==0){ break;}
				if(bac[i].is_alive==1){
					living--;
					if(bac[i].type!=1){ continue;}
					if(pow(bac[i].pos[0]-bac[ptr].pos[0], 2)+pow(bac[i].pos[1]-bac[ptr].pos[1], 2)<2){
					bac[i].is_alive=0;
					bac[ptr].energy = bac[ptr].energy+bac[i].energy;
					bac[i].energy = 0;
					break;
					}
				}
			}
			break;
	}
}

void turn(struct bacteria * bac, int * living, int sun){
	int max_living = SIZE*SIZE;
	int still = *living;
	for(int pointer=0; pointer<max_living; pointer++){
		if(still==0){break;}
		if(bac[pointer].is_alive==1){
			still--;
			bac[pointer].energy--;
			if(bac[pointer].energy<1){
				bac[pointer].is_alive=0;
				*living--;
				continue;
			}
			switch(bac[pointer].dna[bac[pointer].command]){
				case 0:
					break;
				case 1:
					consume(bac, pointer, *living, sun);
					break;
				case 2:
					if(bac[pointer].aim[0]){
						mov_dir(bac, pointer, *living);
					}
					else { mov_bac(bac, pointer, *living); }
					break;
				case 3:
					find_aim(bac, pointer, *living, sun);
					break;
				default:
					break;
			}
			bac[pointer].command++;
			if(bac[pointer].command>63){ bac[pointer].command = 0; }
		}
	}
}


int main(){
	int GodIsntAngry = 1;
	int sun_pos = 10;

	srand(time(NULL));
	
	int max_entity = SIZE*SIZE;
	struct bacteria entities[max_entity];
	
	int alive = 0;
	for(int i=0; i<FIRST_FAM; i++){
		init_bacteria(entities, max_entity);
		alive++;
	}

	InitWindow(SIZE*PIXEL, SIZE*PIXEL, "Epstein Island Sim");
	SetTargetFPS(10);
	entities[0].type =2;
	

	while(GodIsntAngry){
		if(WindowShouldClose()){GodIsntAngry=0; }
		turn(entities, &alive, sun_pos);	
		
		BeginDrawing();
		ClearBackground(BLACK);
		draw_em_all(entities, max_entity, alive);
		draw_sun(sun_pos);
		draw_sun(sun_pos-SIZE);
		draw_sun(sun_pos+SIZE);
		show_grid();
		EndDrawing();
		sun_pos++;
		if(sun_pos>SIZE){sun_pos=sun_pos-SIZE;}
	}
	CloseWindow();
	return 0;
}
