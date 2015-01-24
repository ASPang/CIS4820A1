
/* Derived from scene.c in the The OpenGL Programming Guide */
/* Keyboard and mouse rotation taken from Swiftless Tutorials #23 Part 2 */
/* http://www.swiftless.com/tutorials/opengl/camera2.html */

/* Frames per second code taken from : */
/* http://www.lighthouse3d.com/opengl/glut/index.php?fps */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics.h"

	/* initialize graphics library */
extern void graphicsInit(int *, char **);

	/* lighting control */
extern void setLightPosition(GLfloat, GLfloat, GLfloat);
extern GLfloat* getLightPosition();

	/* viewpoint control */
extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern void getViewOrientation(float *, float *, float *);

	/* add cube to display list so it will be drawn */
extern int addDisplayList(int, int, int);

	/* mob controls */
extern void createMob(int, float, float, float, float);
extern void setMobPosition(int, float, float, float, float);
extern void hideMob(int);
extern void showMob(int);

	/* player controls */
extern void createPlayer(int, float, float, float, float);
extern void setPlayerPosition(int, float, float, float, float);
extern void hidePlayer(int);
extern void showPlayer(int);

	/* flag which is set to 1 when flying behaviour is desired */
extern int flycontrol;
	/* flag used to indicate that the test world should be used */
extern int testWorld;
	/* list and count of polygons to be displayed, set during culling */
extern int displayList[MAX_DISPLAY_LIST][3];
extern int displayCount;
	/* flag to print out frames per second */
extern int fps;
	/* flag to indicate removal of cube the viewer is facing */
extern int dig;
	/* flag indicates the program is a client when set = 1 */
extern int netClient;
	/* flag indicates the program is a server when set = 1 */
extern int netServer; 

	/* frustum corner coordinates, used for visibility determination  */
extern float corners[4][3];

	/* determine which cubes are visible e.g. in view frustum */
extern void ExtractFrustum();
extern void tree(float, float, float, float, float, float, int);

/********* end of extern variable declarations **************/


	/*** collisionResponse() ***/
	/* -performs collision detection and response */
	/*  sets new xyz  to position of the viewpoint after collision */
	/* -can also be used to implement gravity by updating y position of vp*/
	/* note that the world coordinates returned from getViewPosition()
	   will be the negative value of the array indices */
void collisionResponse() {
   int i;  //Loop counter
   float x, y, z; //Viewpoint coordinates
   float oldX, oldY, oldZ; //Old viewpoint coordinates
   float spaceBuffer = -0.2, spaceBuffer2 = 0.5;   //VP buffer space
   
   int vpLocValueX, vpLocValueZ;
   int objX, objY, objZ; //Object coordinate check #1
   int objX2 = 0, objY2 = 0, objZ2 = 0; //Object coordinate check #2
   
   int collisionFlag = 0; 

   /* your collision code goes here */
   gameWall(); //Determine if the player is at the edge of the world


   /*Convert location to an integer*/
   getViewPosition(&x, &y, &z);

   objX = (int)(x - spaceBuffer) * -1;
   objY = (int)(y - spaceBuffer) * -1;
   objZ = (int)(z - spaceBuffer) * -1;

   objX2 = (int)(x) * (- 1);
   objY2 = (int)(y) * (- 1);
   objZ2 = (int)(z) * (- 1);
   
   /*objX2 = (int)((x - spaceBuffer2) * -1) - 1;
   objY2 = (int)((y - spaceBuffer2) * -1) -1;
   objZ2 = (int)((z - spaceBuffer2) * -1) - 1;

   vpLocValueX = ( (int)(floor( fabs( x ) * 10 ) ) ) % 10;
   vpLocValueZ = ( (int)(floor( fabs( x ) * 10 ) ) ) % 10;*/
   
   vpLocValueX = ( (int)(floor( fabs( x ) * 10 ) ) ) % 10;
   vpLocValueZ = ( (int)(floor( fabs( z ) * 10 ) ) ) % 10;

   printf("current location = %f, %f, %f  collsionResponse()\n", x, y, z);     //TESTING!!!!!
   printf("---obj = %d,%d,%d ---obj2 obj = %d,%d,%d --- world=%d and possible world=%d\n", objX, objY, objZ, objX2, objY2, objZ2, world[objX][objY][objZ], world[objX2][objY2][objZ2]);
   
   if (objY < 49) {
      if (world[objX][(int)y*(-1)][objZ] != 0) { // || world[objX2][objY2][objZ2] != 0) {
         printf("YOU'RE IN A CUBE collsionResponse cube=%d,%d,%d (%d)\n", objX,(int)y*(-1),objZ, world[objX][(int)y*(-1)][objZ]);
         getOldViewPosition(&oldX, &oldY, &oldZ);
         setViewPosition(oldX, oldY, oldZ);
         
         //if (world[objX2][objY2][objZ2] != 0) {
         //printf("YOU'RE IN A CUBE collsionResponse2\n");
         //getOldViewPosition(&oldX, &oldY, &oldZ);
         //setViewPosition(oldX, oldY, oldZ);
         //}
      
         /*Determine if the cube can be climbed*/
         //climbCube(objX, objY, objZ); //, oldX, oldY, oldZ);
         collisionFlag = 1;   //Set the collision flag
      }
      /*Determine if current position you're inside a cube*/
     // else {
         printf(">>> vpx=%d, vpy=%d \n", vpLocValueX, vpLocValueZ);
         if (vpLocValueX >= 7) {
            objX += 1;
            objX2 += 1;
         }
         
         if (vpLocValueZ >= 7) {
            objZ += 1;
            objZ2 += 1;
         }
         printf(">>>>YOU'RE IN A CUBE collsionResponse2 cube=%d,%d,%d (%d)\n", objX2,(int)y*(-1),objZ2, world[objX2][(int)y*(-1)][objZ2]);
         if (world[objX2][(int)y*(-1)][objZ2] != 0) {
            printf("YOU'RE IN A CUBE collsionResponse222\n");
            getOldViewPosition(&oldX, &oldY, &oldZ);
            setViewPosition(oldX, oldY, oldZ);
            
            collisionFlag = 1;   //Set the collision flag
         }
         
         if (world[objX][(int)y*(-1)][objZ] != 0) {
            printf("YOU'RE IN A CUBE collsionResponse233\n");
            getOldViewPosition(&oldX, &oldY, &oldZ);
            setViewPosition(oldX, oldY, oldZ);

            collisionFlag = 1;   //Set the collision flag
         }
         
         if (collisionFlag == 1) {
            /*Determine if the cube can be climbed*/
            climbCube(objX, objY, objZ);
            
            collisionFlag = 0;
         }
         
      //}
      /*if (vpLocValueZ > 5 || vpLocValueZ > 5) {
         printf("YOU'RE IN A CUBE collsionResponse2\n");
         if(world[objX2][objY2][objZ2] != 0) {
            getOldViewPosition(&oldX, &oldY, &oldZ);
            setViewPosition(oldX, oldY, oldZ);
         }
      }*/
   }  
}

int vpFirstDec(pos) {
   return ((int)(floor( fabs( pos ) * 10 ) ) ) % 10;
}

void climbCube(int cX, int cY, int cZ) { //, float x, float y, float z) {
   int cubeFront, cubeTop;
   float x, y, z;
   int vpLocValueX, vpLocValueZ;
   
   getViewPosition(&x, &y, &z);
   
   cubeTop = world[cX][(int)(y-1)*(-1)][cZ];
   
   printf("y+1 = %d, %d, %d, xyz = %f, %f, %f,,,, cubeTop = %d \n", cX, cY+1, cZ, x, y+1.1, z, cubeTop);
   printf("0000 old xyz = %f, %f, %f,,,, cubeTop = %d \n", x, y, z, cubeTop);
   
   /*Determine if VP is at the very edge of the cube*/
   vpLocValueX = ( (int)(floor( fabs( x ) * 10 ) ) ) % 10;
   vpLocValueZ = ( (int)(floor( fabs( z ) * 10 ) ) ) % 10;
   
   if (vpLocValueX < 1) {
      //cX += 0.5;
   }
   
   if (vpLocValueZ < 1) {
      //cZ += 0.5;
   }
   
   printf("nnnn Newpos xyz = %f, %f, %f,,,, cubeTop = %d \n", x, y-1.2, z, cubeTop);
   /*Determine if there's another cube on top of the colladed one*/
   /*cX -= 0.5;
   cY -= 0.5;*/
   if (cubeTop == 0) {
      setViewPosition((((float)cX)+0.5)*(-1), y - 1.2, (((float)cZ)+0.5)*(-1));
   }
}

void gameWall() {
    float x, y, z; //Viewpoint coordinates
    float spaceBuffer = 0.5;   //VP buffer space
    float xMax, zMax, min;  //Game wall boarder
    
    /*Calculate the game wall with 0.2 space buffer and covert it to negative value*/
    xMax = -(WORLDX - spaceBuffer);
    zMax = -(WORLDZ - spaceBuffer);
    min = -spaceBuffer;
    
    /*Get the VP current location*/
    getViewPosition(&x, &y, &z);
    
    /*Determine if VP is at the game wall*/
    if (x >= min) {
        setViewPosition(min, y, z);
    }
    else if (x <= xMax) {
        setViewPosition(xMax, y, z);
    }
    else if (z >= min) {
        setViewPosition(x, y, min);
    }
    else if (z <= zMax) {
        setViewPosition(x, y, zMax);
    }
}


	/*** update() ***/
	/* background process, it is called when there are no other events */
	/* -used to control animations and perform calculations while the  */
	/*  system is running */
	/* -gravity must also implemented here, duplicate collisionResponse */
void update() {
   int i, j, k;
   float *la;

	/* sample animation for the test world, don't remove this code */
	/* -demo of animating mobs */
   if (testWorld) {
	/* sample of rotation and positioning of mob */
	/* coordinates for mob 0 */
      static float mob0x = 50.0, mob0y = 25.0, mob0z = 52.0;
      static float mob0ry = 0.0;
      static int increasingmob0 = 1;
	/* coordinates for mob 1 */
      static float mob1x = 50.0, mob1y = 25.0, mob1z = 52.0;
      static float mob1ry = 0.0;
      static int increasingmob1 = 1;

	/* move mob 0 and rotate */
	/* set mob 0 position */
      setMobPosition(0, mob0x, mob0y, mob0z, mob0ry);

	/* move mob 0 in the x axis */
      if (increasingmob0 == 1)
         mob0x += 0.2;
      else 
         mob0x -= 0.2;
      if (mob0x > 50) increasingmob0 = 0;
      if (mob0x < 30) increasingmob0 = 1;

	/* rotate mob 0 around the y axis */
      mob0ry += 1.0;
      if (mob0ry > 360.0) mob0ry -= 360.0;

	/* move mob 1 and rotate */
      setMobPosition(1, mob1x, mob1y, mob1z, mob1ry);

	/* move mob 1 in the z axis */
	/* when mob is moving away it is visible, when moving back it */
	/* is hidden */
      if (increasingmob1 == 1) {
         mob1z += 0.2;
         showMob(1);
      } else {
         mob1z -= 0.2;
         hideMob(1);
      }
      if (mob1z > 72) increasingmob1 = 0;
      if (mob1z < 52) increasingmob1 = 1;

	/* rotate mob 1 around the y axis */
      mob1ry += 1.0;
      if (mob1ry > 360.0) mob1ry -= 360.0;
    /* end testworld animation */
   } else {
	/* your code goes here */
      /**TESTING!!!!!!!!!!!**/
      static int startflag2 = 0;
      
      if (startflag2 == 0) {
         setViewPosition(-53, -3, -50);
         startflag2 = 1;
      }
      
      /*Determine if the fly control is on or off*/
      if (flycontrol == 0) {
         gravity();
         
      } 

      /*Move the clouds*/
      //moveCloud();      //TESTING!!!!!!!!!!---Disabled in for windows environment testing
      
   }
}

void gravity() {
   float x, y, z;   //Viewpoints 0=x,1=y,2=z    
    float oldX, oldY, oldZ; //Old viewpoint coordinates
    float spaceBuffer = -0.2;   //VP buffer space
    int objX, objY, objZ; //Object coordinate check #1
    
    /*Convert location to an integer*/
    getViewPosition(&x, &y, &z);
    
    objX = (int)(x - spaceBuffer) * -1;
    objY = (int)(y - spaceBuffer) * -1;
    objZ = (int)(z - spaceBuffer) * -1;
    
    if (world[objX][objY][objZ] == 0) {
        setViewPosition(x, y + 0.1, z); 
        collisionResponse();
    }
}



int main(int argc, char** argv)
{
int i, j, k;
	/* initialize the graphics system */
   graphicsInit(&argc, argv);

	/* the first part of this if statement builds a sample */
	/* world which will be used for testing */
	/* DO NOT remove this code. */
	/* Put your code in the else statment below */
	/* The testworld is only guaranteed to work with a world of
		with dimensions of 100,50,100. */
   if (testWorld == 1) {
	/* initialize world to empty */
      for(i=0; i<WORLDX; i++)
         for(j=0; j<WORLDY; j++)
            for(k=0; k<WORLDZ; k++)
               world[i][j][k] = 0;
	
	/* some sample objects */
	/* build a red platform */
      for(i=0; i<WORLDX; i++) {
         for(j=0; j<WORLDZ; j++) {
            world[i][24][j] = 3;
         }
      }
	/* create some green and blue cubes */
      world[50][25][50] = 1;
      world[49][25][50] = 1;
      world[49][26][50] = 1;
      world[52][25][52] = 2;
      world[52][26][52] = 2;

	/* blue box shows xy bounds of the world */
      for(i=0; i<WORLDX-1; i++) {
         world[i][25][0] = 2;
         world[i][25][WORLDZ-1] = 2;
      }
      for(i=0; i<WORLDZ-1; i++) {
         world[0][25][i] = 2;
         world[WORLDX-1][25][i] = 2;
      }

	/* create two sample mobs */
	/* these are animated in the update() function */
      createMob(0, 50.0, 25.0, 52.0, 0.0);
      createMob(1, 50.0, 25.0, 52.0, 0.0);

	/* create sample player */
      createPlayer(0, 52.0, 27.0, 52.0, 0.0);

   } else {

	/* your code to build the world goes here */
      /*Creates the game landscape*/
      landscape();

   }

	/* starts the graphics processing loop */
	/* code after this will not run until the program exits */
   glutMainLoop();
   return 0; 
}

void landscape() {
   grassLand();
   waterFlow();
   moutainTops();
   cloudFloat(); 
}

/*Create a floor in the game world*/
void grassLand() {
   int x, z, y = 0;      //Loop counter
      
   for (x = 0; x < WORLDX; x++) {
      for (z = 0; z < WORLDZ; z++) {
         world[x][0][z] = 1;
      }
   }
}

/*Add a river along with the edge of the game world*/
void waterFlow() {
   int x, z = 0, y = 0;      //Loop counter

   for (x = 0; x < WORLDX; x++) {
      world[x][y][z] = 2;
   }
}

/*Add mountain to the game world*/
void moutainTops() {
   /*Use Perline noise to create a mountain tops*/
   //perlinValue();   
   
   int colour = 3;
   world[50][1][50] = colour;   //TESTING!!!
   world[51][1][50] = colour;   //TESTING!!!
   world[50][2][50] = 2;   //TESTING!!!
   world[51][1][51] = colour;   //TESTING!!!
   world[50][1][51] = colour;   //TESTING!!!
}


/*
 * REFERENCE: http://www.angelcode.com/dev/perlin/perlin.html
 */
void perlinValue() {
   int x, z;
   int i, j, p[WORLDX], nSwap;
   int SIZE = WORLDX;
   
   float gx[WORLDX], gy[WORLDX];
   float perX, perZ;
  
   /*Random Generator Seed*/
   srand(time(NULL));

   // Initialize the permutation table
   for(i = 0; i < SIZE; i++)
      p[i] = i;

   for(i = 0; i < SIZE; i++){
      j = rand() % SIZE;

      nSwap = p[i];
      p[i]  = p[j];
      p[j]  = nSwap;
   }
     
   // Generate the gradient look-up tables
   for(i = 0; i < SIZE; i++) {
      gx[i] = (float)(rand())/(RAND_MAX/2) - 1.0f; 
      gy[i] = (float)(rand())/(RAND_MAX/2) - 1.0f;
   }

   for (x = 0; x < (WORLDX - 1); x++) {
      for (z = 0; z < (WORLDZ - 1); z++) {
         perX = x / 13.5; 
         perZ = z / 13.5; 

         // Compute the integer positions of the four surrounding points
         int qx0 = (int)floorf(perX);
         int qx1 = qx0 + 1;

         int qy0 = (int)floorf(perZ);
         int qy1 = qy0 + 1;

         // Permutate values to get indices to use with the gradient look-up tables
         int q00 = p[(qy0 + p[qx0 % SIZE]) % SIZE];
         int q01 = p[(qy0 + p[qx1 % SIZE]) % SIZE];

         int q10 = p[(qy1 + p[qx0 % SIZE]) % SIZE];
         int q11 = p[(qy1 + p[qx1 % SIZE]) % SIZE];

         // Computing vectors from the four points to the input point
         float tx0 = perX - floorf(perX);
         float tx1 = tx0 - 1;

         float ty0 = perZ - floorf(perZ);
         float ty1 = ty0 - 1;

         // Compute the dot-product between the vectors and the gradients
         float v00 = gx[q00]*tx0 + gy[q00]*ty0;
         float v01 = gx[q01]*tx1 + gy[q01]*ty0;

         float v10 = gx[q10]*tx0 + gy[q10]*ty1;
         float v11 = gx[q11]*tx1 + gy[q11]*ty1;

         // Do the bi-cubic interpolation to get the final value
         float wx = (3 - 2*tx0)*tx0*tx0;
         float v0 = v00 - wx*(v00 - v01);
         float v1 = v10 - wx*(v10 - v11);

         float wy = (3 - 2*ty0)*ty0*ty0;
         float v = v0 - wy*(v0 - v1);
         
         /*Height*/
         v = (10 - v * 21);
         
         /*Fill the mountain*/
         fillMountain(x, (int)v, z);
      }
   }
}

/*Fills interior of the mountain*/
void fillMountain(int x, int y, int z) {
   int i, green = 1;
   
   /*Prevent the mountain from spawning in the sky*/
   if (y > 47 || y < 0) {
      y = 0;
   }
   
   /*Top of the mountain terrain*/
   world[x][y][z] = 1;
   
   /*Build the mountain bottom up*/
   for (i = 1; i < y; i++) {
      world[x][i][z] = 1;
   }
}

void cloudFloat() { 
   /*Generate the clouds*/
    int x=50, z = 50, y = 48;      //Coordinates
   int cloud = 5;

   world[x][y][50] = cloud;
   world[x+1][y][50] = cloud;
   world[x+1][y][50] = cloud;
   world[0][y][50] = cloud;

   cloudShape();
}

void cloudShape() {
   /*Create the shape of the cloud*/
   int arySize;

   /*Cloud type #1*/
   int cloudType1[] = {1,3,
                      2,1,  2,2,  2,3,  2,4,
                      3,2,  3,1,  3,-3,  1,-4,  
                      4,1};

   arySize = (int)(sizeof(cloudType1)/sizeof(int));
   createCloud(50,47,47, cloudType1, arySize);
   createCloud(15,47,15, cloudType1, arySize);
   createCloud(27,48,32, cloudType1, arySize);
   createCloud(80,48,22, cloudType1, arySize);
   createCloud(55,48,8, cloudType1, arySize);

   /*Cloud type #2*/
   int cloudType2[] =  {1,1,  1,2,  1,3,  1,5,  1,6,  1,7,
                       2,2,  2,3,  2,4, 2,6,  2,7,  2,8,
                       3,3,  3,4,  3,5,  3,6,  3,7,
                       4,4,  4,5,  4,6,  4,9,
                       5,3,  5,4,  5,6,  5,7,  5,8};

   arySize = (int)(sizeof(cloudType2)/sizeof(int));
   createCloud(20,47,7, cloudType2, arySize);
   createCloud(60,47,48, cloudType2, arySize);
   createCloud(20,47,60, cloudType2, arySize);
   createCloud(30,48,31, cloudType2, arySize);
   createCloud(20,48,7, cloudType2, arySize);
   createCloud(45,48,37, cloudType2, arySize);
   createCloud(25,48,25, cloudType2, arySize);

   /*Cloud type #3*/
   int cloudType3[] =  {1,1,  1,2,  1,3,  1,5,  1,6,  1,7,
                       2,2,  2,3,  2,4, 2,6,  2,7,  2,8,
                       3,3,  3,4,  3,5,  3,6,  3,7,
                       4,4,  4,5,  4,6,  4,9,
                       5,1,  5,3,  5,4,  5,5,  5,6,  5,7,  5,8,
                       6,4,  6,5,  6,6,  6,9,
                       7,2,  7,3,  7,7,  7,8,  7,9,
                       8,6,  8,7,  8,9,  8,10,
                       9,1,  9,3,  9,6,  9,7,  9,9};

   arySize = (int)(sizeof(cloudType3)/sizeof(int));
   createCloud(30,47,57, cloudType3, arySize);
   createCloud(38,48,21, cloudType3, arySize);
   createCloud(21,48,80, cloudType3, arySize);
   createCloud(10,47,80, cloudType3, arySize);
   createCloud(10,48,10, cloudType3, arySize);
   createCloud(21,47,20, cloudType3, arySize);   
}


/*Puts the clouds into the world 3D array*/
void createCloud(int x, int y, int z, int cloud[], int arySize) {
   int cloudX, cloudZ;   //Cloud information
   int i = 0;

   world[x][y][z] = 5;

   cloudX = x + cloud[i];
   cloudZ = z + cloud[i + 1];
   world[cloudX][y][cloudZ] = 5;


   for (i = 2; i < arySize; i+=2) {
      cloudX = x + cloud[i];
      cloudZ = z + cloud[i+1];
      world[cloudX][y][cloudZ] = 5;
   }
}


/*Move the clouds in the game every 0.35 seconds*/
void moveCloud() {
    static int resetTime = 1;
    int diff;
    int milsec = 10000;    //Milliseconds

    static clock_t start;
    clock_t end;

    /*Determine if timer needs to be reset*/
    if (resetTime == 1) {
        /*Reset Timer*/        
        start = clock();     //Get the current time
        resetTime = 0;       //Set timer flag
              
    }
    else if (resetTime == 0) {
        /*Determine number of seconds elapsed*/
        end = clock();
        diff = (int)((end - start) / milsec);

        /*Determine if 0.35 seconds have passed*/
        if (diff >= 35) {
            updateCloudLoc();   //Update cloud location
            resetTime = 1;   //Reset timer flag
        }
    }
}

/*Locate and update the cloud location*/
void updateCloudLoc() {
    int x, y, z;   //Stores the location 
    int cloudLine = 47;   //The height of where the clouds will be at

    /*Determine if the cube is a cloud*/
    for (y = cloudLine; y < WORLDY-1; y++) {
        for (z = WORLDZ - 1; z >= 0; z--) {
            for (x = WORLDX - 1; x >= 0; x--) {
                if (world[x][y][z] == 5) {
                    /*Remove old cloud*/
                    world[x][y][z] = 0;

                    /*Relocate the cloud*/
                    if (x+1 > WORLDX - 1) {
                        world[0][y][z] = 5;
                    }
                    else {
                        world[x+1][y][z] = 5;
                    }
                }
            }    
        }
    }
}
