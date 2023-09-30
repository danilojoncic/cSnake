#include "snake.h"
#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 840
#define GRID_SIZE 30
#define GRID_DIM 600
#define DELAY 90



static int highScore = 0;
int localScore = 0;

enum{
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT,
};


typedef struct{
	int x;
	int y;

}apple;

//globalna jabuka
apple Apple;

struct snake{
	int x;
	int y;
	int dir;
	struct  snake* next;
	
};

typedef struct snake Snake;
//globalni dijelovi zmije
Snake *head;
Snake *tail;


void initSnake(){
	srand(time(NULL));
	Snake *new = malloc(sizeof(Snake));
	new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
	new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
	new->dir = SNAKE_UP;
	new->next = NULL;


	head = new;
	tail = new;

	return;
}

void increaseSnake(){
	Snake *new = malloc(sizeof(Snake));

	switch(tail->dir){
		case SNAKE_UP:
			new->x = tail->x;
			new->y = tail->y + 1;
			break;
		case SNAKE_DOWN:
			new->x = tail->x;
			new->y = tail->y - 1;
			break;
		case SNAKE_LEFT:
			new->x = tail->x + 1;
			new->y = tail->y;
			break;
		case SNAKE_RIGHT:
			new->x = tail->x - 1;
			new->y = tail->y;
			break;			
	}

	new->next = NULL;
	tail->next = new;

	tail = new;
	return;
}

void moveSnake(){

	int prevX = head->x;
	int prevY = head->y;
	int prevDir = head->dir;

	switch(head->dir){
		case SNAKE_UP:
			head->y--;
			break;
		case SNAKE_DOWN:
			head->y++;
			break;
		case SNAKE_LEFT:
			head->x--;
			break;
		case SNAKE_RIGHT:
			head->x++;
			break;
	}

	Snake *track = head;
	if(track->next != NULL){
		track = track->next;
	}
	while(track != NULL){

		int saveX = track->x;
		int saveY = track->y;
		int saveDir = track->dir;


		track->x = prevX;
		track->y = prevY;

		track->dir = prevDir;
		
		track = track->next;

		prevX = saveX;
		prevY = saveY;
		prevDir = saveDir;

	}
	return;
}




void renderSnake(SDL_Renderer *renderer,int x , int y){
	int segSize = GRID_DIM/GRID_SIZE;
	SDL_Rect seg;
	int brightness = 255;
	int bDir = 0;
	seg.w = segSize - 2;
	seg.h = segSize - 2;

	Snake *track = head;
	while(track != NULL){
		SDL_SetRenderDrawColor(renderer,0x00,brightness,0x00,255);
		seg.x = x + track->x * segSize;
		seg.y = y + track->y * segSize;

		SDL_RenderFillRect(renderer,&seg);
		track = track->next;

		if(bDir == 0){
			brightness -= 5;
			if(brightness < 150){
				bDir = 1;
			}
		}
		if(bDir == 1){
			brightness += 5;
			if(brightness > 250){
				bDir = 0;
			}
		}

	}
}



void renderGrid(SDL_Renderer *renderer,int x, int y){
	int cellSize = GRID_DIM/GRID_SIZE;
	SDL_Rect cell;
	cell.w = cellSize;
	cell.h = cellSize;

	SDL_SetRenderDrawColor(renderer,0x55,0x55,0x55,255);
	for(int i = 0; i < GRID_SIZE;i++){
		for(int j = 0; j < GRID_SIZE;j++){
			cell.x = x + (i * cellSize);
			cell.y = y + (j * cellSize);
			SDL_RenderDrawRect(renderer,&cell);
		}
	}
	
	return;
}


void generateApple(){
	bool in_snake;

    do {
        in_snake = false;
        Apple.x = rand() % GRID_SIZE;
        Apple.y = rand() % GRID_SIZE;

        Snake *track = head;
        while(track != NULL) {
            
            if(Apple.x == track->x && Apple.y == track->y) {
                in_snake = true;
            }

            track = track->next;
        }
    }
    while(in_snake);
}

void renderApple(SDL_Renderer * renderer, int x, int y){

	SDL_SetRenderDrawColor(renderer,0xff,0x00,0x00,255);

	int appleSize = GRID_DIM / GRID_SIZE;
	SDL_Rect app;
	app.w = appleSize;
	app.h = appleSize;
	app.x = x + Apple.x * appleSize;
	app.y = y + Apple.y * appleSize;
	SDL_RenderFillRect(renderer,&app);
}

void putSnake(){
	initSnake();
	increaseSnake();
	increaseSnake();
	increaseSnake();
}

void resetSnake(){
	Snake *track = head;
	Snake *temp;

	while(track != NULL){
		temp = track;
		track = track->next;
		free(temp);
	}
	putSnake();
	localScore = 0;
	printf("LocalScore = %d, HighScore = %d\n",localScore,highScore);

}

void detectApple(){
	if(head->x == Apple.x && head->y == Apple.y){
		generateApple();
		increaseSnake();
		localScore++;
		if(localScore >= highScore){
			highScore = localScore;
		}
		printf("LocalScore = %d, HighScore = %d\n",localScore,highScore);

	}
	return;
}

void detectCrash(){
	if(head->x < 0 || head->x >= GRID_SIZE 
	|| head->y < 0 || head->y >= GRID_SIZE){
		resetSnake();
	}

	Snake *track = head;
	//ne gledaj glavu sa glave
	track = track->next;
	while(track != NULL){
		if(track->x == head->x && track->y == head->y){
			resetSnake();
		}
		track = track->next;
	}
}


void turnLeft(){
	switch (head->dir){
	case SNAKE_UP:
		head->dir = SNAKE_LEFT;
		break;
	case SNAKE_DOWN:
		head->dir = SNAKE_RIGHT;
		break;
	case SNAKE_LEFT:
		head->dir = SNAKE_DOWN;
		break;
	case SNAKE_RIGHT:
		head->dir = SNAKE_UP;
		break;
	}
	return;
}

void turnRight(){
	switch (head->dir){
	case SNAKE_UP:
		head->dir = SNAKE_RIGHT;
		break;
	case SNAKE_DOWN:
		head->dir = SNAKE_LEFT;
		break;
	case SNAKE_LEFT:
		head->dir = SNAKE_UP;
		break;
	case SNAKE_RIGHT:
		head->dir = SNAKE_DOWN;
		break;
	}
	return;
}

enum{
	TRY_FORWARD,
	TRY_LEFT,
	TRY_RIGHT,
};

int state(int try){
	int reward = 0;
	int tryX = head->x;
	int tryY = head->y;
	switch (head->dir){
		case SNAKE_UP:
			switch (try){
				case TRY_FORWARD:
					tryY--;
					break;
				case TRY_LEFT:
					tryX--;
					break;
				case TRY_RIGHT:
					tryX++;
					break;
			}
			break;
		case SNAKE_DOWN:
			switch (try){
				case TRY_FORWARD:
					tryY++;
					break;
				case TRY_LEFT:
					tryX++;
					break;
				case TRY_RIGHT:
					tryX--;
					break;
			}
			break;
		case SNAKE_LEFT:
			switch (try){
				case TRY_FORWARD:
					tryX--;
					break;
				case TRY_LEFT:
					tryY++;
					break;
				case TRY_RIGHT:
					tryY--;
					break;
			}
			break;
		case SNAKE_RIGHT:
			switch (try){
				case TRY_FORWARD:
					tryX++;
					break;
				case TRY_LEFT:
					tryY--;
					break;
				case TRY_RIGHT:
					tryY++;
					break;
			}
			break;
	}

	if(tryX < 0 || tryX > GRID_SIZE - 1){
		reward += -100;
	}
	if(tryY < 0 || tryY > GRID_SIZE - 1){
		reward += -100;
	} 

	if(tryX == Apple.x && tryY == Apple.y){
		reward += 100;
	}

	int diffX = abs(head->x - Apple.x);
	int diffY = abs(head->y - Apple.y);
	int tryDiffX = abs(tryX - Apple.x);
	int tryDiffY = abs(tryY - Apple.y);

	if(tryDiffX < diffX){
		reward += 5;
	}

	if(tryDiffY < diffY){
		reward += 5;
	}

	Snake *track = head;
	if(track->next != NULL){
		track = track->next;
	}

	while(track != NULL){
		if(tryX == track->x && tryY == track->y){
			reward += -100;
		}
		track = track->next;
	}





	return reward;
}



void ai(){
	int tryForward = state(TRY_FORWARD);
	int tryLeft	= state(TRY_LEFT);
	int tryRight = state(TRY_RIGHT);


	if(tryForward >= tryLeft && tryForward >= tryRight){
		//naprijed
	}else{
		if(tryLeft > tryRight){
			turnLeft();
			//lijevo
		}else{
			turnRight();
			//desno
		}
	}



	return;
}




int main()
{
	putSnake();
	generateApple();
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_INIT_VIDEO < 0){
		fprintf(stderr,"ERROR SDL INIT VIDEO");
	}
	
	window = SDL_CreateWindow("Snake",
								WINDOW_X,
								WINDOW_Y,
								WINDOW_WIDTH,
								WINDOW_HEIGHT,
								SDL_WINDOW_BORDERLESS);
	if(!window){
		fprintf(stderr,"ERROR !window");
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	if(!renderer){
		fprintf(stderr,"ERROR !rednerer");
	}

	int gridX = (WINDOW_WIDTH/2) - (GRID_DIM / 2);
	int gridY = (WINDOW_HEIGHT/2) - (GRID_DIM / 2);



	bool quit = false;
	SDL_Event event;
	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_UP:
							head->dir = SNAKE_UP;
							break;
						case SDLK_DOWN:
							head->dir = SNAKE_DOWN;
							break;
						case SDLK_LEFT:
							head->dir = SNAKE_LEFT;
							break;
						case SDLK_RIGHT:
							head->dir = SNAKE_RIGHT;
							break;
					}
					break;
			}
		}
		SDL_RenderClear(renderer);
		moveSnake();
		detectCrash();
		detectApple();

		//Render loop start
		renderGrid(renderer,gridX,gridY);
		renderSnake(renderer,gridX,gridY);
		renderApple(renderer,gridX,gridY);

		ai();

		//Render loop end
		SDL_SetRenderDrawColor(renderer,0x11,0x11,0x11,255);
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}
