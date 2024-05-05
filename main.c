#include <stdbool.h>
#include <stdlib.h>

#include "LPC17xx.h"
#include "lcd.h"
#include "timer.h"

#define GAME_MENU 0
#define GAME_RUNNING 1
#define GAME_OVER 2

unsigned char game_status = GAME_MENU;

struct Position {
	unsigned char x;
	unsigned char y;
};

#define SNAKE_BUFFER 10

#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3

struct Snake {
	unsigned char move_direction;
	unsigned char length;
	bool grow;
	struct Position positions[SNAKE_BUFFER];
};

struct Position fruit;
struct Snake snake;

bool is_menu_animation_finished = false;

void _show_game_over_screen() {
	lcd_clear();

	lcd_cursor(0, 0);

	lcd_putchar('#');
	for (unsigned char ii = 1; ii <= 18; ii++) {
		lcd_putchar('=');
	}
	lcd_putchar('#');

	lcd_cursor(0, 1);
	lcd_putchar('I');
	lcd_cursor(20, 1);
	lcd_putchar('I');
	// GAME OVER
	lcd_cursor(5, 1);

	lcd_putchar('G');
	lcd_putchar('a');
	lcd_putchar('m');
	lcd_putchar('e');
	lcd_putchar(' ');
	lcd_putchar('O');
	lcd_putchar('v');
	lcd_putchar('e');
	lcd_putchar('r');

	lcd_cursor(0, 2);
	lcd_putchar('I');
	lcd_cursor(20, 2);
	lcd_putchar('I');

	lcd_cursor(9, 2);
	lcd_putchar(':');
	lcd_putchar('(');

	lcd_cursor(0, 3);

	lcd_putchar('#');
	for (unsigned char ii = 1; ii <= 18; ii++) {
		lcd_putchar('=');
	}
	lcd_putchar('#');
}

bool menu_pos = false;
void _draw_menu() {
	menu_pos = !menu_pos;
	lcd_cursor(0, 0);
	if (menu_pos) {
		// Draw #=
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
	} else {
		// Draw =#
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
	}

	lcd_cursor(0, 3);
	if (!menu_pos) {
		// Draw #= abwechselnd
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
	} else {
		// Draw =# abwechselnd
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
		lcd_putchar('=');
		lcd_putchar('#');
	}

	lcd_cursor(0, 1);
	lcd_putchar('I');
	lcd_cursor(20, 1);
	lcd_putchar('I');

	lcd_cursor(0, 2);
	lcd_putchar('I');
	lcd_cursor(20, 2);
	lcd_putchar('I');

	lcd_cursor(7, 1);
	// SNAKE
	lcd_putchar('S');
	lcd_putchar('N');
	lcd_putchar('A');
	lcd_putchar('K');
	lcd_putchar('E');

	// Press to start
	lcd_cursor(3, 2);
	lcd_putchar('P');
	lcd_putchar('r');
	lcd_putchar('e');
	lcd_putchar('s');
	lcd_putchar('s');
	lcd_putchar(' ');
	lcd_putchar('t');
	lcd_putchar('o');
	lcd_putchar(' ');
	lcd_putchar('s');
	lcd_putchar('t');
	lcd_putchar('a');
	lcd_putchar('r');
	lcd_putchar('t');
}

#define DELAY 15
void _show_game_menu_screen() {
	lcd_clear();
	lcd_cursor(0, 0);

	for (unsigned char ii = 1; ii <= 20; ii++) {
		lcd_putchar('#');
		delay_ms(DELAY);
	}

	lcd_cursor(20, 1);
	lcd_putchar('#');
	delay_ms(DELAY);

	lcd_cursor(20, 2);
	lcd_putchar('#');

	for (unsigned char ii = 20; ii > 0; ii--) {
		lcd_cursor(ii, 3);
		lcd_putchar('#');
		delay_ms(DELAY);
	}

	lcd_cursor(0, 2);
	lcd_putchar('#');
	delay_ms(DELAY);

	lcd_cursor(0, 1);
	lcd_putchar('#');
	delay_ms(50);

	lcd_clear();

	delay_ms(50);

	lcd_cursor(0, 0);
	for (unsigned char ii = 1; ii <= 20; ii++) {
		lcd_putchar('=');
	}
	lcd_cursor(0, 1);
	for (unsigned char ii = 1; ii <= 20; ii++) {
		lcd_putchar('=');
	}
	lcd_cursor(0, 2);
	for (unsigned char ii = 1; ii <= 20; ii++) {
		lcd_putchar('=');
	}
	lcd_cursor(0, 3);
	for (unsigned char ii = 1; ii <= 20; ii++) {
		lcd_putchar('=');
	}

	delay_ms(100);

	lcd_clear();

	is_menu_animation_finished = true;
	_draw_menu();
}

void spawn_fruit() {
	unsigned char x;
	unsigned char y;

	while (true) {
		x = rand() % 20;
		y = rand() % 4;

		// Check if position already exists
		bool exists = false;

		for (unsigned char index = 0; index < snake.length; index++) {
			if (snake.positions[index].x == x &&
				snake.positions[index].y == y) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			break;
		}
	}

	fruit.x = x;
	fruit.y = y;
}

void draw() {
	// ===== Draw Snake =====
	lcd_clear();

	for (unsigned char index = 0; index < snake.length; index++) {
		struct Position *position = &snake.positions[index];

		lcd_cursor(position->x, position->y);
		lcd_putchar('#');
	}

	unsigned char head_character;
	switch (snake.move_direction) {
		case DIRECTION_UP: {
			head_character = '^';
			break;
		}
		case DIRECTION_RIGHT: {
			head_character = '>';
			break;
		}
		case DIRECTION_DOWN: {
			head_character = 'v';
			break;
		}
		case DIRECTION_LEFT: {
			head_character = '<';
			break;
		}
	}

	struct Position *last_position = &snake.positions[0];

	lcd_cursor(last_position->x, last_position->y);
	lcd_putchar(head_character);

	// === === Draw fruit === ===
	lcd_cursor(fruit.x, fruit.y);
	lcd_putchar('*');
}

bool _is_snake_in_fruit() {
	return snake.positions[0].x == fruit.x && snake.positions[0].y == fruit.y;
}

bool _is_snake_collision() {
	struct Position *head = &snake.positions[0];

	for (unsigned char index = 1; index < snake.length; index++) {
		struct Position *position = &snake.positions[index];

		if (position->x == head->x && position->y == head->y) {
			return true;
		}
	}

	return false;
}

// Move all positions back by onee
void _move_positions_back() {
	// [1, 2, 3, 4] to:
	// [1, 1, 2, 3]

	// `- 1` because we don't need to move the last element
	for (unsigned char index = snake.length - 1; index > 0; index--) {
		unsigned char swap_index = index - 1;

		snake.positions[index].x = snake.positions[swap_index].x;
		snake.positions[index].y = snake.positions[swap_index].y;
	}
}

// 0 = ok
// 1 = abort
unsigned char move_snake() {
	struct Position *last_position = &snake.positions[0];

	if (snake.grow) {
		struct Position pop_position;
		pop_position.x = snake.positions[snake.length - 1].x;
		pop_position.y = snake.positions[snake.length - 1].y;

		snake.length++;
		snake.grow = false;

		_move_positions_back();

		snake.positions[snake.length - 1].x = pop_position.x;
		snake.positions[snake.length - 1].y = pop_position.y;
	} else {
		_move_positions_back();
	}

	switch (snake.move_direction) {
		case DIRECTION_UP: {
			if (last_position->y == 0) {
				last_position->y = 4;
			} else {
				last_position->y -= 1;
			}

			break;
		}
		case DIRECTION_RIGHT: {
			last_position->x = (last_position->x + 1) % 20;

			break;
		}
		case DIRECTION_DOWN: {
			last_position->y = (last_position->y + 1) % 4;

			break;
		}
		case DIRECTION_LEFT: {
			if (last_position->x == 0) {
				last_position->x = 20;
			} else {
				last_position->x -= 1;
			}

			break;
		}
	}

	if (_is_snake_collision()) {
		game_status = GAME_OVER;

		_show_game_over_screen();
		return 1;
	}

	if (_is_snake_in_fruit()) {
		snake.grow = true;
		spawn_fruit();
	}

	return 0;
}

// To avoid the user quickly changing the rotation too often, we need to lock it
unsigned char lock_rotation = 0;

void TIMER0_IRQHandler() {
	// Reset interrupt for MR0
	LPC_TIM0->IR |= 1 << 0;

	if (game_status == GAME_RUNNING) {
		if (move_snake() == 1) {
			return;
		}

		draw();

		lock_rotation = 0;
	}
}

void TIMER1_IRQHandler() {
	// Reset interrupt for MR0
	LPC_TIM1->IR |= 1 << 0;

	if (game_status == GAME_MENU && is_menu_animation_finished) {
		_draw_menu();
	}
}

void _init_snake() {
	snake.move_direction = DIRECTION_RIGHT;
	snake.length = 1;
	snake.grow = false;

	for (unsigned char index = 0; index < SNAKE_BUFFER; index++) {
		struct Position position;
		position.x = 0;
		position.y = 0;
		snake.positions[index] = position;
	}
}

void start_game() {
	game_status = GAME_RUNNING;
	_init_snake();
	spawn_fruit();
	draw();
}

#define BTN1 6
#define BTN2 16

void EINT3_IRQHandler() {
	// BTN 1
	if ((LPC_GPIOINT->IO0IntStatF) & (1 << BTN1)) {
		// Left button
		// Change directions to left

		// This is an optimized way to change the direction to the left
		if (game_status == GAME_RUNNING) {
			if (lock_rotation == 0) {
				if (snake.move_direction == 0) {
					snake.move_direction = 3;
				} else {
					snake.move_direction -= 1;
				}

				lock_rotation = 1;
			}
		} else {
			start_game();
		}

		// Release interrupt
		LPC_GPIOINT->IO0IntClr |= 1 << BTN1;
	} else if ((LPC_GPIOINT->IO0IntStatF) & (1 << BTN2)) {
		// Right button
		// Change direction to right

		if (game_status == GAME_RUNNING) {
			if (lock_rotation == 0) {
				snake.move_direction = (snake.move_direction + 1) % 4;

				lock_rotation = 1;
			}
		} else {
			start_game();
		}

		// Release interrupt
		LPC_GPIOINT->IO0IntClr |= 1 << BTN2;
	}
}

int main() {
	LPC_GPIOINT->IO0IntEnF |= 1 << BTN1;
	LPC_GPIOINT->IO0IntEnF |= 1 << BTN2;

	LPC_GPIOINT->IO0IntClr |= 1 << BTN1;
	LPC_GPIOINT->IO0IntClr |= 1 << BTN2;

	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn, 15);

	lcd_init();

	register_simple_timer_interrupt(TIMER_0, 500);
	register_simple_timer_interrupt(TIMER_1, 500);

	// Initialize random
	rand();

	_show_game_menu_screen();

	while (1) {
	}
}
