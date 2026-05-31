#ifndef BPSIO_H
#define BPSIO_H

void bpsio_hide_cursor(void);
void bpsio_show_cursor(void);
void bpsio_clear_screen(void);
int bpsio_getch(void);

#ifndef BPSIO_NO_SHORT_NAMES
#define hide_cursor() bpsio_hide_cursor()
#define show_cursor() bpsio_show_cursor()
#define clear_screen() bpsio_clear_screen()
#define getch() bpsio_getch()
#define printx(color, ...) bpsio_printx(color, __VA_ARGS__);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BPSIO_H */

// =================================

#ifdef BPSIO_IMPLEMENTATION

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void bpsio_hide_cursor(void) {
  printf("\033[?25l");
  fflush(stdout);
}

void bpsio_show_cursor(void) {
  printf("\033[?25h");
  fflush(stdout);
}

void bpsio_clear_screen(void) {
  printf("\033[H\033[2J");
  fflush(stdout);
}

int bpsio_getch(void) {
  struct termios old_attr, new_attr;
  int ch;

  if (tcgetattr(STDIN_FILENO, &old_attr) == -1) {
    return -1;
  }

  new_attr = old_attr;
  new_attr.c_lflag &= ~(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) == -1) {
    return -1;
  }

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
  return ch;
}

#define bpsio_printx(color, ...)                                               \
  do {                                                                         \
    printf(color);                                                             \
    printf(__VA_ARGS__);                                                       \
  } while (0)

#endif /* BPSIO_IMPLEMENTATION */
