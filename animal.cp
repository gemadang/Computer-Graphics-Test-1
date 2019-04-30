//
//  animal.cpp
//  Animal_Class3
//
//  Created by Geri Elise Madanguit on 1/25/17.
//  Copyright © 2017 Geri. All rights reserved.
//
/******************************************
 *
 * Official Name:  Geri Madanguit
 *
 * Nickname:  Geri
 *
 * Environment/Compiler: Xcode
 *
 * Date:  Jan 31, 2017
 *
 *******************************************/

#include "animal.hpp"

///////////////////////////////////////////////////////////////////////////////////////
//
// animal.cpp
//
//In this program there is one alive catepillar in the middle. Around the catepillar is his dead catepillar buddies
//By using, x, X, y, Y, z, and Z, you must maneuver through the space to expose the last catepillar with radiation
//from the cylindrical lazer detector.
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

#include <cstdlib>
#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.

static float colorx = 0.7;
static float colory = 0.9;
static float colorz = 1.0;

static long font = (long)GLUT_BITMAP_HELVETICA_18; //FONT SELECT

//IGNORE THESE VARIABLES BELOW
    static float R = 3.0;
    static int v = 6;
    static int h = 4;
    static float cx=3.0, cy=3.0, cz=-10.0;
    static float necklength = 2.0;
    static float nx, ny;
    static float neckAngle=PI/6.0;
    static float torso=10.0;
    static float girth=5.0;
    static float URx, URy;
    static float legx=3.0, legy=5.0;
    static float tailx=5.0, taily=1.0;
//IGNORE THESE VARIABLES ABOVE



//ANT
    static float Ro = 8.0; // Radius of head.
    static int vo = 5; // Number of longitudinal (verticle) slices.
    static int ho = 3; // Number of latitudinal (horizontal) slices
    //on hemisphere.
    static float cxo=0.0, cyo=0.0, czo=-30.0;  //center of sphere - hea

// DETECTOR
static int gridCol = 38; // Number of grid columns (p)
static int gridRow = 53; // Number of grid rows (q)
static float *vertices = NULL; // Vertex array of the mapped sample

static float Xangle = 150.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate the cylinder.

//BOOL
static bool isOrtho = false;

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j))
    float e(int i, int j){
       return ( cos( (-1 + 2*(float)i/gridCol) * PI ) );
       // return j + 20;
    }

    float f(int i, int j){
        return ( sin( (-1 + 2*(float)i/gridCol) * PI ) );
    }

    float g(int i, int j){
        return ( -1 + 2*(float)j/gridRow);
    }

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
    int i, j, k;
    
    k = 0;
    for (j = 0; j <= gridRow; j++)
        for (i = 0; i <= gridCol; i++)
        {
            vertices[k++] = e(i,j);
            vertices[k++] = f(i,j);
            vertices[k++] = g(i,j);
            //cout<<g(i,j)<<endl;
        }
}

//FONT METHODS

void writeBitmapString(void *font, char*string)
{
    char *c;
    
    for (c = string; *c != '\0';c++) glutBitmapCharacter(font, *c);
}





//ignore this code below
void drawHead()
{
    int i, j;
    // Based on hemisphere.cpp from Guha
    // cx, cy,cz is the center of the sphere, R is the radius.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);
    
    for(j = 0; j < h; j++)
    {
        // One latitudinal triangle strip. top half
        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= v; i++)
        {
            glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                       R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                       R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
            glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                       R * sin( (float)j/h * PI/2.0  )+cy,
                       R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= v; i++)
        {
            glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                       -1*R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                       R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
            glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                       -1*R * sin( (float)j/h * PI/2.0  )+cy,
                       R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );
        }
        glEnd();
    }
}

void drawStickNeck()
{
    //
    glLineWidth(5.0);
    glColor3f(0.6,0.4,0.6);
    nx=necklength*cos(neckAngle); //forward component of neck
    ny=necklength*cos(neckAngle); //up component of neck
    URx=cx+R*cos(PI+neckAngle)-nx; //upper right x coord of body
    // = lower left coord of neck
    URy=cy+R*sin(PI+neckAngle)-ny; //upper right y coord of body
    glBegin(GL_LINES);
    glVertex3f(cx+R*cos(PI+neckAngle),cy+R*sin(PI+neckAngle),cz);
    glVertex3f(URx,URy,cz);
    glEnd();
    glLineWidth(1.0);  //restore width and color
    glColor3f(0.0,0.0,0.0);
}

void drawRectBody()
{
    glLineWidth(1.0);
    glColor3f(0.0,1.0,0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex3f(URx,URy,cz);      //upper right corner of body
    glVertex3f(URx-torso,URy,cz);
    glVertex3f(URx-torso,URy-girth,cz);
    glVertex3f(URx,URy-girth,cz);
    glEnd();
    glLineWidth(1.0);  //restore width and color
    glColor3f(0.0,0.0,0.0);
    
}


void drawTail()
{
    glLineWidth(2.0);
    glColor3f(0.3,0.3,1.0);
    glBegin(GL_LINES);
    glVertex3f(URx-torso,URy,cz);
    glVertex3f(URx-torso-tailx,URy+taily,cz);
    glEnd();
    glLineWidth(1.0);  //restore width and color
    glColor3f(0.0,0.0,0.0);
}


void drawStickLegs()
{
    glLineWidth(5.0);
    glColor3f(1.0,0.3,0.7);
    glBegin(GL_LINES);
    //two front legs
    glVertex3f(URx,URy-girth,cz); //at bottom right of body
    glVertex3f(URx+legx,URy-girth-legy,cz);//front forward
    glVertex3f(URx,URy-girth,cz); //at bottom right of body
    glVertex3f(URx-legx,URy-girth-legy,cz);//front backward
    //two back legs
    glVertex3f(URx-torso,URy-girth,cz); //at bottom left of body
    glVertex3f(URx-torso+legx,URy-girth-legy,cz);//back forward
    glVertex3f(URx-torso,URy-girth,cz); //at bottom left of body
    glVertex3f(URx-torso-legx,URy-girth-legy,cz);//back backward
    
    glEnd();
    glLineWidth(1.0);  //restore width and color
    glColor3f(0.0,0.0,0.0);
}

void drawAnimal()
{
    drawHead();
    drawStickNeck();
    drawRectBody();
    drawTail();
    drawStickLegs();
}

//IGNORE ABOVE




//LOOK HERE AND BELOW
void drawBody(){
    int i, j;
    // Based on hemisphere.cpp from Guha
    // cx, cy,cz is the center of the sphere, R is the radius.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);
    
    for(j = 0; j < ho; j++)
    {
       //THE ONE IN THE MIDDLE
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
    }
    
      glColor3f(0.0, 1.0, 0.0);

    for(j = 0; j < ho; j++)
    {
        // GREEN ONE
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo-15,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo-15,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo-15,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo-15,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
    }
    
    glColor3f(0.0, 0.0, 0.0);
    
    for(j = 0; j < ho; j++)
    {
  
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+15,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo+15,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+15,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo+15,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
        }
        glEnd();
    }
    
}


void drawEars(){
    glLineWidth(5.0);
    glColor3f(1.0,0.3,0.7);
    glBegin(GL_LINES);
    glVertex3f(URx,URy-girth,cz); //at bottom right of body
    glVertex3f(URx+legx,URy-girth-legy,cz);//front forward
    glVertex3f(URx,URy-girth,cz); //at bottom right of body
    glVertex3f(URx-legx,URy-girth-legy,cz);//front backward
    
    glEnd();
    glLineWidth(1.0);  //restore width and color
    glColor3f(0.0,0.0,0.0);
    
}


void drawBodyFill(int addx, int addy, int addz){
    int i, j;
    // Based on hemisphere.cpp from Guha
    // cx, cy,cz is the center of the sphere, R is the radius.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0, 0.0, 0.0);
    
    for(j = 0; j < ho; j++)
    {
        //THE ONE IN THE MIDDLE
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo+addz);
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+ addz);
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo+addz );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+addz );
        }
        glEnd();
    }
    
    glColor3f(0.0, 1.0, 0.0);
    
    for(j = 0; j < ho; j++)
    {
        // GREEN ONE
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo-15+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo+addz );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo-15+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+addz );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo-15+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo+addz );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo-15+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+addz);
        }
        glEnd();
    }
    
    glColor3f(0.0, 0.0, 0.0);
    
    for(j = 0; j < ho; j++)
    {
        
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+15+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+czo+addz );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       Ro * sin( (float)j/ho * PI/2.0  )+cyo+15+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+addz );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_FAN);
        for(i = 0; i <= vo; i++)
        {
            glVertex3f( Ro * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo+addx,
                       -1*Ro * sin( (float)(j+1)/ho * PI/2.0 )+cyo+15+addy,
                       Ro * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo+addz );
            glVertex3f( Ro * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo+addx,
                       -1*Ro * sin( (float)j/ho * PI/2.0  )+cyo+15+addy,
                       Ro * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo+addz );
        }
        glEnd();
    }

}

void drawDetector(){
   
    int i, j;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1.0, 0.0, 0.0);
    
    // Fill the vertex array.
    fillVertexArray();
    
    glRotatef(Zangle, 0.0, 2.0, 3.0);
    glRotatef(Yangle, 0.0, 3.0, 0.0);
    glRotatef(Xangle, 3.0, 2.0, 0.0);
    
    for(j = 0; j < gridRow; j++)
    {
        // One latitudinal triangle strip. top half
        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= gridCol; i++)
        {
            
            
            glArrayElement( (j+1)*(gridCol+1) + i );
            glArrayElement( j*(gridCol+1) + i );
            
            //glVertex3f( 5 * (j+1)/5-cxo+10,
              //         5 * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cyo -15,
                //       5 * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo);
            //glVertex3f( 5 * (j)/5-cxo+10,
              //         5 * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cyo -15,
                //       5 * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo );
            
            
            //glVertex3f( 3 * cos( (float)(j+1)/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI )+cxo -15,
             //          3 * sin( (float)(j+1)/ho * PI/2.0 )+cyo,
             //          3 * cos( (float)(j+1)/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI )+czo -15 );
            //glVertex3f( 3 * cos( (float)j/ho * PI/2.0 ) * cos( 2.0 * (float)i/vo * PI  )+cxo - 15,
              //         3 * sin( (float)j/ho * PI/2.0  )+cyo,
              //         3 * cos( (float)j/ho * PI/2.0 ) * sin( 2.0 * (float)i/vo * PI)+czo -15 );
    
        }
        glEnd();

    }
    

}


void drawCatP(){
    drawBody();
    drawEars();
    
    //the dead catepillar bodies around at random x y and z values
    
    drawBodyFill(-40,5,0);
    drawBodyFill(30,1,0);
    drawBodyFill(80,-23,4);
    drawBodyFill(25,7,11);
    drawBodyFill(-20,5,-6);
    drawBodyFill(-10,1,100);
    drawBodyFill(8,43,94);
    drawBodyFill(500,20,-1);
    
    drawBodyFill(-200,-95,-6);
    drawBodyFill(-18,-31,100);
    drawBodyFill(-34,-3,94);
    drawBodyFill(-35,-42,-1);
    
}

// Drawing routine.
void drawScene(void)
{
    glClearColor(colorx, colory, colorz, 0.0);
    if(isOrtho){
        cout<<"ortho"<<endl;
        glLoadIdentity();
        glClear (GL_COLOR_BUFFER_BIT);
        glOrtho(-10.0,30.0,-10.0,20.0,-20.0,3.0);
    }
    gluPerspective(200.0, 1, 1.0,500.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClear (GL_COLOR_BUFFER_BIT);
    
    glColor3f(0,1,1);
    glRasterPos3f(-20, 20, -30);
    writeBitmapString((void*) font, ("X: Y: Z:"));
    
    if(isOrtho){
        cout<<"ortho"<<endl;
        glLoadIdentity();
        glClear (GL_COLOR_BUFFER_BIT);
        glOrtho(-10.0,30.0,-10.0,20.0,-20.0,3.0);
    }
    else
        gluPerspective(100.0, 1, 1.0,200.0);
    
    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    vertices = new float[3*(gridCol+1)*(gridRow+1)]; // Dynamic array allocation with new value of gridRow and gridCol
    glVertexPointer(3, GL_FLOAT, 0, vertices);
  
    //draw animal
    drawCatP();
    drawDetector();
    glFlush();
    //glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    //i added ^^
    
    //white background
    glClearColor(colorx, colory, colorz, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //glOrtho(-30.0, 30.0, -30.0, 30.0, 10.0, 30.0);
        gluPerspective(200.0, (float)w/(float)h, 1.0,500.0);
   //glFrustum(-10.0,30.0,-10.0,20.0,-20.0,3.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'P':
            isOrtho = false;
            //glOrtho(-10.0,30.0,-10.0,20.0,-20.0,3.0);
            glutPostRedisplay();
            break;
        case 'O':
            isOrtho = true;
            glutPostRedisplay();
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colorx+=0.1;
            if(colorx > 1) colorx -= 0.4;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colorx+=0.1;
            if(colorx > 1) colorx -= 0.4;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colory+=0.1;
            if(colory > 1) colory -= 0.4;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colory+=0.1;
            if(colory > 1) colory -= 0.4;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colorz+=0.1;
            if(colorz > 1) colorz -= 0.4;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            cout<<"X:"<<Xangle<<"   Y:"<<Yangle<<"   Z:"<<Zangle<<endl;
            colorz+=0.1;
            if(colorz > 1) colorz -= 0.4;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout<< "Press x, X, y, Y, z, Z to maneuver with the laser detector to kill the catepillars! All the dead ones are around but there is still one left." << endl;
    
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("AN·NI·HI·LA·TION");
    setup(); 
    glutDisplayFunc(drawScene); 
    glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);
    glutMainLoop(); 
    
    return 0;  
}


