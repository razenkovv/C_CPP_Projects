#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "queue.h"

#define X_AXIS 51
#define Y_AXIS 30
#define BOTTOM 2
#define UP 8
#define LEFT 11
#define RIGHT 38
#define ALIEN_BULLET_PROBABILITY 100 

char world[X_AXIS][Y_AXIS];
const char player = 'A';
const char playerBullet = '^';
const char alienBullet = 'v';
const char smallAlien = 'o';
const char bigAlien = 'H';
const char space = ' ';
const char explosion = 'X';
const char smallWall = '_';
const char medWall = '-';
const char bigWall = 'T';

void aliens_moving(Queue *aliens, Queue *explosions, char *aliens_direction, int *live, int *max_y) {
	int add_x, add_y;
	if (*aliens_direction == 'l') {
		add_x = -1;
		add_y = 0;
	} else if (*aliens_direction == 'r') {
		add_x = 1;
		add_y = 0;
	} else {
		add_x = 0;
		add_y = 1;
		++*max_y;
	}
	
	Item *enemy = aliens->head;
	Item *prev_enemy = aliens->head;
	while (enemy) {
		int x = (enemy->data).x;
		int y = (enemy->data).y;
		if (world[x][y] == space) {
			queue_remove(aliens, &enemy, &prev_enemy);
		} else if (world[x+add_x][y+add_y] != playerBullet) {
			(enemy->data).x += add_x;
			(enemy->data).y += add_y;
			world[x][y] = space;
			prev_enemy = enemy;
			enemy = enemy->next;
			if (!enemy)
				aliens->tail = prev_enemy;
		} else if (world[x][y] == bigAlien) {
				(enemy->data).x += add_x;
				(enemy->data).y += add_y;
				(enemy->data).type = smallAlien;
				world[x+add_x][y+add_y] = smallAlien;
				world[x][y] = space;
				enemy = enemy->next;
		} else {
				queue_remove(aliens, &enemy, &prev_enemy);
				pos explPos;
				explPos.x = x + add_x;
				explPos.y = y + add_y;
				explPos.type = explosion;
				queue_put(explosions, explPos);
				world[x+add_x][y+add_y] = space;
				world[x][y] = space;
		}
	}

	if (*aliens_direction == 'd') {
		if ((aliens->head->data).x == 1)
			*aliens_direction = 'r';
		else
			*aliens_direction = 'l';
	} else if (((aliens->head->data).x == 1) ||
					((aliens->tail->data).x == (X_AXIS - 2)))
		*aliens_direction = 'd';
}

void player_bullet_manage(Queue *playerBullets, Queue *explosions) {
	Item *bull = playerBullets->head;
	Item *prev_bull = playerBullets->head;
	while (bull) {
		int x = (bull->data).x;
		int y = (bull->data).y;
		if (world[x][y] != playerBullet) {
			queue_remove(playerBullets, &bull, &prev_bull);
		} else if (y == 1) {
			world[x][y] = space;
			queue_remove(playerBullets, &bull, &prev_bull);
		} else if (world[x][y-1] == smallAlien) {
			world[x][y] = space;
			world[x][y-1] = space;
			pos explPos;
			explPos.x = x;
			explPos.y = y - 1;
			explPos.type = explosion;
			queue_put(explosions, explPos);
			queue_remove(playerBullets, &bull, &prev_bull);
		} else if (world[x][y-1] == bigAlien) {
			world[x][y] = space;
			world[x][y-1] = smallAlien;
			queue_remove(playerBullets, &bull, &prev_bull);
		} else if (world[x][y-1] == alienBullet) {
			world[x][y-1] = space;
			world[x][y] = space;
			queue_remove(playerBullets, &bull, &prev_bull);
		} else if (world[x][y-1] == space) {
			world[x][y-1] = playerBullet;
			world[x][y] = space;
			if (bull == playerBullets->head) {
				--(bull->data).y;
				bull = bull->next;
			} else {
				--(bull->data).y;
				prev_bull = bull;
				bull = bull->next;
				if (!bull)
					playerBullets->tail = prev_bull;
			}
		} 
	}
}

void alien_bullet_manage(Queue *alienBullets, Queue *aliens, int freq, int max_y, int *live) {
	Item *enemy = aliens->head;
	while (enemy) {
		int x = (enemy->data).x;
		int y = (enemy->data).y;
		if (world[x][y+1] == space) {
			int k = rand() % ALIEN_BULLET_PROBABILITY;
			if ((k == (ALIEN_BULLET_PROBABILITY - 1)) && ((max_y - y) < freq)) {
				pos alBulletPos;
				alBulletPos.x = x;
				alBulletPos.y = y;
				alBulletPos.type = alienBullet;
				queue_put(alienBullets, alBulletPos);
			}
		}
		enemy = enemy->next;
	}
	Item *ptr =  alienBullets->head;
		
	Item *bull = alienBullets->head;
	Item *prev_bull = alienBullets->head;
	while (bull) {
		int x = (bull->data).x;
		int y = (bull->data).y;
		if ((world[x][y] == smallAlien) || (world[x][y] == bigAlien)) {
				world[x][y+1] = alienBullet;
				++(bull->data).y;
				if (bull == alienBullets->head) {
					bull = bull->next;
				} else { 
					prev_bull = bull;
					bull = bull->next;
					if (!bull)
						alienBullets->tail = prev_bull;
				}
		} else if (world[x][y] == space) {
			queue_remove(alienBullets, &bull, &prev_bull);
		} else if ((y == (Y_AXIS - 2)) || (world[x][y+1] == smallAlien) || (world[x][y+1] == bigAlien)) {
			world[x][y] = space;
			queue_remove(alienBullets, &bull, &prev_bull);
		} else if (world[x][y+1] == playerBullet) {
			world[x][y] = space;
			world[x][y+1] = space;
			queue_remove(alienBullets, &bull, &prev_bull);
		} else if (world[x][y+1] == space) {
			world[x][y+1] = alienBullet;
			world[x][y] = space;
			++(bull->data).y;
			if (bull == alienBullets->head) {
				bull = bull->next;
			} else { 
				prev_bull = bull;
				bull = bull->next;
				if (!bull)
					alienBullets->tail = prev_bull;
			}
		} else if (world[x][y+1] == player) {
			world[x][y] = space;
			world[x][y+1] = space;
			*live = -1;
			break;
		}
	}
}
			
void wall_manage(Queue *walls) {
	Item *wall = walls->head;
	Item *prev_wall = walls->head;
	while (wall) {
		int x = (wall->data).x;
		int y = (wall->data).y;
		if (world[x][y+1] == playerBullet) {
			world[x][y+1] = space;
			if (wall == walls->head) {
				wall = wall->next;
			} else {
				prev_wall = wall;
				wall = wall->next;
				if (!wall)
					walls->tail = prev_wall;
			}
		} else if (world[x][y-1] == alienBullet) {
			world[x][y-1] = space;
			if (((wall->data).type) == smallWall) {
				world[x][y] = space;
				queue_remove(walls, &wall, &prev_wall);
			} else {
				if (((wall->data).type) == bigWall) {
					(wall->data).type = medWall;
					world[x][y] = medWall;
				} else if (((wall->data).type) == medWall) {
					(wall->data).type = smallWall;
					world[x][y] = smallWall;
				}
				if (wall == walls->head) {
					wall = wall->next;
				} else {
					prev_wall = wall;
					wall = wall->next;
					if (!wall)
						walls->tail = prev_wall;
				}
			}
		} else if (world[x][y-1] == space) {
			if (wall == walls->head) {
				wall = wall->next;
			} else {
				prev_wall = wall;
				wall = wall->next;
				if (!wall)
					walls->tail = prev_wall;
			}
		}
	}
}





void clean_explosions(Queue *explosions, int *enemy_count) {
	Item *expl = explosions->head, *prev;
	while (expl) {
		if (world[(expl->data).x][(expl->data).y] == explosion) {
			world[(expl->data).x][(expl->data).y] = space;
		}
		prev = expl;
		expl = expl->next;
		free(prev);
		--*enemy_count;
	}
	explosions->head = NULL;
}

void push_world(Queue *queue) {
	Item *ptr = queue->head;
	while (ptr) {
		if (world[(ptr->data).x][(ptr->data).y] == space)
			world[(ptr->data).x][(ptr->data).y] = (ptr->data).type;
		ptr = ptr->next;
	}
}

void display_world() {
	for (int y = 0; y < Y_AXIS; ++y) {
		for (int x = 0; x < X_AXIS; ++x) {
			printw("%c", world[x][y]);
		}
		printw("\n");
	}
}

int main() {
	Queue *playerBullets = queue_new();
	Queue *alienBullets = queue_new();
	Queue *aliens = queue_new();
	Queue *explosions = queue_new();
	Queue *walls = queue_new();
	const char QUIT = 'q';
	const int player_y = Y_AXIS - 3;
	int freq = 0;
	int max_y = UP;
	char aliens_direction;
	int enemy_count = ((UP - BOTTOM - 1) * (RIGHT - LEFT - 1)) / 2;
	srand(time(NULL));
	int r = rand() % 2;
	if (r == 1)
		aliens_direction = 'l';
	else 
		aliens_direction = 'r';
	initscr();
	clear();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	addstr("\n\n\n     WELCOME SOLDIER \n\n");
	addstr("     << New Game >> \n");
	addstr("        Continue  ");
	int answer = 1;
	while (1) {
		int c = getch();
		if (c == KEY_UP) {
			clear();
			addstr("\n\n\n     WELCOME SOLDIER \n\n");
			addstr("     << New Game >> \n");
			addstr("        Continue  \n");
			answer = 1;
		} else if (c == KEY_DOWN) {
			clear();
			addstr("\n\n\n     WELCOME SOLDIER \n\n");
			addstr("        New Game  \n");
			addstr("     << Continue >> \n ");
			answer = 2;
		} else if (c == '\n') {
			clear();
			break;
		}
	}
	FILE *bin;
	if (answer == 2) {
		bin = fopen("bin", "r+b");
		if (!bin) {
			addstr("You haven't saved anything yet.\n");
			addstr("New game will start.\n");
			refresh();
			sleep(2);
			answer = 1;
		}
	}
	pos playerPos;
	if (answer == 2) {
		addstr("Reading from file.\n");
		sleep(1);
		refresh();
		clear();
		bin = fopen("bin", "r+b");
		fread(world, sizeof(char), X_AXIS * Y_AXIS, bin);
		fread(&aliens_direction,  sizeof(char), 1, bin);  
		fread(&freq,  sizeof(int), 1, bin);  
		fread(&max_y,  sizeof(int), 1, bin);  
		fread(&enemy_count, sizeof(int), 1, bin);
		for (int y = 0; y < Y_AXIS; ++y) {
			for (int x = 0; x < X_AXIS; ++x) {
				if (world[x][y] == playerBullet) {
					pos bPos;
					bPos.x = x;
					bPos.y = y;
					bPos.type = playerBullet;
					queue_put(playerBullets, bPos);
				} else if (world[x][y] == player) {
					playerPos.x = x;
					playerPos.y = y;
					playerPos.type = player;
				}
			}
		}
		
		for (int x = 0, y = player_y - 1; x < X_AXIS; ++x) {
			if ((world[x][y] == bigWall) || (world[x][y] == medWall) || (world[x][y] == smallWall)) {
				pos wPos;
				wPos.x = x;
				wPos.y = y;
				wPos.type = world[x][y];
				queue_put(walls, wPos);
			}
		}

		for (int y = (Y_AXIS - 1); y >= 0; --y) {
			for (int x = 0; x < X_AXIS; ++x) {
				if (world[x][y] == alienBullet) {
					pos bPos;
					bPos.x = x;
					bPos.y = y;
					bPos.type = alienBullet;
					queue_put(alienBullets, bPos);
				}
			}
		}
		for (int x = 0; x < X_AXIS; ++x) {
			for (int y = 0; y < Y_AXIS; ++y) {
				if ((world[x][y] == bigAlien) || (world[x][y] == smallAlien)) {
					pos alPos;
					alPos.x = x;
					alPos.y = y;
					alPos.type = world[x][y];
					queue_put(aliens, alPos);
				}
			}
		}
		
		addstr("Game is starting.\n");
		sleep(1);
		refresh();
	} else {
		for (int y = 0; y < Y_AXIS; ++y) {
			for (int x = 0; x < X_AXIS; ++x) {
				if ((y > BOTTOM) && (y < UP) && (x > LEFT) && (x < RIGHT) && (x % 2 == 0)) {
					int k = rand() % 4;
					if (k < 3)
						world[x][y] = smallAlien;
					else 
						world[x][y] = bigAlien;
				} else
					world[x][y] = space;
			}
		}
		playerPos.x = X_AXIS / 2;
		playerPos.y = Y_AXIS - 3;
		playerPos.type = player;
	
		for (int y = 0; y < Y_AXIS; ++y) {
			world[0][y] = '|';
			world[X_AXIS - 1][y] = '|';
		}
		for (int x = 0; x < X_AXIS; ++x) {
			world[x][Y_AXIS - 1] = '-';
		}

		for (int x = LEFT + 1; x < RIGHT; x += 2) {
			for (int y = BOTTOM + 1; y < UP; ++y) {
				pos alienPos;
				alienPos.x = x;
				alienPos.y = y;
				alienPos.type = world[x][y];
				queue_put(aliens, alienPos);
			}
		}

		for (int x = 1, y = player_y - 1; x < X_AXIS - 1; ++x) {
			if (x % 5 == 0) {
				world[x][y] = bigWall;
				world[x-1][y] = bigWall;
				world[x+1][y] = bigWall;
				pos wPos;
				wPos.y = y;
				wPos.type = bigWall;
				wPos.x = x-1;
				queue_put(walls, wPos);
				wPos.x = x;
				queue_put(walls, wPos);
				wPos.x = x+1;
				queue_put(walls, wPos);

			}
		}
	}

	cbreak();
	int reload = 0;
	int left_stop = 0;
	int right_stop = 0;
	int live = 1;

	while (live == 1) {
		move(0, 0);
		world[playerPos.x][playerPos.y] = playerPos.type;
		display_world();
		refresh();
		clean_explosions(explosions, &enemy_count);
		if (!enemy_count)
			live = 2;
		timeout(50);
		int c = getch();
		if (c == KEY_UP) {
			if (reload == 0) {
				pos plBulletPos;
				plBulletPos.x = playerPos.x;
				plBulletPos.y = playerPos.y;
				plBulletPos.type = playerBullet;
				queue_put(playerBullets, plBulletPos);
				world[plBulletPos.x][plBulletPos.y] = plBulletPos.type;
				reload = 10;
			}
			
		} else if (c == KEY_LEFT) {
			if (left_stop == 0) {
				if (playerPos.x != 1) {
					world[playerPos.x][playerPos.y] = space;
					--playerPos.x;
				}
				left_stop = 3;
			}
		
		} else if (c == KEY_RIGHT) {
			if (right_stop == 0) {
				if (playerPos.x != X_AXIS - 2) {
					world[playerPos.x][playerPos.y] = space;
					++playerPos.x;
				}
				right_stop = 3;
			}
		
		} else if (c == QUIT) {
			FILE *bin = fopen("bin", "w+b");
			fwrite(world, sizeof(char), X_AXIS * Y_AXIS, bin);
			fwrite(&aliens_direction, sizeof(char), 1, bin);
			fwrite(&freq, sizeof(int), 1, bin);
			fwrite(&max_y, sizeof(int), 1, bin);
			fwrite(&enemy_count, sizeof(int), 1, bin);
			live = 3;
			break;
		}
		if (reload > 0)	
			--reload;
		if (left_stop > 0)
			--left_stop;
		if (right_stop > 0)
			--right_stop;
		
		wall_manage(walls);
		if (freq == 0) {
			aliens_moving(aliens, explosions, &aliens_direction, &live, &max_y);
			if ((aliens->head->data).y < 8)
				freq = 3;
			else if ((aliens->head->data).y >= 8 || ((aliens->head->data).y < 12))
				freq = 2;
			else if ((aliens->head->data).y >= 12)
				freq = 1;
		} else if (freq)
			--freq;
		
		push_world(aliens);
		alien_bullet_manage(alienBullets, aliens, freq, max_y, &live);
		player_bullet_manage(playerBullets, explosions);
		push_world(explosions);
		Item *enemy = aliens->head;
		while (enemy) {
			if (world[(enemy->data).x][(enemy->data).y] == explosion) {
				(enemy->data).type = space;
			} else if (world[(enemy->data).x][(enemy->data).x] != (enemy->data).type) {
				(enemy->data).type = world[(enemy->data).x][(enemy->data).y];
			}
			enemy = enemy->next;
		}
		if (max_y == (player_y - 2))
			live = 0;
	}

	if (live == 2) {
		clear();
		addstr("VICTORY\n");
		refresh();
		sleep(2);
	} else {
		if (live == -1) {
			clear();
			addstr("DEFEAT\n");
			addstr("Your ship was destroyed.\n");
			refresh();
			sleep(2);
		} else if (live == 0) {
			clear();
			addstr("DEFEAT\n");
			addstr("Aliens came too close.\n");
			refresh();
			sleep(2);
		} else {
			clear();
			addstr("Saved succesfully.\n");
			refresh();
			sleep(2);
		}
	}
	queue_delete(explosions);
	queue_delete(playerBullets);
	queue_delete(alienBullets);
	queue_delete(aliens);
	queue_delete(walls);
	endwin();
	return 0;
}
