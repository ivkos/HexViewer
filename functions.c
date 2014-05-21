#include <stdio.h>
#include <math.h>   // ceil()
#include <string.h> // memset()
#include <ctype.h>  // isprint()
#include "functions.h"

// Default console window size is 80x25
#define CONSOLE_COLS  80 // console width (characters)
#define CONSOLE_LINES 25 // console height (lines)

#define HEX_LINES 19 // lines of hex per page
#define SPACER    15 // space between hex and ASCII (up to 15)

unsigned getPageCount(FILE *pSrc) {
  long int fileSize;

  fseek(pSrc, 0, SEEK_END);
  fileSize = ftell(pSrc);
  rewind(pSrc);

  // Page count is equal to the file size divided by the size of the page
  return (unsigned) ceil(fileSize / (double)(HEX_LINES * 16));
}

void printHex(FILE *pSrc, unsigned long page) {
  int c;
  unsigned hexIndex = 0, printablesIndex = 0, readBytes = 0;
  char currentLine[CONSOLE_COLS];

  // Move the file indicator to the page provided
  fseek(pSrc, page * HEX_LINES * 16, 0);

  memset(currentLine, ' ', CONSOLE_COLS); // Fills the array with spaces
  currentLine[CONSOLE_COLS - 1] = '\0'; // Inserts a null-terminator

  for (;;) {
    c = fgetc(pSrc);
    ++readBytes;

    // Put anything but EOF
    if (c != EOF) {
      // Put hex code
      sprintf(currentLine + hexIndex, " %02x", c);
      currentLine[hexIndex + 3] = ' '; // removes the \0, inserted by sprintf() after the hex byte

      // Put printable characters in the right hand part of the array
      if (isprint(c))
        currentLine[48 + SPACER + printablesIndex++] = c;   // 48 = 16 hex bytes * 3 chars wide each
      else
        currentLine[48 + SPACER + printablesIndex++] = '.';

      hexIndex += 3; // increment by 3 since a hex byte is 3 chars wide (including 1 space)
    }

    // If 16 bytes are read or EOF is reached
    if (printablesIndex == 16 || c == EOF) {
      puts(currentLine); // Print the line

      if (c == EOF)
        break;

      // Reset the variables
      hexIndex = 0;
      printablesIndex = 0;
      memset(currentLine, ' ', CONSOLE_COLS);
      currentLine[CONSOLE_COLS - 1] = '\0';
    }

    // Stop reading after the page is full
    if (readBytes == HEX_LINES * 16)
      break;
  }
}