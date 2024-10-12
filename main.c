#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>

#define SIZE 20
#define PIXEL 10

struct bacteria{
	int dna[64];
	int pos[2];
	int aim[2];
	int energy;
	int type;
	int is_alive;	
};


void init_dna(int *dna, int length){
	for(int i=0; i<length; i++){
		dna[i] = rand()%4;
		printf("%d", dna[i]);
	}
	printf("\n");
}


void init_bacteria(struct bacteria * bac, int max_alive){
	for(int i=0; i<max_alive; i++){
		if(bac[i].is_alive != 1){
			init_dna(bac[i].dna, 64);
			bac[i].pos[0] = rand()%SIZE;
			bac[i].pos[1] = rand()%SIZE;
			bac[i].energy = 20;
			bac[i].type = rand()%3;
			bac[i].is_alive = 1;
			break;
		}
	}
}

void draw_em_all(struct bacteria * bac, int max_alive){
	int margin = 1;
	for(int i=0; i<max_alive; i++){
		if(bac[i].is_alive==1){
			DrawRectangle(bac[i].pos[0]*PIXEL+margin, bac[i].pos[1]*PIXEL+margin, PIXEL-margin*2, PIXEL-margin*2, RED);
		}
	}
}

void draw_sun(int x){
	DrawCircleGradient(x*PIXEL+PIXEL/2, SIZE/2*PIXEL+PIXEL/2, SIZE/2*PIXEL, YELLOW, BLANK);
}

int main(){
	int GodIsntAngry = 1;
	int sun_pos = 10;

	srand(time(NULL));
	struct bacteria entities[64];
	int alive = 0;
	for(int i=0; i<4; i++){
		init_bacteria(entities, 64);
		alive++;
	}

	InitWindow(SIZE*PIXEL, SIZE*PIXEL, "Epstein Island Sim");
	SetTargetFPS(10);

	while(GodIsntAngry){
		if(WindowShouldClose()){GodIsntAngry=0; }
		BeginDrawing();
		ClearBackground(BLACK);
		draw_em_all(entities, 64);
		draw_sun(sun_pos);
		draw_sun(sun_pos-SIZE);
		draw_sun(sun_pos+SIZE);
		EndDrawing();
		sun_pos++;
		if(sun_pos>SIZE){sun_pos=sun_pos-SIZE;}
	}
	CloseWindow();
	return 0;
}
