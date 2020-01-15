#define _USE_MATH_DEFINES

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <fcntl.h>
#include"funkcje2.h"
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define speed1 2
#define speed2 3
#define speed3 5


int main(int argc, char** argv) {
	int t1, t2, quit, frames, rc, placesAchieved = 0, lives = 5, jump = 0, up = 0, down = 0, left = 0, right = 0;
	int gameTime, dead = 0, drown = 0, moving = 0, alive = 1, timelimit, newlevel = 1, thing = 0, object_count = 0, frogframe = 0;
	int frogframecount = 0, framedifference = 0, points = 0, loaded = 0, level = 1, show_highscores = 0,girl=0;
	int pause = 0, quitprogramm = 0, win = 0, quitAttempt = 0, speeds[16], heartposition = 100, bonus_count = 0, fire = 0;
	char objects[16][33];
	char directions[16];
	double miliseconds = 0, difference = 0;
	char levelname[128];
	double delta, worldTime, fpsTimer, fps, etiSpeed;
	SDL_Event event;
	SDL_Surface* screen, * charset, * heart,*skull, * map, * home,*witcherBase,*witcherGirl,*bigheart,*coin;
	SDL_Surface* things[30];
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	object enemies[200];
	object bonuses[10];
	object head;
	record highscores[10];
	FILE* highscore_record,* fp;
	record player;
	Witcher witcher;
	char text[128];
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	srand(time(NULL));
	loadHighscores(highscores);
	sortScores(highscores);
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	SDL_SetWindowTitle(window, "FROGGER'2020a");


	SDL_Surface* el;
	el = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 255);
	SDL_SetColorKey(el, true, 0x000000);
	SDL_Surface* elr;
	elr = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 255);
	SDL_SetColorKey(elr, true, 0x000000);
	int x = 0;
	int y = 0;
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);
	char transform[50];
	FILE* key;
	key = fopen("key.txt", "r");
	for (int i = 0; i < 32; i++) {
		transform[i]=getc(key);
	}
	fclose(key);
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	things[1] = SDL_LoadBMP("./log1.bmp");
	if (things[1] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 1);
		return 1;
	}
	things[2] = SDL_LoadBMP("./log2.bmp");
	if (things[2] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 2);
		return 1;
	}
	things[3] = SDL_LoadBMP("./log3.bmp");
	if (things[3] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 3);
		return 1;
	}
	things[4] = SDL_LoadBMP("./frog2.bmp");
	if (things[4] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 4);
		return 1;
	}
	things[5] = SDL_LoadBMP("./divingfrog.bmp");
	if (things[5] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 5);
		return 1;
	}
	things[6] = SDL_LoadBMP("./body.bmp");
	if (things[6] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 6);
		return 1;
	}
	things[7] = SDL_LoadBMP("./pocisk1.bmp");
	if (things[7] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 7);
		return 1;
	}
	things[8] = SDL_LoadBMP("./wunsz.bmp");
	if (things[8] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 8);
		return 1;
	}
	things[9] = SDL_LoadBMP("./potion.bmp");
	if (things[9] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 9);
		return 1;
	}
	things[10] = SDL_LoadBMP("./ghoul2.bmp");//a
	if (things[10] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 10);
		return 1;
	}
	things[11] = SDL_LoadBMP("./ghoul1.bmp");//b
	if (things[11] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 11);
		return 1;
	}
	things[12] = SDL_LoadBMP("./wolf.bmp");//c
	if (things[12] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 12);
		return 1;
	}
	things[13] = SDL_LoadBMP("./kula.bmp");//d
	if (things[13] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 13);
		return 1;
	}
	things[14] = SDL_LoadBMP("./arrow.bmp");//e
	if (things[14] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 14);
		return 1;
	}
	things[15] = SDL_LoadBMP("./log.bmp");//w
	if (things[15] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 15);
		return 1;
	}
	things[16] = SDL_LoadBMP("./pocisk2.bmp");//f
	if (things[16] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 16);
		return 1;
	}
	things[17] = SDL_LoadBMP("./crocodile1.bmp");
	if (things[17] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 17);
		return 1;
	}
	things[18] = SDL_LoadBMP("./crocodile2.bmp");
	if (things[18] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 18);
		return 1;
	}
	things[19] = SDL_LoadBMP("./crocodile3.bmp");
	if (things[19] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 19);
		return 1;
	}
	things[20] = SDL_LoadBMP("./girl.bmp");
	if (things[20] == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 20);
		return 1;
	}
	for (int i = 1; i < 21; i++) {
		SDL_SetColorKey(things[i], true, 0x00ffffff);
	}
	SDL_SetColorKey(charset, true, 0x000000);
	witcher.look = SDL_LoadBMP("./witcher.bmp");
	if (witcher.look == NULL) {
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 21);
		return 1;
	}
	SDL_SetColorKey(witcher.look, true, 0x00ffffff);
	heart = SDL_LoadBMP("./heart.bmp");
	if (heart == NULL) {
		SDL_FreeSurface(witcher.look);
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 21);
		return 1;
	}
	SDL_SetColorKey(heart, true, 0x00ffffff);
	map = SDL_LoadBMP("./mapa1.bmp");
	if (map == NULL) {
		SDL_FreeSurface(witcher.look);
		SDL_FreeSurface(heart);
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 21);
		return 1;
	}
	home = SDL_LoadBMP("./homewitcher.bmp");
	if (home == NULL) {
		SDL_FreeSurface(witcher.look);
		SDL_FreeSurface(map);
		SDL_FreeSurface(heart);
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 21);
		return 1;
	}
	SDL_SetColorKey(home, true, 0x00ffffff);
	skull = SDL_LoadBMP("./skull.bmp");
	if (skull == NULL) {
		SDL_FreeSurface(witcher.look);
		SDL_FreeSurface(map);
		SDL_FreeSurface(skull);
		SDL_FreeSurface(heart);
		freeSurfaces(screen, charset, scrtex, window, renderer, things, 21);
		return 1;
	}
	SDL_SetColorKey(skull, true, 0x00ffffff);
	t1 = SDL_GetTicks();
	witcherBase = SDL_LoadBMP("./witcher.bmp");
	SDL_SetColorKey(witcherBase, true, 0x00ffffff);
	witcherGirl = SDL_LoadBMP("./witcher_girl.bmp");
	SDL_SetColorKey(witcherGirl, true, 0x00ffffff);
	bigheart= SDL_LoadBMP("./bigheart.bmp");
	SDL_SetColorKey(bigheart, true, 0x00ffffff);
	coin = SDL_LoadBMP("./coin.bmp");
	SDL_SetColorKey(coin, true, 0x00ffffff);
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	while (!quitprogramm) {
		DrawSurface(screen, home, 320, 240);
		sprintf(text, " PRESS A KEY TO PLAY GAME");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 300, text, charset);
		sprintf(text, " PRESS Q TO QUIT GAME");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 320, text, charset);
		sprintf(text, " PRESS H TO SEE HIGHSCORES");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 340, text, charset);
		sprintf(text, " PRESS L TO LOAD SAVED GAME");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 360, text, charset);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		witcher.look = witcherBase;
		quit = 1;
		lives = 5;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_q) {
					quitprogramm = 1;
					quit = 1;
					loaded = 0;
				}
				else if (event.key.keysym.sym == SDLK_h) {
					show_highscores = 1;
					loaded = 0;
				}
				else if (event.key.keysym.sym == SDLK_l) {
					load(&worldTime,&level,&points,&lives,&placesAchieved,enemies);
					newlevel = 1;
					quit = 0;
					alive = 1;
					quitAttempt = 0;
					loaded = 1;
				}
				else {
					quit = 0;
					alive = 1;
					quitAttempt = 0;
					loaded = 0;
				}
				break;

			case SDL_QUIT:
				quitprogramm = 1;
				break;
			}
		}
		int first = 1;
		while (show_highscores) {
				DrawRectangle(screen, 90, 100, 470, 200, czerwony, czarny);
				sprintf(text, "HIGHSCORES");
				DrawString(screen, 250, 102, text, charset);
				sprintf(text, "PRESS Q TO QUIT");
				DrawString(screen, 250, 270, text, charset);
				if (first) {
					for (int i = 0; i < 10; i++) {
						sprintf(text, "%d) %s - %d", i + 1, highscores[i].name, highscores[i].score);
						DrawString(screen, 200, 120 + i * 15, text, charset);
						SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
						SDL_RenderCopy(renderer, scrtex, NULL, NULL);
						SDL_RenderPresent(renderer);
					}
					first = 0;
				}
				while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_q) {
						show_highscores = 0;
					}
					break;

				case SDL_QUIT:
					quitprogramm = 1;
					break;
				}
			}
		}
			while (!quit) {
				heartposition = 100;
				if (lives > 0) {
					alive = 1;
				}
				if (placesAchieved == 5) {
					newlevel = 1;
					placesAchieved = 0;
					level++;
				}
				win = 0;
				timelimit = 50;
				worldTime = 0;
				witcher.look = witcherBase;
				if (newlevel) {
					sprintf(levelname, "levels/level%d.txt", level);
					fp = fopen(levelname, "r");
					loadLevel(fp, objects, &witcher, speeds, directions);
					fclose(fp);
					thing = 0;
					object_count = 0;
					for (int row = 0; row < 15; row++) {
						for (int column = 0; column < 32; column++) {
							for (int k = 0; k < 30; k++) {
								if (transform[k] == objects[row][column]) {
									thing = k;
								}
							}
							if (objects[row][column] != '0') {
								enemies[object_count].y = row;
								enemies[object_count].look = things[thing];
								enemies[object_count].x = column * 30;
								enemies[object_count].direction = directions[row];
								enemies[object_count].width = 30;
								if (speeds[row] == 1) {
									enemies[object_count].speed = speed1;
								}
								else if (speeds[row] == 2) {
									enemies[object_count].speed = speed2;
								}
								else if (speeds[row] == 3) {
									enemies[object_count].speed = speed3;
								}
								else {
									enemies[object_count].speed = 0;
								}
								if (enemies[object_count].y < 8) {
									enemies[object_count].element = 'w';
								}
								else {
									enemies[object_count].element = 'l';
								}
								if (object_count < 5) {
									if (!loaded) {
										enemies[object_count].state = 0;
									}
									else {
										if (enemies[object_count].state == 1) {
											enemies[object_count].look = things[18];
										}
									}
								}
								else {
									enemies[object_count].state = 0;
								}
								object_count++;
							}
						}
					}
					for (int j = 0; j < 32; j++) {
						for (int k = 0; k < 30; k++) {
							if (transform[k] == objects[15][j]) {
								thing = k;
							}
						}
						if (objects[15][j] != '0') {
							bonuses[j].y = 16;
							bonuses[j].look = things[thing];
							bonuses[j].x = j * 30;
							bonuses[j].speed = 0;
							bonuses[j].width = 30;
							bonuses[j].state = 0;
							bonus_count++;
						}

					}
					newlevel = 0;
				}
				if (lives == 5) {
					witcher.besty = witcher.y;
				}
				if(dead) {
					DrawSprite(screen, witcher.x, witcher.y, skull, 0);
					SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
					SDL_RenderCopy(renderer, scrtex, NULL, NULL);
					SDL_RenderPresent(renderer);
					_sleep(700);
					dead = 0;
				}
				witcher.x = witcher.startx;
				witcher.y = witcher.starty;
				while (alive) {
					if (!pause&&!quitAttempt) {
						t2 = SDL_GetTicks();
						miliseconds = (t2 - t1) * 0.1;
						delta = (t2 - t1) * 0.001;
						t1 = t2;
						difference += miliseconds;
						worldTime += delta;
						gameTime = (int)difference;
						framedifference = gameTime;
						DrawSurface(screen, map, 320, 240);
						if (witcher.y / 30 < 8 && witcher.y / 30 > 2) {
							drown++;
						}
						moving = 0;
						if (framedifference % 160 >= 80) {
							frogframe = (framedifference % 80) / 20;
						}
						else {
							frogframe =3-( (framedifference % 80) / 20);
						}
						
						for (int i = 0; i < object_count-1; i++) {
							if (enemies[i].look != things[5]) {
								if (enemies[i].x % 30 < 15) {
									DrawSprite(screen, enemies[i].x, enemies[i].y * 30, enemies[i].look, 0);
								}
								else {
									DrawSprite(screen, enemies[i].x, enemies[i].y * 30, enemies[i].look, 2);
									
								}
							}
							else {
								DrawSprite(screen, enemies[i].x, enemies[i].y * 30, enemies[i].look,frogframe);
								if (frogframe == 3) {
									enemies[i].state = 1;
								}
								else {
									enemies[i].state = 0;
								}
							}
							if (framedifference % 10 == 0) {
								if (enemies[i].direction == 'l') {
									enemies[i].x = enemies[i].x - enemies[i].speed;
									if (enemies[i].y < 8 && overlap(screen, witcher, enemies[i])&&enemies[i].state==0 && moving == 0) {
										witcher.x = witcher.x - enemies[i].speed;
										drown = 0;
										moving = 1;
									}
								}
								else if (enemies[i].direction == 'p') {
									enemies[i].x = enemies[i].x + enemies[i].speed;
									if (enemies[i].y < 8 && overlap(screen, witcher, enemies[i]) && enemies[i].state == 0 && moving == 0) {
										witcher.x = witcher.x + enemies[i].speed;
										drown = 0;
										moving = 1;
									}
								}
								if (bonuses[1].x > 100) {

								}
								if (enemies[i].x < 1) {
									enemies[i].x = 1000;
								}
								if (enemies[i].x > 1000) {
									enemies[i].x = 0;
									if (enemies[i].y < 7) {
										if (rand() % 10 == 2&&girl==0) {
											bonuses[1].x = enemies[i].x + 10;
											bonuses[1].y = enemies[i].y;
											bonuses[1].speed = enemies[i].speed;
											girl = 1;
										}
									}
								}
							}
							else {
								if (enemies[i].y < 8 && overlap(screen, witcher, enemies[i]) && enemies[i].state == 0 &&enemies[i].look!=things[19]) {
									drown = 0;
								}
							}
						}
						if (framedifference % 10 == 0) {
							if (girl == 1) {
								bonuses[1].x = bonuses[1].x + bonuses[1].speed;
							}
						}
						if (framedifference % 5 == 0) {
							if (fire== 1) {
								bonuses[5].x = bonuses[5].x -5;
							}
							if (bonuses[5].x < 1) {
								fire = 0;
							}
						}
						if (witcher.look != witcherGirl) {
							if (framedifference % 40 < 20) {
								DrawSprite(screen, bonuses[1].x, bonuses[1].y * 30, bonuses[1].look, 0);
							}
							else {
								DrawSprite(screen, bonuses[1].x, bonuses[1].y * 30, bonuses[1].look, 2);
							}
						}
						if (bonuses[1].x > 600) {
							girl = 0;
						}
						if (fire == 1) {
							if (framedifference % 40 < 20) {
								DrawSprite(screen, bonuses[5].x, bonuses[5].y * 30, bonuses[5].look, 0);
							}
							else {
								DrawSprite(screen, bonuses[5].x, bonuses[5].y * 30, bonuses[5].look, 2);
							}
						}
						if (bonuses[0].state == 1) {
							DrawSprite(screen, bonuses[0].x, bonuses[0].y * 30, bonuses[0].look, 0);
						}
						random(screen, &bonuses[0], worldTime, 800, 1, 2, 1, &bonuses[0]);
						random(screen, &bonuses[2], worldTime, 1000, 1, 2, 1, &bonuses[2]);
						if (bonus_count > 3) {
							random(screen, &bonuses[3], worldTime, 1000, 6, 3, 2, &head);
							if (overlap(screen, witcher, head)) {
								lives--;
								alive = 0;
								dead = 1;
							}
						}
						if (bonus_count > 4) {
							if (rand() % 500 == 2 && fire == 0) {
								fire = 1;
								bonuses[5].x = 570;
								bonuses[5].y=rand() % 10 + 2;
							}
							if (overlap(screen, witcher, bonuses[5])) {
								lives--;
								alive = 0;
								dead = 1;
							}
						}
						if (overlap(screen, witcher, bonuses[2])) {
							lives--;
							alive = 0;
							dead = 1;
						}
						if (overlap(screen, witcher, bonuses[1])) {
							witcher.look = witcherGirl;
						}
						if (overlap(screen, witcher, bonuses[0])) {
							points += 200;
							bonuses[0].state = 0;
						}
						
						for (int i = 0; i < object_count; i++) {
							if (enemies[i].y >= 8 && overlap(screen, witcher, enemies[i])) {
								lives--;
								alive = 0;
								dead = 1;
							}
							else if (witcher.y / 30 == 2) {
								if (overlap(screen, witcher, enemies[i]) && enemies[i].state == 0&&alive==1) {
									win = 1;
									enemies[i].look = coin;
									enemies[i].state = 1;
									placesAchieved++;
									points += 50;
									points += (timelimit - (int)worldTime) * 10;
									if (witcher.look == witcherGirl) {
										points += 200;
										DrawSurface(screen, bigheart, 320, 240);
										DrawRectangle(screen, 0, 0, 90, 480, czarny, czarny);
										DrawRectangle(screen, 560, 0, 80, 480, czarny, czarny);
										sprintf(text, " %d lives ", lives);
										DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 450, text, charset);
										sprintf(text, " %d points ", points);
										DrawString(screen, 400, 450, text, charset);
										SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
										SDL_RenderCopy(renderer, scrtex, NULL, NULL);
										SDL_RenderPresent(renderer);
										_sleep(1500);
									}
								}
							}
						}
						if (witcher.y / 30 == 2 && win > 0) {
							alive = 0;
							witcher.besty = witcher.starty;
						}
						else if (witcher.y / 30 == 2 && win == 0) {
							lives--;
							alive = 0;
							dead = 1;
						}
						if (witcher.y / 30 < 8 && drown > 0) {
							lives--;
							alive = 0;
							dead = 1;
						}
						if (witcher.x > 510 || witcher.x < 90) {
							lives--;
							alive = 0;
							dead = 1;
						}
						if (timelimit - worldTime < 0) {
							lives--;
							alive = 0;
							dead = 1;
						}

						if (jump) {
							if ((up == 1) || (down == 1)) {
								DrawSprite(screen, witcher.x, witcher.y, witcher.look, 1);
							}
							if (left == 1) {
								DrawSprite(screen, witcher.x + 15, witcher.y, witcher.look, 1);
							}
							if (right == 1) {
								DrawSprite(screen, witcher.x - 15, witcher.y, witcher.look, 1);
							}
						}
						else {
							DrawSprite(screen, witcher.x, witcher.y, witcher.look, 0);
						}

						if (gameTime % 40 == 0) {
							jump = 0; up = 0; right = 0; left = 0; down = 0;
						}
						if (fpsTimer > 0.5) {
							fps = frames * 2;
							frames = 0;
							fpsTimer -= 0.5;
						};
						heartposition = 100;
						for (int i = 0; i < lives; i++) {
							DrawSprite(screen, heartposition, 460, heart, 0);
							heartposition += 30;
						}
						for (int i = 5; i > lives; i--) {
							DrawSprite(screen, heartposition, 460, heart, 2);
							heartposition += 30;
						}
						if (witcher.besty > witcher.y) {
							points += 10;
							witcher.besty = witcher.y;
						}
						DrawRectangle(screen, 0, 0, 90, 480, czarny, czarny);
						DrawRectangle(screen, 560, 0, 80, 480, czarny, czarny);
						sprintf(text, " %d lives ", lives);
						DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 450, text, charset);
						sprintf(text, " %d points ", points);
						DrawString(screen, 400, 450, text, charset);
						if (timelimit - worldTime > 10) {
							DrawRectangle(screen, 300, 465, (timelimit - worldTime) * 2, 5, zielony, zielony);
						}
						else {
							DrawRectangle(screen, 300, 465, (timelimit - worldTime) * 2, 5, czerwony, czerwony);
						}
					}
					else if(pause) {
						DrawRectangle(screen, 90, 200, 470, 90, czerwony, czerwony);
						sprintf(text, " PAUZA ");
						DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 245, text, charset);
					}

					while (SDL_PollEvent(&event)) {
						switch (event.type) {
						case SDL_KEYDOWN:
							jump = 1;
							if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
							if (pause == 0) {
								if (event.key.keysym.sym == SDLK_UP) {
									witcher.y = witcher.y - 30; up = 1; left = 0; down = 0; right = 0;
								}
								else if (event.key.keysym.sym == SDLK_DOWN) {
									witcher.y = witcher.y + 30; up = 0; left = 0; down = 1; right = 0;
								}
								else if (event.key.keysym.sym == SDLK_LEFT) {
									witcher.x = witcher.x - 30; up = 0; left = 1; down = 0; right = 0;
								}
								else if (event.key.keysym.sym == SDLK_RIGHT) {
									witcher.x = witcher.x + 30; up = 0; left = 0; down = 0; right = 1;
								}
								else if (event.key.keysym.sym == SDLK_p) {
									pause = 1;
								}
								else if (event.key.keysym.sym == SDLK_s) {
									save(worldTime,level,points,lives,enemies);
								}
								else if (event.key.keysym.sym == SDLK_q) {
									sprintf(text, "QUIT GAME? Y/N");
									DrawRectangle(screen, 250, 295, 150, 20, czarny, czarny);
									DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 300, text, charset);
									quitAttempt = 1;
									bonus_count = 0;
									points = 0;
								}
							}
							else if (event.key.keysym.sym == SDLK_p) {
								pause = 0;
							}
							if (quitAttempt == 1) {
								if (event.key.keysym.sym == SDLK_y) {
									reset(&quit, &alive, &newlevel, &level);
								}
								else if (event.key.keysym.sym == SDLK_n) {
									quitAttempt = 0;
									
								}
							}
							break;

						case SDL_QUIT:
							quit = 1;
							quitprogramm = 1;
							alive = 0;
							break;
						};
					};
					SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
					SDL_RenderCopy(renderer, scrtex, NULL, NULL);
					SDL_RenderPresent(renderer);
					if (lives == 0) {
						alive = 0;
					}
					frames++;
				};
				if (lives == 0) {

					SDL_StartTextInput();
					char name[15];
					bool running = true;
					int licz= 0;
					player.score = points;
					level = 1;
					getName(screen, scrtex, renderer, charset, name);
					sprintf(player.name, "%s\0", name);
					replaceScore(highscores, player);
					sortScores(highscores);
					highscore_record = fopen("highscore.txt", "w");
					saveScores(highscores, highscore_record);
					fclose(highscore_record);
					SDL_StopTextInput();
					reset(&quit, &alive, &newlevel, &level);
					points = 0;
					lives = 5;
					dead = 0;

					
				}
			};
	}

	// zwolnienie powierzchni / freeing all surfaces
	SDL_RenderClear(renderer);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	};
