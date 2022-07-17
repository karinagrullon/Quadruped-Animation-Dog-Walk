//
//  display.hpp
//  CS560-Term-Project
//
//  Created by Karina Grullon on 5/21/21.
//
#define PI 3.1415926
#define header 54

void setup();
void reshape( int w, int h );
void display();
void projection(int width, int height, int perspectiveORortho);
void RenderGLScene(int base_raidus, int top_radius, int height);
void DrawGround();
void drawAxisLines();

void PitchUp(float n);
void PitchDown(float n);
void YawUp(float n);
void YawDown(float n);
void SlideUp(float n);
void SlideDown(float n );
void Walk(float n);

void readBMP(char * filename);


