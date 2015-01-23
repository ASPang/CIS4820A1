
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
    float spaceBuffer = -0.2;   //VP buffer space
    int objX, objY, objZ; //Object coordinate check #1
    int objX2 = 0, objY2 = 0, objZ2 = 0; //Object coordinate check #2
    
	/* your collision code goes here */
    gameWall(); //Determine if the player is at the edge of the world
    
    
    /*Convert location to an integer*/
    getViewPosition(&x, &y, &z);
    
    objX = (int)(x - spaceBuffer) * -1;
    objY = (int)(y - spaceBuffer) * -1;
    objZ = (int)(z - spaceBuffer) * -1;
    
    //objX2 = (int)(x + spaceBuffer) * -1;
    //objY2 = (int)(y + spaceBuffer) * -1;
    //objZ2 = (int)(z + spaceBuffer) * -1;

    
    printf("current location = %f, %f, %f  collsionResponse()\n", x, y, z);     //TESTING!!!!!
    printf("---obj = %d,%d,%d ---obj2 obj = %d,%d,%d --- world=%d and possible world=%d\n", objX, objY, objZ, objX2, objY2, objZ2, world[objX][objY][objZ], world[objX2][objY2][objZ2]);

    
    if (world[objX][(int)y*(-1)][objZ] != 0) { // || world[objX2][objY2][objZ2] != 0) {
        printf("YOU'RE IN A CUBE collsionResponse\n");
        getOldViewPosition(&oldX, &oldY, &oldZ);
        setViewPosition(oldX, oldY, oldZ);
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
    printf("%f and z= %f -- xmax=%f zmax=%f min= %f\n", x, z, xMax, zMax, min);     //TESTING!!!!!
    /*Determine if VP is at the game wall*/
    if (x >= min) {
        //setViewPosition(x, downY, z);
        printf("xmin boarder reached %f \n", x);    //TESTING!!!!!
        setViewPosition(min, y, z);
    }
    else if (x <= xMax) {
        printf("xMax boarder reached %f \n", x);    //TESTING!!!!!
        setViewPosition(xMax, y, z);
    }
    else if (z >= min) {
        printf("ymin boarder reached %f \n", z);    //TESTING!!!!!
        setViewPosition(x, y, min);
    }
    else if (z <= zMax) {
        printf("ymax boarder reached %f \n", z);    //TESTING!!!!!
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
      /*Determine if the fly control is on or off*/
      if (flycontrol == 0) {
         gravity();
         
      }       
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
        printf("fall current location = %f, %f, %f\n", x, y, z);     //TESTING!!!!!
        setViewPosition(x, y + 0.1, z);
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
       //static int cloudAry[][];
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

void grassLand() {
   int x, z, y = 0;      //Loop counter
      
   for (x = 0; x < WORLDX; x++) {
      for (z = 0; z < WORLDZ; z++) {
         world[x][0][z] = 1;
      }
   }

    for (x = 0; x < WORLDX; x++) {
        for (z = 0; z < WORLDZ; z++) {
            //world[x][49][z] = 3;
        }
    }
}

void waterFlow() {
   int x, z = 0, y = 0;      //Loop counter

   for (x = 0; x < WORLDX; x++) {
      world[x][y][z] = 2;
   }
}

void moutainTops() {
   int x=50, z = 50, y = 1;      //Coordinates
   int mount = 3;

   /*world[x][y][z] = mount;
   world[x+1][y][z] = mount;
   world[x+2][y][z] = mount;
    world[x-1][y][z] = mount;
    world[x-2][y][z] = mount;
    
    world[x][y][z] = mount;
    world[x][y][z-1] = mount;
    world[x][y][z-2] = mount;
    world[x][y][z+1] = mount;
    world[x][y][z+2] = mount;
    
    
    world[x+1][y][z-1] = mount;
    world[x+2][y][z-2] = mount;
    world[x-1][y][z+1] = mount;*/
    world[x-2][y][z+2] = mount;
}

void cloudFloat() {
    
   /*Generate the clouds*/
    int x=50, z = 50, y = 48;      //Coordinates
   int cloud = 5;

   world[x][y][50] = cloud;
   world[x+1][y][50] = cloud;
   world[x+1][y][50] = cloud;
   world[0][y][50] = cloud;

}

void cloudShape() {
   /*Create the shape of the cloud*/
}

void moveCloud() {
    /*Move the cloud*/
}
