/* Assignment2 of COMP20005(2017SM2)
 * Written by Dongwei Wei(728580)
 * Programming is fun!
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct {
  double x, y;
} point;

typedef struct {
  point pt;
  double soundLevel;
} loudspeaker;

typedef struct {
  point pt;
} observationPoint;


/* function prototype */
double calculateAggregateSoundLevel(loudspeaker loudspeakers[], int numLoudspeaker);
void printStage1(int numLoudspeaker, double Lsum);
double distanceBetween(point pt1, point pt2);
double calculateSoundLevel(double L1, double r1, double r2);


#define MAX_LOUDSPEAKER_NUMBER 98 /* From the specification, assume between 1
                                     and 99 loudspeakers will be specified */
#define DEFAULT_DISTANCE 1  /* A distance of one metre(1m) from the source is
                               a frequently used default distance */

const point origin = {0.0 , 0.0}; /* The origin point */


int main(int argc, char* argv[]) {

  char lineSpecifier;
  int loudspeakerCount = 0;
  double aggregateSoundLevel;
  loudspeaker loudspeakers[MAX_LOUDSPEAKER_NUMBER];

  while(scanf("%c", &lineSpecifier) != EOF) {
    if(lineSpecifier == 'S') {
      scanf("%lf %lf %lf", &loudspeakers[loudspeakerCount].pt.x,
                           &loudspeakers[loudspeakerCount].pt.y,
                           &loudspeakers[loudspeakerCount].soundLevel);
      loudspeakerCount++;
    }
  }
  aggregateSoundLevel = calculateAggregateSoundLevel(loudspeakers, loudspeakerCount);
  printStage1(loudspeakerCount, aggregateSoundLevel);


  return 0;
}

void printStage1(int numLoudspeaker, double Lsum) {
  printf("Stage1\n");
  printf("==========\n");
  printf("Number of loudspeakers: %02d\n", numLoudspeaker);
  printf("Sound level at (000.0, 000.0): %5.2f dB\n", Lsum);
}

/* Calculate and return the aggregate soundLevel to selected point */
double calculateAggregateSoundLevel(loudspeaker loudspeakers[], int numLoudspeaker) {
  double partialSum = 0.0;
  int i;
  double distanceToSelectedPoint, soundLevelAtSelectedPoint;

  for(i = 0; i < numLoudspeaker; i++) {
    /* If Li = 0 then it should not be added into Equation 2, additionally,
       try not use '==' for double comparison */
    if(abs(loudspeakers[i].soundLevel - 0.0) < 0.00000001) { /* Do we need to defne this magic number? how? */
      continue;
    }

    distanceToSelectedPoint = distanceBetween(origin, loudspeakers[i].pt);
    soundLevelAtSelectedPoint = calculateSoundLevel(loudspeakers[i].soundLevel,
                                DEFAULT_DISTANCE, distanceToSelectedPoint);
    partialSum += pow(10, (soundLevelAtSelectedPoint / 10));
  }

  return 10 * log10(partialSum);
}

/* Calculate and return the distance(m) between point pt1 and point pt2 */
double distanceBetween(point pt1, point pt2) {
  return sqrt(pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2));
}

/* Calculate and return the soundlevel measured from distance r2 with respective
   to that from distance r1 */
double calculateSoundLevel(double L1, double r1, double r2) {
  return L1 + 20 * log10(r1 / r2);
}
