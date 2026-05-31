#ifndef BPSIO_H
#define BPSIO_H

void bpsio_hide_cursor(void);
void bpsio_show_cursor(void);
void bpsio_clear_screen(void);
char bpsio_getch(void);
char bpsio_getch_nonblocking(void);

#ifndef BPSIO_NO_SHORT_NAMES
#define hide_cursor() bpsio_hide_cursor()
#define show_cursor() bpsio_show_cursor()
#define clear_screen() bpsio_clear_screen()
#define getch() bpsio_getch()
#define getch_nonblocking() bpsio_getch_nonblocking()
#define printx(color, ...) bpsio_printx(color, __VA_ARGS__)
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

char bpsio_getch(void) {
  struct termios old_attr, new_attr;
  int ch;

  if (tcgetattr(STDIN_FILENO, &old_attr) == -1) {
    return '\0';
  }

  new_attr = old_attr;
  new_attr.c_lflag &= ~(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) == -1) {
    return '\0';
  }

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);

  // If getchar() encountered EOF or an error, return null
  if (ch == EOF) {
    return '\0';
  }

  return (char)ch;
}

char bpsio_getch_nonblocking(void) {
  struct termios oldt, newt;
  char ch;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);

  newt.c_cc[VMIN]  = 0;
  newt.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Initialize to 0 in case read() doesn't populate it (nothing pressed)
  ch = 0;
  read(STDIN_FILENO, &ch, 1);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return (ch == 0) ? '\0' : ch;
}

#define bpsio_printx(color, ...)                                               \
  do {                                                                         \
    printf(color);                                                             \
    printf(__VA_ARGS__);                                                       \
  } while (0)

#endif /* BPSIO_IMPLEMENTATION */
