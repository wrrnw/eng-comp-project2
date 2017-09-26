/* Assignment2 of COMP20005(2017SM2)
 * Written by Dongwei Wei(728580)
 * Programming is fun!
 */

/*?? Is such long variable name a bad habit? */
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





/* function prototype */
double calculateAggSoundLvl(loudspeaker loudspeakers[],
                                    int numLoudspeaker, point selectedPoint);
void printStage1(int numLoudspeaker, loudspeaker loudspeakers[]);
void printStage2(point observationPoints[], int numObservationPoints,
                 loudspeaker loudspeakers[], int numLoudspeakers);
void printStage3(int numLoudspeakers, loudspeaker loudspeakers[]);
void printStage4(int numLoudspeakers, loudspeaker loudspeakers[]);
double distanceBetween(point pt1, point pt2);
double calculateSoundLvl(double L1, double r1, double r2);


#define MAX_LOUDSPEAKER_NUMBER 98 /* From the specification, assume between 1
                                     and 99 loudspeakers will be specified */
#define MAX_OBSERVATION_POINTS_NUMBER 98 /* From the specification, assume
                                            between 1 and 99 observation points
                                            will be specified */
#define DEFAULT_DISTANCE 1  /* A distance of one metre(1m) from the source is
                               a frequently used default distance */
#define SQUARE_REGION_SIDE_LENGTH 312 /* The side legnth of square region in
                                         stage3 */
#define THRESHOLD_SOUND_LEVEL 55 /* The threshold sound level used to check if
                                    a sound level is to low */
#define MAX_VERTICES_NUMBER 98 /* From the specification, assume the region
                              boundary consists of between 3 and 99 vertives */
const point origin = {0.0 , 0.0}; /* The origin point */



int main(int argc, char* argv[]) {

  char lineSpecifier;
  int loudspeakerCount = 0, observationPointsCount = 0, verticesCount = 0;
  loudspeaker loudspeakers[MAX_LOUDSPEAKER_NUMBER];
  point observationPoints[MAX_OBSERVATION_POINTS_NUMBER];
  point vertices[MAX_VERTICES_NUMBER];

  while(scanf("%c", &lineSpecifier) != EOF) {
    if(lineSpecifier == 'S') {
      scanf("%lf %lf %lf", &loudspeakers[loudspeakerCount].pt.x,
                           &loudspeakers[loudspeakerCount].pt.y,
                           &loudspeakers[loudspeakerCount].soundLevel);
      loudspeakerCount++;
    } else if(lineSpecifier == 'P') {
      scanf("%lf %lf", &observationPoints[observationPointsCount].x,
                       &observationPoints[observationPointsCount].y);
      observationPointsCount++;
    } else if(lineSpecifier == 'V') {
      scanf("%lf %lf", &vertices[verticesCount].x, &vertices[verticesCount].y);
      verticesCount++;
    }

  }

  printStage1(loudspeakerCount, loudspeakers);
  printStage2(observationPoints, observationPointsCount,
              loudspeakers, loudspeakerCount);
  printStage3(loudspeakerCount, loudspeakers);
  printStage4(loudspeakerCount, loudspeakers);

  return 0;
}

void printStage5() {

}

void printStage4(int numLoudspeakers, loudspeaker loudspeakers[]) {
  int x, y;
  double currentSoundLvl;
  printf("Stage 4\n");
  printf("==========\n");
  for(y = 308; y > 0; y -= 8){
    for(x = 2; x < SQUARE_REGION_SIDE_LENGTH; x += 4) {
      point currentPt = {x ,y};
      currentSoundLvl = calculateAggSoundLvl(loudspeakers, numLoudspeakers, currentPt);
      if(currentSoundLvl >= 100)
        printf("+");
      else if(currentSoundLvl >= 90)
        printf(" ");
      else if(currentSoundLvl >= 80)
        printf("8");
      else if(currentSoundLvl >= 70)
        printf(" ");
      else if(currentSoundLvl >= 60)
        printf("6");
      else if(currentSoundLvl > 55)
        printf(" ");
      else
        printf("-");
    }
    printf("\n");
  }
}


void printStage3(int numLoudspeakers, loudspeaker loudspeakers[]) {
  int x, y, numSampPts = 0, numTooLowPts = 0;
  double currentSoundLvl;
  for(x = 4; x < SQUARE_REGION_SIDE_LENGTH; x += 4) {
    for(y = 4; y < SQUARE_REGION_SIDE_LENGTH; y += 4) {
      point currentPt = {x, y};
      currentSoundLvl = calculateAggSoundLvl(loudspeakers, numLoudspeakers, currentPt);
      numSampPts++;
      if(currentSoundLvl <= THRESHOLD_SOUND_LEVEL) /*?? How to avoid using == for double comparison here? */
        numTooLowPts++;
    }
  }
  printf("Stage 3\n");
  printf("==========\n");
  printf("%d points sampled\n", numSampPts); /*?? Do we need to assume numSampts always be 4 digits?(use %04d specifier) */
  printf("%04d points (%05.2f%%) have sound level <= 55 dB\n\n", numTooLowPts,
                                              (double)numTooLowPts/numSampPts * 100);
}


void printStage2(point observationPoints[], int numObservationPoints,
                 loudspeaker loudspeakers[], int numLoudspeakers) {
  printf("Stage 2\n");
  printf("==========\n");
  int i;
  for(i = 0; i < numObservationPoints; i++) {
    double aggSoundLevelAtCurrentPoint;
    aggSoundLevelAtCurrentPoint = calculateAggSoundLvl(
                                        loudspeakers,
                                        numLoudspeakers,
                                        observationPoints[i]);
    printf("Sound level at (%05.1f, %05.1f): %5.2f dB\n",
                                        observationPoints[i].x,
                                        observationPoints[i].y,
                                        aggSoundLevelAtCurrentPoint);
  }
  printf("\n");
}


void printStage1(int numLoudspeakers, loudspeaker loudspeakers[]) {
  double aggSoundLevelAtOrigin;
  aggSoundLevelAtOrigin = calculateAggSoundLvl(loudspeakers,
                                                     numLoudspeakers, origin);
  printf("Stage 1\n");
  printf("==========\n");
  printf("Number of loudspeakers: %02d\n", numLoudspeakers);
  printf("Sound level at (%05.1f, %05.1f): %5.2f dB\n\n", origin.x,
                                                              origin.y,
                                                  aggSoundLevelAtOrigin);
}

/* Calculate and return the aggregate soundLevel at selected point */
double calculateAggSoundLvl(loudspeaker loudspeakers[],
                                    int numLoudspeakers, point selectedPoint) {

  double partialSum = 0.0;
  int i;
  double distanceToSelectedPoint, soundLevelAtSelectedPoint;

  for(i = 0; i < numLoudspeakers; i++) {
    /* If Li = 0 then it should not be added into Equation 2, additionally,
       try not use '==' for double comparison */
    if(abs(loudspeakers[i].soundLevel - 0.0) < 0.00000001) { /*?? Do we need to defne this magic number? how? */
      continue;
    }

    distanceToSelectedPoint = distanceBetween(selectedPoint, loudspeakers[i].pt);
    soundLevelAtSelectedPoint = calculateSoundLvl(loudspeakers[i].soundLevel,
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
double calculateSoundLvl(double L1, double r1, double r2) {
  return L1 + 20 * log10(r1 / r2);
}
