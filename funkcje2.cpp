
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <fcntl.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

// narysowanie napisu txt na powierzchni screen, zaczynaj?c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj?ca znaki
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt ?rodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

// rysowanie linii o d?ugo?ci l w pionie (gdy dx = 0, dy = 1) 
// b?d? poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

// rysowanie prostok?ta o d?ugo?ci bok?w l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

// rysowanie klatki animacji (c to numer klatki 0-3) z 4 klatkowego sprite'a
void DrawSprite(SDL_Surface* screen, int x, int y,
	SDL_Surface* sprite, int c) {
	int px, py;
	SDL_Rect s, d;
	s.w = 30;
	s.h = 30;
	d.w = 30;
	d.h = 30;
	px = (c % 2) * 30;
	py = (c / 2) * 30;
	s.x = px;
	s.y = py;
	d.x = x;
	d.y = y;
	SDL_BlitSurface(sprite, &s, screen, &d);
};
Uint32 colors[6] = { 0x00000000,0x00ffffff,0x0000ff00, 0x00ffff00, 0x00FFC0CB, 0x00ff0000 };
/*
0 - czarny
1 - bia?y
2 - zielony
3 - ??ty
4 - r?owy
5 - czerwony
*/

#ifdef __cplusplus
extern "C"
#endif
//object - objekty poruszaj?ce si? po planszy
struct object {
	SDL_Surface* look;
	int y;
	int x;
	int speed;
	char direction;
	int width;
	char element;
	float time;
	int state; //for places - taken/free
};
//struktura bohatera
struct Witcher {
	SDL_Surface* look;
	int y;
	int x;
	int speed;
	int besty;
	int startx;
	int starty;
};
//wers z Highscores - nazwa u?ytkownika i osi?gni?ty wynik
struct record {
	int score;
	char name[16];
};

//funkcja zwracaj?ca 1 w przypadku kolizji bohatera z podanym obiektem i 0 w przypadku jej braku
bool overlap(SDL_Surface* screen, Witcher witcher, object enemy) {
	if (witcher.y == enemy.y * 30) {
		if (witcher.y == 60) {
			if ((witcher.x - enemy.x < 35 && enemy.x - witcher.x < 35) || witcher.x == enemy.x || witcher.x + 30 == enemy.x + enemy.width) {
				return 1;
			}
		}
		else {
			if ((witcher.x - enemy.x < 20 && enemy.x - witcher.x < 20) || witcher.x == enemy.x || witcher.x + 30 == enemy.x + enemy.width) {
				return 1;
			}
		}
		
	}
	return 0;
}

//funkcja sortuj?ca Highscores
void sortScores(record scores[10]) {
	record temp;
	for (int i = 0; i < 10; i++) {
		for (int j = i + 1; j < 10; j++)
		{
			if (scores[j].score > scores[i].score) {
				temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}

}

//funkcja zapisuj?ca Highscores do pliku
void saveScores(record scores[10], FILE* f) {
	for (int i = 0; i < 10; i++) {
		fprintf_s(f, "%s::%d\n", scores[i].name, scores[i].score);
	}
}

//funkcja zwalniaj?ca powierzchnie wczytane do tablicy things, od pierwszego do itego elementu
void freeSurfaces(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* things[30], int i) {
	printf("SDL_LoadBMP(things[%d]) error: %s\n", i, SDL_GetError());
	for (int j = 1; j < i; j++) {
		SDL_FreeSurface(things[j]);
	}
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

//funkcja losuj?ca pojawienie si? obiektu thing w przedziale od lini start do lini start+interval i wy?wietlaj?ca obiekt
void random(SDL_Surface* screen, object* thing, double worldTime, int chance, int interval, int start, int size,object* head) {
	if (rand() % chance == 2 && (thing->state == 0)) {
		thing->x = (rand() % 5) * 90 + 120;
		thing->y = (rand() % interval) + start;
		thing->state = 1;
		thing->time = worldTime;
	}

	if (worldTime - thing->time > 4) {
		thing->state = 0;
	}

	if (thing[0].state == 1) {
		for (int i = 0; i < size; i++) {
			DrawSprite(screen, thing->x + i * 30, thing->y * 30, thing->look, i);
		}
	}
	if (size > 1) {
		head->x = thing->x + 30;
		head->y = thing->y;
		head->look = thing->look;
	}

}

// funkcja zapisuje stan gry do pliku
void save(double worldTime, int level, int points, int lives, object enemies[]) {
	FILE* save;
	int base = 0;
	char bases[10];
	save = fopen("save.txt", "w");
	sprintf(bases, ":");
	for (int i = 0; i < 5; i++) {
		base = base * 10 + (enemies[i].state) + 1;
	}
	fprintf_s(save, "%fl::%d::%d::%d::%d", worldTime, level, points, lives, base);
	fclose(save);
}

// funkcja wczytuje stan gry z pliku
void load(double* worldTime, int* level, int* points, int* lives, int* placesAchieved, object enemies[]) {
	FILE* save;
	int base;
	char bases[10];
	*placesAchieved = 0;
	save = fopen("save.txt", "r");
	fscanf_s(save, "%fl::%d::%d::%d::%d", worldTime, level, points, lives, &base);
	itoa(base, bases, 10);
	for (int i = 0; i < 5; i++) {
		if (bases[i] == '2') {
			enemies[i].state = 1;
			(*placesAchieved)++;
		}
		else {
			enemies[i].state = 0;
		}
	}
	fclose(save);
}

//funkcja wczytuje z pliku Highscores
void loadHighscores(record highscores[]) {
	FILE* highscore_record;
	highscore_record = fopen("highscore.txt", "r");
	int tmpscore;
	for (int i = 0; i < 10; i++) {
		int j = 0;
		do {
			highscores[i].name[j] = getc(highscore_record);
			j++;
		} while (highscores[i].name[j - 1] != ':');
		highscores[i].name[j - 1] = '\0';
		fscanf_s(highscore_record, ":%d\n", &tmpscore);
		highscores[i].score = tmpscore;

	}
	fclose(highscore_record);
}

//funkcja wczytuje poziom gry z pliku
void loadLevel(FILE* fp, char objects[16][33], Witcher* witcher, int speeds[], char directions[]) {
	for (int row = 0; row < 16; row++) {
		char ch;
		for (int column = 0; column < 32; column++) {
			ch = getc(fp);
			objects[row][column] = ch;
			if (ch == 'x') {
				objects[row][column] = '0';
				witcher->x = 85 + column * 30;
				witcher->y = row * 30;
				witcher->startx = witcher->x;
				witcher->starty = witcher->y;
			}
		}
		ch = getc(fp);
		speeds[row] = ch - 48;
		ch = getc(fp);
		directions[row] = ch;
		ch = getc(fp);
	}
}

//funkcja pobiera nazwa gracza
void getName(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, char name[]) {
	bool running = true;
	char text[128];
	int licz = 0;
	sprintf(name, " ");
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	while (running) {
		SDL_Event ev;
		DrawRectangle(screen, 90, 200, 470, 90, czerwony, czerwony);
		sprintf(text, " YOU DIED");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 220, text, charset);
		DrawString(screen, 300, 240, name, charset);
		sprintf(text, "ENTER YOUR NAME:");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 230, text, charset);
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_TEXTINPUT) {
				licz += strlen(ev.text.text);
				sprintf(name, "%s%s", name, ev.text.text);
				DrawString(screen, 300, 240, name, charset);

			}
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && licz) {
				name[licz] = '\0';
				licz--;
				DrawString(screen, 300, 240, name, charset);
			}
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN && licz) {
				running = false;
			}
			else if (ev.type == SDL_QUIT) {
				running = false;
			}
		}
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

//funkcja sprawdza czy wynik gracza kwalifikuje si? na highscores i dopisuje go do tablicy
void replaceScore(record highscores[10], record player) {
	if (player.score > highscores[9].score) {
		highscores[9].score = player.score;
		for (int k = 0; k < 16; k++) {
			if (k < sizeof(player.name)) {
				highscores[9].name[k] = player.name[k];
			}
		}
	}
}

//funkcja ustawia zmienne na wyj?ciowe ustawienia
void reset(int* quit, int* alive, int* newlevel, int* level) {
	*quit = 1;
	*alive = 0;
	*newlevel = 1;
	*level = 1;
}