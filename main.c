#include <stdio.h>
#include <stdlib.h> // system()
#include <string.h> // memset()
#include <ctype.h>  // tolower()
#include <conio.h>  // getch()
#include "functions.h"

#define APP_NAME "HexViewer"

int main(int argc, char *argv[])
{
  unsigned long currentPage = 0, pageCount = 0;
  char fileName[FILENAME_MAX];
  FILE *pFile;

  puts(APP_NAME);

  if (argc != 2) {
    printf("Usage: %s <file name>\n", argv[1]);
    exit(1);
  }

  if ((pFile = fopen(argv[1], "rb")) == NULL)
  {
    perror("Cannot open file");
    exit(1);
  }

  pageCount = getPageCount(pFile);

  for (;;) {
    // Clear screen
    system("cls");

    puts(APP_NAME);
    printf("File: %s\n", argv[1]);
    printf("\nPage %lu/%lu. (P)revious (N)ext (Q)uit?\n\n", currentPage + 1, pageCount);

    printHex(pFile, currentPage);

    // Parse keyboard input
    switch (tolower(getch())) {
      case 'p':
        if (currentPage > 0) --currentPage;
        break;
      case 'n':
        if (currentPage+1 < pageCount) ++currentPage;
        break;
      case 'q':
        fclose(pFile);
        return 0;
    }
  }

  return 0;
}