/* Assignment2 of COMP20005(2017SM2)
 * Written by Dongwei Wei(728580)
 * Programming is fun!
 */

#include<stdio.h>

typedef struct {
  double xCoord, yCoord;
} point;

typedef struct {
  char lineSpecifier;
  point point;
  double soudLevel;
} eachLine;


int main(int argc, char* argv[]) {
  

  while(scanf("%c %f %f %f", eachLine.lineSpecifier,
      eachLine.point.xCoord, eachLine.point.yCoord, eachLine.soudLevel)) == 4) {
     printStage1();
  }
  return 0;
}

void printStage1() {
  printf("Stage1\n");
  printf("==========\n");
  printf("Number of loudspeakers: %02d\n");
  printf("Sound level at (000.0, 000.0): %5.2f\n", );
}
