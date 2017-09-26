/* Assignment2 of COMP20005(2017SM2)
 * Written by Dongwei Wei(728580)
 * Programming is fun!
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct {
  double x, y;
} point_t;

typedef struct {
  point_t pt;
  double soundLevel;
} loudspeaker_t;

typedef struct {
  point_t p1;
  point_t p2;
} line_t;


/* function prototype */
void readData(loudspeaker_t loudspeakers[], point_t observationPts[],
              point_t vertices[], int* loudspeakerCount,
              int* observationPtsCount, int* verticesCount);
void storeAllBoundryLines(int numVertices, point_t vertices[],
                          line_t boundaryLines[]);
void printStage1(int numLoudspeaker, loudspeaker_t loudspeakers[]);
void printStage2(point_t observationPoints[], int numObservationPoints,
                 loudspeaker_t loudspeakers[], int numLoudspeakers);
void printStage3(int numLoudspeakers, loudspeaker_t loudspeakers[]);
void printStage4(int numLoudspeakers, loudspeaker_t loudspeakers[]);
void printStage5(int numLoudspeakers, loudspeaker_t loudspeakers[],
                 int numVertices, point_t vertices[], line_t boundaryLines[]);
double calculateAggSoundLvl(loudspeaker_t loudspeakers[],
                            int numLoudspeaker, point_t selectedPoint);
double distanceBetween(point_t pt1, point_t pt2);
void displayCharacter(double souldLvl);
double calculateSoundLvl(double L1, double r1, double r2);
point_t findCentroid(int numVertices, point_t vertices[]);
int lineIntersect(line_t l1, line_t l2);


#define MAX_LOUDSPEAKERS_NUMBER 98 /* From the specification, assume between 1
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
#define TRUE 1 /* The value of truth */
#define FALSE 0 /* The value of falsehood */
#define EPS (1e-06) /* ??? */
#define ABS(x) (fabs(x)) /* The absolute value of a double number x */
#define MIN(a,b) (a<b ? a:b) /* The minimum value of a and b */
#define MAX(a,b) (a>b ? a:b) /* The maximum value of a and b */
const point_t origin = {0.0 , 0.0}; /* The origin point */
                            /*?? Should we put const here? */




/* The main function that generates the whole program */
int main(int argc, char* argv[]) {
  int numLoudspeakers, numObservationPts, numVertices;
  loudspeaker_t loudspeakers[MAX_LOUDSPEAKERS_NUMBER];
  point_t observationPts[MAX_OBSERVATION_POINTS_NUMBER];
  point_t vertices[MAX_VERTICES_NUMBER];
  line_t boundaryLines[MAX_VERTICES_NUMBER];

  readData(loudspeakers, observationPts, vertices, &numLoudspeakers,
           &numObservationPts, &numVertices);
  storeAllBoundryLines(numVertices, vertices, boundaryLines);
  printStage1(numLoudspeakers, loudspeakers);
  printStage2(observationPts, numObservationPts,
              loudspeakers, numLoudspeakers);
  printStage3(numLoudspeakers, loudspeakers);
  printStage4(numLoudspeakers, loudspeakers);
  printStage5(numLoudspeakers, loudspeakers, numVertices,
              vertices, boundaryLines);

  return 0;
}


/* Read the data from standard output */
void readData(loudspeaker_t loudspeakers[], point_t observationPts[],
              point_t vertices[], int* loudspeakerCount,
              int* observationPtsCount, int* verticesCount) {
  char lineSpecifier;
  *loudspeakerCount = 0;
  *observationPtsCount = 0;
  *verticesCount = 0;

  while(scanf("%c", &lineSpecifier) != EOF) {
    if(lineSpecifier == 'S') {
      scanf("%lf %lf %lf", &loudspeakers[*loudspeakerCount].pt.x,
                           &loudspeakers[*loudspeakerCount].pt.y,
                           &loudspeakers[*loudspeakerCount].soundLevel);
      (*loudspeakerCount)++;
    } else if(lineSpecifier == 'P') {
      scanf("%lf %lf", &observationPts[*observationPtsCount].x,
                       &observationPts[*observationPtsCount].y);
      (*observationPtsCount)++;
    } else if(lineSpecifier == 'V') {
      scanf("%lf %lf", &vertices[*verticesCount].x,
                       &vertices[*verticesCount].y);
      (*verticesCount)++;
    }
  }
}


void storeAllBoundryLines(int numVertices, point_t vertices[],
                          line_t boundaryLines[]) {
  /* Note that the number of boundary line segments is the same as the number
     of vertices */
  int i, numBoundaryLines;
  numBoundaryLines = numVertices;
  for(i = 0; i < numBoundaryLines-1; i++) {
    boundaryLines[i].p1 = vertices[i];
    boundaryLines[i].p2 = vertices[i+1];
  }
  /* The last boundary line connect the last vertex and the first vertex */
  boundaryLines[i].p1 = vertices[i];
  boundaryLines[i].p2 = vertices[0];
}


/* Calculate the aggregate sound level at origin and print stage 1 */
void printStage1(int numLoudspeakers, loudspeaker_t loudspeakers[]) {
  double aggSoundLvlAtOrigin;
  aggSoundLvlAtOrigin = calculateAggSoundLvl(loudspeakers,
                                             numLoudspeakers, origin);
  printf("Stage 1\n");
  printf("==========\n");
  printf("Number of loudspeakers: %02d\n", numLoudspeakers);
  printf("Sound level at (%05.1f, %05.1f): %5.2f dB\n\n", origin.x,
                                                          origin.y,
                                                  aggSoundLvlAtOrigin);
}


/* print stage 2 */
void printStage2(point_t observationPts[], int numObservationPts,
                 loudspeaker_t loudspeakers[], int numLoudspeakers) {
  double aggSoundLvlAtCurrentPt;

  printf("Stage 2\n");
  printf("==========\n");
  int i;
  for(i = 0; i < numObservationPts; i++) {
    aggSoundLvlAtCurrentPt = calculateAggSoundLvl(
                                        loudspeakers,
                                        numLoudspeakers,
                                        observationPts[i]);
    printf("Sound level at (%05.1f, %05.1f): %5.2f dB\n",
                                        observationPts[i].x,
                                        observationPts[i].y,
                                        aggSoundLvlAtCurrentPt);
  }
  printf("\n");
}


/* print stage 3 */
void printStage3(int numLoudspeakers, loudspeaker_t loudspeakers[]) {
  int x, y, numSampPts = 0, numTooLowPts = 0;
  double currentSoundLvl;
  point_t currentPt;

  for(x = 4; x < SQUARE_REGION_SIDE_LENGTH; x += 4) {
    for(y = 4; y < SQUARE_REGION_SIDE_LENGTH; y += 4) {
      currentPt = (point_t){.x = x, .y = y};
      currentSoundLvl = calculateAggSoundLvl(loudspeakers, numLoudspeakers,
                                             currentPt);
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


/* print stage 4 */
void printStage4(int numLoudspeakers, loudspeaker_t loudspeakers[]) {
  int x, y;
  double currentSoundLvl;
  point_t currentPt;

  printf("Stage 4\n");
  printf("==========\n");
  for(y = 308; y > 0; y -= 8) {
    for(x = 2; x < SQUARE_REGION_SIDE_LENGTH; x += 4) {
      currentPt = (point_t){.x = x, .y = y};
      currentSoundLvl = calculateAggSoundLvl(loudspeakers, numLoudspeakers,
                                             currentPt);
      displayCharacter(currentSoundLvl);
    }
    printf("\n");
  }
  printf("\n");
}


/* print stage 5 */
void printStage5(int numLoudspeakers, loudspeaker_t loudspeakers[],
                 int numVertices, point_t vertices[], line_t boundaryLines[]) {
  int x, y, i;
  double currentSoundLvl;
  int haveIntersected;
  point_t currentPt, centroidOfPolygon;
  line_t testLine, currentBoundaryLine;

  printf("Stage 5\n");
  printf("==========\n");
  centroidOfPolygon = findCentroid(numVertices, vertices);
  for(y = 308; y > 0; y -=8) {
    for(x = 2; x < SQUARE_REGION_SIDE_LENGTH; x += 4) {
      haveIntersected = FALSE;
      currentPt = (point_t){.x = x, .y = y};
      /* testLine represents the line between current point and centroid
         of the polygon */
      testLine.p1 = currentPt;
      testLine.p2 = centroidOfPolygon;
      currentSoundLvl = calculateAggSoundLvl(loudspeakers, numLoudspeakers,
                                             currentPt);
      for(i = 0; i < numVertices; i++) {
        currentBoundaryLine = boundaryLines[i];
        if(lineIntersect(testLine, currentBoundaryLine)) {
          haveIntersected = TRUE;
          break;
        }
      }
      if(haveIntersected) {
        printf("#");
      } else {
        displayCharacter(currentSoundLvl);
      }
    }
    printf("\n");
  }
}


/* Calculate and return the aggregate soundLevel at selected point */
double calculateAggSoundLvl(loudspeaker_t loudspeakers[],
                            int numLoudspeakers, point_t selectedPt) {

  double partialSum = 0.0;
  int i;
  double distanceToSelectedPt, soundLvlAtSelectedPt;

  for(i = 0; i < numLoudspeakers; i++) {
    /* If Li = 0 then it should not be added into Equation 2, additionally,
       try not use '==' for double comparison */
    if(abs(loudspeakers[i].soundLevel - 0.0) < 0.00000001) { /*?? Do we need to define this magic number? how? */
      continue;
    }

    distanceToSelectedPt = distanceBetween(selectedPt, loudspeakers[i].pt);
    soundLvlAtSelectedPt = calculateSoundLvl(loudspeakers[i].soundLevel,
                                DEFAULT_DISTANCE, distanceToSelectedPt);
    partialSum += pow(10, (soundLvlAtSelectedPt / 10));
  }

  return 10 * log10(partialSum);
}


/* Calculate and return the distance(m) between point pt1 and point pt2 */
double distanceBetween(point_t pt1, point_t pt2) {
  return sqrt(pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2));
}


/* Calculate and return the soundlevel measured from distance r2 with respective
   to that from distance r1 */
double calculateSoundLvl(double L1, double r1, double r2) {
  return L1 + 20 * log10(r1 / r2);
}

/* Display the characters corresponds to the sound level */
void displayCharacter(double soundLvl) {
  if(soundLvl >= 100)
    printf("+");
  else if(soundLvl >= 90)
    printf(" ");
  else if(soundLvl >= 80)
    printf("8");
  else if(soundLvl >= 70)
    printf(" ");
  else if(soundLvl >= 60)
    printf("6");
  else if(soundLvl > 55)
    printf(" ");
  else
    printf("-");
}

/* Find the centroid of a polygon using the vertices of the polygon */
point_t findCentroid(int numVertices, point_t vertices[]) {
  point_t centroid;
  int i, xSum = 0.0, ySum = 0.0;
  for(i = 0; i < numVertices; i++) {
    xSum += vertices[i].x;
    ySum += vertices[i].y;
  }
  centroid.x = xSum/numVertices;
  centroid.y = ySum/numVertices;
  return centroid;
}

/* This function was adapted in 2012 from
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/pdb.c
 * (no longer available at that URL in 2013)
 * and was written in the first instance by Paul Bourke
 *
 * Modified for use in a different assignment previously by Alistair Moffat by:
 *   . changing the argument type to two structs type line_t
 *   . making sure result is TRUE if an endpoint is on the other line
 * Modified for use by Jianzhong Qi by:
 *   . testing whether the projections of the two line segments intersect first
 *
 * Check if two lines are intersect and return ture for intersect */
int lineIntersect(line_t l1, line_t l2) {
   double x1=l1.p1.x, y1=l1.p1.y,
   	  x2=l1.p2.x, y2=l1.p2.y,
   	  x3=l2.p1.x, y3=l2.p1.y,
   	  x4=l2.p2.x, y4=l2.p2.y;
   double mua,mub;
   double denom,numera,numerb;

   /* Take the projections of the two line segments */
   double xMin1, xMax1, xMin2, xMax2, yMin1, yMax1, yMin2, yMax2;
   xMin1 = MIN(x1, x2);
   xMax1 = MAX(x1, x2);
   xMin2 = MIN(x3, x4);
   xMax2 = MAX(x3, x4);

   yMin1 = MIN(y1, y2);
   yMax1 = MAX(y1, y2);
   yMin2 = MIN(y3, y4);
   yMax2 = MAX(y3, y4);

   /* Do the projects intersect? */
   if((xMin2-xMax1) >= EPS || (xMin1-xMax2) >= EPS ||
   	   (yMin2-yMax1) >= EPS || (yMin1-yMax2) >= EPS) {
   	   return FALSE;
   }

   denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
   numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
   numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

   /* Are the line coincident? */
   if (ABS(numera) < EPS && ABS(numerb) < EPS && ABS(denom) < EPS) {
      return(TRUE);
   }

   /* Are the line parallel */
   if (ABS(denom) < EPS) {
      return(FALSE);
   }

   /* Is the intersection along the the segments */
   mua = numera / denom;
   mub = numerb / denom;
   /* AM - use equality here so that "on the end" is not an
    * intersection; use strict inequality if "touching at end" is an
    * intersection */
   if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
      return(FALSE);
   }
   return(TRUE);
}
