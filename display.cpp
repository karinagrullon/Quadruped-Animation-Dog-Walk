//
//  display.cpp
//  CS560-Term-Project
//
//  Created by Karina Grullon on 5/21/21.
//
//#include"display.hpp"
#include"display.hpp"
#include"util.h"


GLsizei width, height;

double m_slide=20;
double m_xtheta = 0;
double m_ytheta = 0;


int renderCounter = 0;

double lb1rotAngle = -10;
double lb1rotAngleOpposite = 10;
double lb2rotAngle = 10;
double displace = 0.0;

bool toggle = false;


int WIDTH_IMG;
int HEIGHT_IMG;
std::vector<unsigned char> data;

const unsigned int W = 400;
const unsigned int H = 300;
unsigned char *allPixels;

static GLuint texName;

// load BMP image.
void readBMP(char * filename) {
    
    FILE * fd;
    if((fd=fopen(filename,"rb")) == NULL){
        printf("Error happens\n");
    }else{
        printf("file opened\n");
    }
    unsigned char info[header];
  //  fread(info,sizeof(unsigned char), header, fd); //read the header-byte header
    //extract the  heght and width of the image from the header info.

    WIDTH_IMG = *(int *)&info[18];
    HEIGHT_IMG = *(int *)&info[22];
    int size=3 * WIDTH_IMG * HEIGHT_IMG;
   // printf("%d, %d\n", WIDTH_IMG, HEIGHT_IMG );

    unsigned char * pixel = (unsigned char *)malloc(sizeof(unsigned char)*size);
  //  fread(pixel, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);

    //restore pixel from BGR to RGB.
    //
    for (int i = 0; i < size; i += 3){
        unsigned char temp=pixel[i];
        pixel[i] = pixel[i+2];
        pixel[i+2] = temp;
    }

    for(int i = 0; i < size; i++){
        data.push_back(pixel[i]);
    }

    printf("%d\n", data.size());
    
    if(data.size()!=0) {
        glReadPixels(0, 0, W, H, GL_RGB,  GL_UNSIGNED_BYTE, pixel);
    }
    
   // return data;
}

void setup()
{
    glClearColor(255, 255, 255, 1.0); // *should* display white background
   // readBMP("./gress.bmp"); //WIDTH HEIGHT data are avaiable now!
    printf("load BMP image\n");
    
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH_IMG, HEIGHT_IMG, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
}

void drawGrassGround() {
    // draw the image using
     
       glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, 1000, 700, 0, -1, 1 );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
     
    
     glEnable(GL_TEXTURE_2D);
           glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_DECAL);
           glBindTexture(GL_TEXTURE_2D,texName); //active the texture.
           
    glBegin(GL_QUADS);
              glTexCoord2f(0.0, 1.0);    glVertex3f(50,      50,       0);
              glTexCoord2f(1.0, 1.0);    glVertex3f(50+WIDTH_IMG,50,       0);
              glTexCoord2f(1.0, 0.0);    glVertex3f(50+WIDTH_IMG,50+HEIGHT_IMG,0);
              glTexCoord2f(0.0, 0.0);    glVertex3f(50,      50+HEIGHT_IMG,0);
           glEnd();
       glDisable(GL_TEXTURE_2D);
}

void reshape( int w, int h ){
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h ); // set to size of window
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //gluOrtho2D( 0.0, (float)w, 0.0, (float)h );

    glOrtho( 0, w, h, 0, -1, 1 );
    width = w;  // records width globally
    height = h; // records height globally

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display() {
   // glClear(GL_COLOR_BUFFER_BIT); // clear window
   glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

   glViewport(0, 0, width, height);
   {
      projection(width/2.0, height/2.0, 1);
     //(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
      gluLookAt(0.0, 0.0, m_slide, // virtual camera initial position
                0.0, 0.0, 0.0, // looking at origin point because is 0.0 0.0 0.0
                0.0, 1.0, 0.0);  // up-vector (selects orientation of the camera)
     // glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );
       glPushMatrix();
       
       glRotatef(m_xtheta, 1,0,0);
       glRotatef(m_ytheta, 0,1,0);
       glRotatef(0, 0,0,1);
       RenderGLScene(5, 5, 30);
       glPopMatrix();
   }
    
    if(1){
        // render the mesh model
      //  drawGrassGround();
    }
    
    glutSwapBuffers(); // display newly drawn image in window
}


void projection(int width, int height, int perspectiveORortho){
  float ratio = (float)width/height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (perspectiveORortho)
      gluPerspective(60, ratio, 1, 256); // fovy, aspect = w/h, zNear, zFar
  else
      glOrtho(-ratio, ratio, -ratio, ratio, 1, 1000);
  glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
}

void RenderGLScene(int base_radius, int top_radius, int height) {
   // renderCounter++;
    // draw a cylinder or a sphere,
    // you will draw more objects in this function
    
 //   (toggle == false) ? true : false;
  //  printf("%d", renderCounter);
    
    glPushMatrix();
    glTranslatef(displace, 0, 0);
    
    
    GLUquadricObj* quadObj = gluNewQuadric();
    gluQuadricDrawStyle (quadObj, GLU_LINE);
    

    
    // face
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
                                      // Top face (y = 1.0f)
                                    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.5f, 0.35f, 0.05f);     // Brown
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
   // glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
   // glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
   // glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
  //  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
   
    
    
    // snout
    glPushMatrix();
    glTranslatef(-1.5, -0.4, 0);
    
    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
  //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 0.6f, -1.0f);
    glVertex3f(-1.0f, 0.6f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    // glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -0.6f, 1.0f);
    glVertex3f(-1.0f, -0.6f, 1.0f);
    glVertex3f(-1.0f, -0.6f, -1.0f);
    glVertex3f(1.0f, -0.6f, -1.0f);

    // Front face  (z = 1.0f)
   //  glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 0.6f, 1.0f);
    glVertex3f(-1.0f, -0.6f, 1.0f);
    glVertex3f(1.0f, -0.6f, 1.0f);

    // Back face (z = -1.0f)
    // glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -0.6f, -1.0f);
    glVertex3f(-1.0f, -0.6f, -1.0f);
    glVertex3f(-1.0f, 0.6f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    //  glColor3f(0.0f, 0.0f, 0.7f);     // Blue
    glVertex3f(-1.0f, 0.6f, 1.0f);
    glVertex3f(-1.0f, 0.6f, -1.0f);
    glVertex3f(-1.0f, -0.6f, -1.0f);
    glVertex3f(-1.0f, -0.6f, 1.0f);

    // Right face (x = 1.0f)
   
  //  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -0.6f, 1.0f);
    glVertex3f(1.0f, -0.6f, -1.0f);
    
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    
    // Upper neck
    glPushMatrix();
    glTranslatef(0.3, -1.3, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(20, 0,0,1);
    
    glBegin(GL_QUADS);
   
    // Top face
    //   glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    // Bottom face (y = -1.0f)
   // glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Front face  (z = 1.0f)
   // glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Back face (z = -1.0f)
   // glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Right face (x = 1.0f)
  //  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    

    // Upper neck 2
    glPushMatrix();
    glTranslatef(0.6, -2.2, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(20, 0,0,1);
    
    glBegin(GL_QUADS);
    // Top face
 //     glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.55f, 0.3f, -0.5f);
    glVertex3f(-0.55f, 0.3f, -0.5f);
    glVertex3f(-0.55f, 0.3f, 0.5f);
    glVertex3f(0.55f, 0.3f, 0.5f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.55f, -0.3f, 0.5f);
    glVertex3f(-0.55f, -0.3f, 0.5f);
    glVertex3f(-0.55f, -0.3f, -0.5f);
    glVertex3f(0.55f, -0.3f, -0.5f);

    // Front face  (z = 1.0f)
  //  glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.55f, 0.3f, 0.5f);
    glVertex3f(-0.55f, 0.3f, 0.5f);
    glVertex3f(-0.55f, -0.3f, 0.5f);
    glVertex3f(0.55f, -0.3f, 0.5f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.55f, -0.3f, -0.5f);
    glVertex3f(-0.55f, -0.3f, -0.5f);
    glVertex3f(-0.55f, 0.3f, -0.5f);
    glVertex3f(0.55f, 0.3f, -0.5f);

    // Left face (x = -1.0f)
   // glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.55f, 0.3f, 0.5f);
    glVertex3f(-0.55f, 0.3f, -0.5f);
    glVertex3f(-0.55f, -0.3f, -0.5f);
    glVertex3f(-0.55f, -0.3f, 0.5f);

    // Right face (x = 1.0f)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.55f, 0.3f, -0.5f);
    glVertex3f(0.55f, 0.3f, 0.5f);
    glVertex3f(0.55f, -0.3f, 0.5f);
    glVertex3f(0.55f, -0.3f, -0.5f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
   
    
    
    
    // Upper neck 3
    glPushMatrix();
    
    glTranslatef(0.9, -3.0, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(20, 0,0,1);
    
    glBegin(GL_QUADS);
    // Top face
   //   glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f, 0.3f, -0.5f);
    glVertex3f(-0.6f, 0.3f, -0.5f);
    glVertex3f(-0.6f, 0.3f, 0.5f);
    glVertex3f(0.6f, 0.3f, 0.5f);

    // Bottom face (y = -1.0f)
 //   glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.6f, -0.3f, 0.5f);
    glVertex3f(-0.6f, -0.3f, 0.5f);
    glVertex3f(-0.6f, -0.3f, -0.5f);
    glVertex3f(0.6f, -0.3f, -0.5f);

    // Front face  (z = 1.0f)
 //   glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f, 0.3f, 0.5f);
    glVertex3f(-0.6f, 0.3f, 0.5f);
    glVertex3f(-0.6f, -0.3f, 0.5f);
    glVertex3f(0.6f, -0.3f, 0.5f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.6f, -0.3f, -0.5f);
    glVertex3f(-0.6f, -0.3f, -0.5f);
    glVertex3f(-0.6f, 0.3f, -0.5f);
    glVertex3f(0.6f, 0.3f, -0.5f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f, 0.3f, 0.5f);
    glVertex3f(-0.6f, 0.3f, -0.5f);
    glVertex3f(-0.6f, -0.3f, -0.5f);
    glVertex3f(-0.6f, -0.3f, 0.5f);

    // Right face (x = 1.0f)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f, 0.3f, -0.5f);
    glVertex3f(0.6f, 0.3f, 0.5f);
    glVertex3f(0.6f, -0.3f, 0.5f);
    glVertex3f(0.6f, -0.3f, -0.5f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
   
    
   
    // Torso 1
    glPushMatrix();
    glTranslatef(0.9, -4.3, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(60, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
  //    glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.5f*3.5, 0.3f*2.5, -2.0f);
    glVertex3f(-0.5f*3.5, 0.3f*2.5, -2.0f);
    glVertex3f(-0.5f*3.5, 0.3f*2.5, 2.0f);
    glVertex3f(0.5f*3.5, 0.3f*2.5, 2.0f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.6f*3.5, -0.3f*2.5, 2.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, 2.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, -2.0f);
    glVertex3f(0.6f*3.5, -0.3f*2.5, -2.0f);

    // Front face  (z = 1.0f)
 //   glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.5f*3.5, 0.3f*2.5, 2.0f);
    glVertex3f(-0.5f*3.5, 0.3f*2.5, 2.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, 2.0f);
    glVertex3f(0.6f*3.5, -0.3f*2.5, 2.0f);

    // Back face (z = -1.0f)
   // glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.6f*3.5, -0.3f*2.5, -2.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, -2.0f);
    glVertex3f(-0.5f*3.5, 0.3f*2.5, -2.0f);
    glVertex3f(0.5f*3.5, 0.3f*2.5, -2.0f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.5f*3.5, 0.3f*2.5, 1.0f);
    glVertex3f(-0.5f*3.5, 0.3f*2.5, -1.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, -1.0f);
    glVertex3f(-0.6f*3.5, -0.3f*2.5, 1.0f);

    // Right face (x = 1.0f)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.5f*3.5, 0.3f*2.5, -2.0f);
    glVertex3f(0.5f*3.5, 0.3f*2.5, 2.0f);
    glVertex3f(0.6f*3.5, -0.3f*2.5, 2.0f);
    glVertex3f(0.6f*3.5, -0.3f*2.5, -2.0f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    
    // Torso 2
    glPushMatrix();
    glTranslatef(2.3, -5.1, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(60, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
   // glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*3.5, 0.3f*3, -2.0f);
    glVertex3f(-0.6f*3.5, 0.3f*3, -2.0f);
    glVertex3f(-0.6f*3.5, 0.3f*3, 2.0f);
    glVertex3f(0.6f*3.5, 0.3f*3, 2.0f);

    // Bottom face (y = -1.0f)
 //   glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.7f*3.5, -0.3f*3, 2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, 2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, -2.0f);
    glVertex3f(0.7f*3.5, -0.3f*3, -2.0f);

    // Front face  (z = 1.0f)
  //  glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*3.5, 0.3f*3, 2.0f);
    glVertex3f(-0.6f*3.5, 0.3f*3, 2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, 2.0f);
    glVertex3f(0.7f*3.5, -0.3f*3, 2.0f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.7f*3.5, -0.3f*3, -2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, -2.0f);
    glVertex3f(-0.6f*3.5, 0.3f*3, -2.0f);
    glVertex3f(0.6f*3.5, 0.3f*3, -2.0f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*3.5, 0.3f*3, 2.0f);
    glVertex3f(-0.6f*3.5, 0.3f*3, -2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, -2.0f);
    glVertex3f(-0.0f*3.5, -0.8f*3, 2.0f);

    // Right face (x = 1.0f)
 //   glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*3.5, 0.3f*3, -2.0f);
    glVertex3f(0.6f*3.5, 0.3f*3, 2.0f);
    glVertex3f(0.7f*3.5, -0.3f*3, 2.0f);
    glVertex3f(0.7f*3.5, -0.3f*3, -2.0f);
    glEnd();  // End of drawing color-cube
    
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
  //  glDisable(GL_CULL_FACE);
    
    
    // rib 1
    glPushMatrix();
    glTranslatef(5.4, -4.9, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(90, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
   //   glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*2.3, 0.3f*2.0, -1.9f);
    glVertex3f(-0.6f*2.3, 0.3f*2.0, -1.9f);
    glVertex3f(-0.6f*2.3, 0.3f*2.0, 1.9f);
    glVertex3f(0.6f*2.3, 0.3f*2.0, 1.9f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.6f*2.3, -0.3f*2.0, 1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, 1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, -1.9f);
    glVertex3f(0.6f*2.3, -0.3f*2.0, -1.9f);

    // Front face  (z = 1.0f)
   // glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*2.3, 0.3f*2.0, 1.9f);
    glVertex3f(-0.6f*2.3, 0.3f*2.0, 1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, 1.9f);
    glVertex3f(0.6f*2.3, -0.3f*2.0, 1.9f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.6f*2.3, -0.3f*2.0, -1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, -1.9f);
    glVertex3f(-0.6f*2.3, 0.3f*2.0, -1.9f);
    glVertex3f(0.6f*2.3, 0.3f*2.0, -1.9f);

    // Left face (x = -1.0f)
   // glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*2.3, 0.3f*2.0, 1.9f);
    glVertex3f(-0.6f*2.3, 0.3f*2.0, -1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, -1.9f);
    glVertex3f(-0.5f*2.3, -0.3f*2.0, 1.9f);

    // Right face (x = 1.0f)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*2.3, 0.3f*2.0, -1.9f);
    glVertex3f(0.6f*2.3, 0.3f*2.0, 1.9f);
    glVertex3f(0.6f*2.3, -0.3f*2.0, 1.9f);
    glVertex3f(0.6f*2.3, -0.3f*2.0, -1.9f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    
    // rib 2
    glPushMatrix();
    glTranslatef(6.6, -4.7, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(90, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
  //    glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*2.0, 0.3f*1.2, -1.9f);
    glVertex3f(-0.6f*2.0, 0.3f*1.2, -1.9f);
    glVertex3f(-0.6f*2.0, 0.3f*1.2, 1.9f);
    glVertex3f(0.6f*2.0, 0.3f*1.2, 1.9f);

    // Bottom face (y = -1.0f)
 //   glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.6f*2.0, -0.3f*1.2, 1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, 1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, -1.9f);
    glVertex3f(0.6f*2.0, -0.3f*1.2, -1.9f);

    // Front face  (z = 1.0f)
//    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*2.0, 0.3f*1.2, 1.9f);
    glVertex3f(-0.6f*2.0, 0.3f*1.2, 1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, 1.9f);
    glVertex3f(0.6f*2.0, -0.3f*1.2, 1.9f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.6f*2.0, -0.3f*1.2, -1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, -1.9f);
    glVertex3f(-0.6f*2.0, 0.3f*1.2, -1.9f);
    glVertex3f(0.6f*2.0, 0.3f*1.2, -1.9f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*2.0, 0.3f*1.2, 1.9f);
    glVertex3f(-0.6f*2.0, 0.3f*1.2, -1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, -1.9f);
    glVertex3f(-0.5f*2.0, -0.3f*1.2, 1.9f);

    // Right face (x = 1.0f)
  //  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*2.0, 0.3f*1.2, -1.9f);
    glVertex3f(0.6f*2.0, 0.3f*1.2, 1.9f);
    glVertex3f(0.6f*2.0, -0.3f*1.2, 1.9f);
    glVertex3f(0.6f*2.0, -0.3f*1.2, -1.9f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    
    // rib 3
    glPushMatrix();
    glTranslatef(7.5, -4.5, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(90, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
   //   glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*1.8, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.8, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.8, 0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.8, 0.3f*0.9, 1.9f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.6f*1.8, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.8, -0.3f*0.9, -1.9f);

    // Front face  (z = 1.0f)
   // glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*1.8, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.8, 0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.8, -0.3f*0.9, 1.9f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.6f*1.8, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.8, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.8, 0.3f*0.9, -1.9f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*1.8, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.8, 0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.8, -0.3f*0.9, 1.9f);

    // Right face (x = 1.0f)
  //  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*1.8, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.8, 0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.8, -0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.8, -0.3f*0.9, -1.9f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    // rib 4
    glPushMatrix();
    glTranslatef(8.2, -4.3, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(90, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
 //     glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*1.5, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.5, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.5, 0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.5, 0.3f*0.9, 1.9f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.55f*1.5, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, -1.9f);
    glVertex3f(0.55f*1.5, -0.3f*0.9, -1.9f);

    // Front face  (z = 1.0f)
 //   glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*1.5, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.5, 0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, 1.9f);
    glVertex3f(0.55f*1.5, -0.3f*0.9, 1.9f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.55f*1.5, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.5, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.5, 0.3f*0.9, -1.9f);

    // Left face (x = -1.0f)
  //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*1.5, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.5, 0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.5, -0.3f*0.9, 1.9f);

    // Right face (x = 1.0f)
//    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*1.5, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.5, 0.3f*0.9, 1.9f);
    glVertex3f(0.55f*1.5, -0.3f*0.9, 1.9f);
    glVertex3f(0.55f*1.5, -0.3f*0.9, -1.9f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    // tail holder
    glPushMatrix();
    glTranslatef(8.8, -4.2, 0);
    
    glRotatef(0, 1,0,0);
    glRotatef(0, 0,1,0); // angle, x, y, c
    glRotatef(90, 0,0,1);
    
    glBegin(GL_QUADS);
    
    // Top face
  //    glColor3f(0.5f, 0.35f, 0.05f);     // Green
    glVertex3f(0.6f*1.2, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.2, 0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.2, 0.3f*0.9, 1.9f);
    glVertex3f(0.6f*1.2, 0.3f*0.9, 1.9f);

    // Bottom face (y = -1.0f)
  //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0.4f*1.2, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, -1.9f);
    glVertex3f(0.4f*1.2, -0.3f*0.9, -1.9f);

    // Front face  (z = 1.0f)
   // glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.6f*1.2, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.2, 0.3f*0.9, 1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, 1.9f);
    glVertex3f(0.4f*1.2, -0.3f*0.9, 1.9f);

    // Back face (z = -1.0f)
  //  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(0.4f*1.2, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, -1.9f);
    glVertex3f(-0.6f*1.2, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.2, 0.3f*0.9, -1.9f);

    // Left face (x = -1.0f)
   // glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.6f*1.2, 0.3f*0.9, 1.9f);
    glVertex3f(-0.6f*1.2, 0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, -1.9f);
    glVertex3f(-0.5f*1.2, -0.3f*0.9, 1.9f);

    // Right face (x = 1.0f)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.6f*1.2, 0.3f*0.9, -1.9f);
    glVertex3f(0.6f*1.2, 0.3f*0.9, 1.9f);
    glVertex3f(0.4f*1.2, -0.3f*0.9, 1.9f);
    glVertex3f(0.4f*1.2, -0.3f*0.9, -1.9f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    glPushMatrix();
    
    // rear leg 1 a
    glPushMatrix();
    glColor3f(0.6f, 0.45f, 0.05f);     // Brown
    
        glTranslatef(7.8, -3.5, 2.2);

        glRotatef(lb1rotAngle, 0,0,1);
       
        glTranslatef(-1, -1, -1); // Move anchor point
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glTranslatef(1, 1, 1);
        
        glBegin(GL_QUADS);
        
        // Top face
        //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);

        // Bottom face (y = -1.0f)
      //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //  glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);

        // Front face  (z = 1.0f)
        //glColor3f(1.0f, 0.0f, 0.0f);     // Red
     //   glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
     //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glEnd();  // End of drawing color-cube
    glPopMatrix();
    
    
    
    // front leg 1 a
    glPushMatrix();
 //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
    
        glTranslatef(1.8, -3.5, 2.2);

        glRotatef(lb1rotAngle, 0,0,1);
       
        glTranslatef(-1, -1, -1); // Move anchor point
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glTranslatef(1, 1, 1);
        
        glBegin(GL_QUADS);
        
        // Top face
        //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);

        // Bottom face (y = -1.0f)
      //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //  glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);

        // Front face  (z = 1.0f)
        //glColor3f(1.0f, 0.0f, 0.0f);     // Red
     //   glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
     //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glEnd();  // End of drawing color-cube
        glPopMatrix();
    glPopMatrix();
    
    
    
    
    
    
      //  glPopMatrix();
        
        /*
        
        // front leg 1 b
      //  glPushMatrix();
      //  glTranslatef(6.9, -8.3, 2.2);
       // glTranslatef(7.4, -6.3, 2.2);
    
        glRotatef(lb1rotAngleOpposite, 0,0,1);
   
        glTranslatef(1, 1, 1); // Move anchor point
    
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
    
        glTranslatef(-1, -1, -1);
    
        glBegin(GL_QUADS);
        
        // Top face
     //     glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.4f*0.7, 0.3f*0.7, -1.4f);
        glVertex3f(-0.4f*0.7, 0.3f*0.7, -1.4f);
        glVertex3f(-0.25f*0.7, 0.3f*0.7, 1.4f);
        glVertex3f(0.25f*0.7, 0.3f*0.7, 1.4f);

        // Bottom face (y = -1.0f)
     //   glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f(0.25f*0.7, -0.3f*0.7, 1.4f);
        glVertex3f(-0.25f*0.7, -0.3f*0.7, 1.4f);
        glVertex3f(-0.4f*0.7, -0.3f*0.7, -1.4f);
        glVertex3f(0.4f*0.7, -0.3f*0.7, -1.4f);

        // Front face  (z = 1.0f)
     //   glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.25f*0.7, 0.3f*0.7, 1.4f);
        glVertex3f(-0.25f*0.7, 0.3f*0.7, 1.4f);
        glVertex3f(-0.25f*0.7, -0.3f*0.7, 1.4f);
        glVertex3f(0.25f*0.7, -0.3f*0.7, 1.4f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f(0.4f*0.7, -0.3f*0.7, -1.4f);
        glVertex3f(-0.4f*0.7, -0.3f*0.7, -1.4f);
        glVertex3f(-0.4f*0.7, 0.3f*0.7, -1.4f);
        glVertex3f(0.4f*0.7, 0.3f*0.7, -1.4f);

        // Left face (x = -1.0f)
      //  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*0.7, 0.3f*0.7, 1.4f);
        glVertex3f(-0.4f*0.7, 0.3f*0.7, -1.4f);
        glVertex3f(-0.4f*0.7, -0.3f*0.7, -1.4f);
        glVertex3f(-0.25f*0.7, -0.3f*0.7, 1.4f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.4f*0.7, 0.3f*0.7, -1.4f);
        glVertex3f(0.25f*0.7, 0.3f*0.7, 1.4f);
        glVertex3f(0.25f*0.7, -0.3f*0.7, 1.4f);
        glVertex3f(0.4f*0.7, -0.3f*0.7, -1.4f);
        glEnd();  // End of drawing color-cube
        glPopMatrix();
        
        

        // front leg 1 c
        glPushMatrix();
        glTranslatef(6.7, -9.0, 2.2);
        
        glRotatef(90, 1,0,0);
        glRotatef(-70, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glBegin(GL_QUADS);
        
        // Top face
    //      glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.3f*0.7, 0.3f*0.7, -0.9f);
        glVertex3f(-0.3f*0.7, 0.3f*0.7, -0.9f);
        glVertex3f(-0.15f*0.7, 0.3f*0.7, 0.9f);
        glVertex3f(0.15f*0.7, 0.3f*0.7, 0.9f);

        // Bottom face (y = -1.0f)
    //    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f(0.15f*0.7, -0.3f*0.7, 0.9f);
        glVertex3f(-0.15f*0.7, -0.3f*0.7, 0.9f);
        glVertex3f(-0.3f*0.7, -0.3f*0.7, -0.9f);
        glVertex3f(0.3f*0.7, -0.3f*0.7, -0.9f);

        // Front face  (z = 1.0f)
   //     glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.15f*0.7, 0.3f*0.7, 0.9f);
        glVertex3f(-0.15f*0.7, 0.3f*0.7, 0.9f);
        glVertex3f(-0.15f*0.7, -0.3f*0.7, 0.9f);
        glVertex3f(0.15f*0.7, -0.3f*0.7, 0.9f);

        // Back face (z = -1.0f)
    //    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f(0.3f*0.7, -0.3f*0.7, -0.9f);
        glVertex3f(-0.3f*0.7, -0.3f*0.7, -0.9f);
        glVertex3f(-0.3f*0.7, 0.3f*0.7, -0.9f);
        glVertex3f(0.3f*0.7, 0.3f*0.7, -0.9f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.15f*0.7, 0.3f*0.7, 0.9f);
        glVertex3f(-0.3f*0.7, 0.3f*0.7, -0.9f);
        glVertex3f(-0.3f*0.7, -0.3f*0.7, -0.9f);
        glVertex3f(-0.15f*0.7, -0.3f*0.7, 0.9f);

        // Right face (x = 1.0f)
     //   glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.3f*0.7, 0.3f*0.7, -0.9f);
        glVertex3f(0.15f*0.7, 0.3f*0.7, 0.9f);
        glVertex3f(0.15f*0.7, -0.3f*0.7, 0.9f);
        glVertex3f(0.3f*0.7, -0.3f*0.7, -0.9f);
        glEnd();  // End of drawing color-cube
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);
        
        
        
        // feet back 1
        glPushMatrix();
        glTranslatef(5.8, -9.4, 2.2);
        
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glBegin(GL_QUADS);
        
        // Top face
      //    glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.3f*1.2, 0.3f*0.7, -0.2f);
        glVertex3f(-0.15f*1.2, 0.3f*0.7, -0.2f);
        glVertex3f(-0.3f*1.2, 0.3f*0.7, 0.2f);
        glVertex3f(0.3f*1.2, 0.3f*0.7, 0.2f);

        // Bottom face (y = -1.0f)
    //    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f(0.3f*1.2, -0.3f*0.7, 0.2f);
        glVertex3f(-0.3f*1.2, -0.3f*0.7, 0.2f);
        glVertex3f(-0.15f*1.2, -0.3f*0.7, -0.2f);
        glVertex3f(0.3f*1.2, -0.3f*0.7, -0.2f);

        // Front face  (z = 1.0f)
    //    glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.3f*1.2, 0.3f*0.7, 0.2f);
        glVertex3f(-0.3f*1.2, 0.3f*0.7, 0.2f);
        glVertex3f(-0.3f*1.2, -0.3f*0.7, 0.2f);
        glVertex3f(0.3f*1.2, -0.3f*0.7, 0.2f);

        // Back face (z = -1.0f)
    //    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f(0.3f*1.2, -0.3f*0.7, -0.2f);
        glVertex3f(-0.15f*1.2, -0.3f*0.7, -0.2f);
        glVertex3f(-0.15f*1.2, 0.3f*0.7, -0.2f);
        glVertex3f(0.3f*1.2, 0.3f*0.7, -0.2f);

        // Left face (x = -1.0f)
   //     glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.3f*1.2, 0.3f*0.7, 0.2f);
        glVertex3f(-0.15f*1.2, 0.3f*0.7, -0.2f);
        glVertex3f(-0.15f*1.2, -0.3f*0.7, -0.2f);
        glVertex3f(-0.3f*1.2, -0.3f*0.7, 0.2f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.3f*1.2, 0.3f*0.7, -0.2f);
        glVertex3f(0.3f*1.2, 0.3f*0.7, 0.2f);
        glVertex3f(0.3f*1.2, -0.3f*0.7, 0.2f);
        glVertex3f(0.3f*1.2, -0.3f*0.7, -0.2f);
        glEnd();  // End of drawing color-cube
        glPopMatrix();
         */
     //   glEnable(GL_DEPTH_TEST);
    
   
   
    
    
    // rear leg 2 a
    glPushMatrix();
 //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
    
        glTranslatef(7.8, -3.5, -2.2);

        glRotatef(lb2rotAngle, 0,0,1);
       
        glTranslatef(-1, -1, -1); // Move anchor point
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glTranslatef(1, 1, 1);
        
        glBegin(GL_QUADS);
        
        // Top face
        //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);

        // Bottom face (y = -1.0f)
      //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //  glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);

        // Front face  (z = 1.0f)
        //glColor3f(1.0f, 0.0f, 0.0f);     // Red
     //   glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
     //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glEnd();  // End of drawing color-cube
    
    glPopMatrix();
    
    
    
    // front leg 2 a
    glPushMatrix();
 //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
    
        glTranslatef(1.8, -3.5, -2.2);

        glRotatef(lb2rotAngle, 0,0,1);
       
        glTranslatef(-1, -1, -1); // Move anchor point
        glRotatef(90, 1,0,0);
        glRotatef(0, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glTranslatef(1, 1, 1);
        
        glBegin(GL_QUADS);
        
        // Top face
        //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);

        // Bottom face (y = -1.0f)
      //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //  glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);

        // Front face  (z = 1.0f)
        //glColor3f(1.0f, 0.0f, 0.0f);     // Red
     //   glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
     //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glEnd();  // End of drawing color-cube
    
    glPopMatrix();
    
    
    
    // tail
    glPushMatrix();
    
        glTranslatef(10.0, -3.5, 2.0);

       // glRotatef(lb2rotAngle, 0,0,1);
        glColor3f(0.4f, 0.25f, 0.01);
        glTranslatef(-1, -1, -1); // Move anchor point
        glRotatef(0, 1,0,0);
        glRotatef(90, 0,1,0); // angle, x, y, c
        glRotatef(0, 0,0,1);
        
        glTranslatef(1, 1, 1);
        
        glBegin(GL_QUADS);
        
        // Top face
        //  glColor3f(0.5f, 0.35f, 0.05f);     // Green
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);

        // Bottom face (y = -1.0f)
      //  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //  glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);

        // Front face  (z = 1.0f)
        //glColor3f(1.0f, 0.0f, 0.0f);     // Red
     //   glColor3f(0.4f, 0.25f, 0.01); // Dark Brown
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);

        // Back face (z = -1.0f)
     //   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
     //   glColor3f(0.5f, 0.35f, 0.05f);     // Brown
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);

        // Left face (x = -1.0f)
     //   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(-0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(-0.5f*1.0, -0.3f*0.7, -1.7f);
        glVertex3f(-0.25f*1.0, -0.3f*0.7, 1.7f);

        // Right face (x = 1.0f)
    //    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(0.5f*1.0, 0.3f*0.7, -1.7f);
        glVertex3f(0.25f*1.0, 0.3f*0.7, 1.7f);
        glVertex3f(0.25f*1.0, -0.3f*0.7, 1.7f);
        glVertex3f(0.5f*1.0, -0.3f*0.7, -1.7f);
        glEnd();  // End of drawing color-cube
    
    glPopMatrix();
    
    
    glPopMatrix();
  //  glDisable(GL_CULL_FACE);
    
    DrawGround();
//    gluDeleteQuadric(quadObj);
    glFlush();
}


void DrawGround() {
    glColor3d(0.5,0.5,0.5); // green ground
  glBegin(GL_LINES);
  
  for(float i =-1; i<=3; ) // 40 vertical lines
   {
       glVertex3f(i*100.f, -10.f, -100.f);
       glVertex3f(i*100.f, -10.f, 100.f);
       i+=0.1;
   }
  for(float j =-1; j<=3; ) // 40 horizontal lines
    {
       glVertex3f(-100.f, -10.f, j*100.f);
       glVertex3f(100.f, -10.f, j*100.f);
       j+=0.1;
    }
  glEnd();
}

void PitchUp(float n){
    m_ytheta+=n;
  if(m_ytheta > 360) m_ytheta=-360;
}

void PitchDown(float n){
    m_ytheta-=n;
  if(m_ytheta < -360) m_ytheta=360;
}

void YawUp(float n){
    m_xtheta+=n;
  if(m_xtheta > 360) m_xtheta=-360;
}

void YawDown(float n){
    m_xtheta-=n;
  if(m_xtheta < -360) m_xtheta=360;
}

void SlideUp(float n){
    m_slide+=n;
}

void SlideDown(float n ){
    m_slide-=n;
}

void Walk(float n){
    
    // Rear leg 1
    
    if (toggle == false) {
        // move forward
        for (double i=-10; i<=10; i=i+0.40) { // frame rate
            lb1rotAngle =  i;
            lb1rotAngleOpposite =  i;
            displace = displace - 0.02;
            display();

        }

        toggle = true;
        
    } else {
        // move backward
        if (toggle == true) {
            for (double i=10; i>=-10; i=i-0.40) { // frame rate
                lb1rotAngle =  i;
                lb1rotAngleOpposite =  i;
                displace = displace - 0.02;
                display();
            }

            toggle = false;
        }
    }

    // Rear leg 2
    
    if (toggle == false) {
        // move forward
        
        for (double i=20; i>=10; i=i-0.40) { // frame rate

            lb2rotAngle =  i + 10;
            displace = displace - 0.02;
                display();
        }
        
        toggle = true;
        
    } else {
        // move backward
        if (toggle == true) {
            for (double i=10; i<=20; i=i+0.40) { // frame rate
                
                lb2rotAngle = i + 10;
                displace = displace - 0.02;
                display();
            }
            
            toggle = false;
        }
    }
    
    
}
