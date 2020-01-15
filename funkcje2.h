#ifndef functions
#define functions
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

// narysowanie napisu txt na powierzchni screen, zaczynaj?c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj?ca znaki
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt ?rodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

// rysowanie linii o d?ugo?ci l w pionie (gdy dx = 0, dy = 1) 
// b?d? poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

// rysowanie prostok?ta o d?ugo?ci bok?w l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);

// rysowanie klatki animacji (c to numer klatki 0-3) z 4 klatkowego sprite'a
void DrawSprite(SDL_Surface* screen, int x, int y, SDL_Surface* sprite, int c);


#ifdef __cplusplus
extern "C"
#endif
//object - objekty poruszaj?ce si? po planszy
typedef struct object {
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
typedef struct Witcher {
	SDL_Surface* look;
	int y;
	int x;
	int speed;
	int besty;
	int startx;
	int starty;
};
//wers z Highscores - nazwa u?ytkownika i osi?gni?ty wynik
typedef struct record {
	int score;
	char name[16];
};

//funkcja zwracaj?ca 1 w przypadku kolizji bohatera z podanym obiektem i 0 w przypadku jej braku
bool overlap(SDL_Surface* screen, Witcher witcher, object enemy);

//funkcja sortuj?ca Highscores
void sortScores(record scores[10]);

//funkcja zapisuj?ca Highscores do pliku
void saveScores(record scores[10], FILE* f);

//funkcja zwalniaj?ca powierzchnie wczytane do tablicy things, od pierwszego do itego elementu
void freeSurfaces(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* things[30], int i);

//funkcja losuj?ca pojawienie si? obiektu thing w przedziale od lini start do lini start+interval i wy?wietlaj?ca obiekt
void random(SDL_Surface* screen, object* thing, double worldTime, int chance, int interval, int start, int size,object* head);

// funkcja zapisuje stan gry do pliku
void save(double worldTime, int level, int points, int lives, object enemies[]);

// funkcja wczytuje stan gry z pliku
void load(double* worldTime, int* level, int* points, int* lives, int* placesAchieved, object enemies[]);
//funkcja wczytuje z pliku Highscores
void loadHighscores(record highscores[]);

//funkcja wczytuje poziom gry z pliku
void loadLevel(FILE* fp, char objects[16][33], Witcher* witcher, int speeds[], char directions[]);

//funkcja pobiera nazwa gracza
void getName(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, char name[]);
//funkcja sprawdza czy wynik gracza kwalifikuje si? na highscores i dopisuje go do tablicy
void replaceScore(record highscores[10], record player);

//funkcja ustawia zmienne na wyj?ciowe ustawienia
void reset(int* quit, int* alive, int* newlevel, int* level);

#endif

