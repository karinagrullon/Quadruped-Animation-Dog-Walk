//
//  main.cpp
//  CS560-Term-Project
//
//  Created by Karina Grullon on 5/21/21.
//

#include"menu.hpp"
#include"display.hpp"
#include"util.h"


//glutMainLoop never return;
int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1000,700);
    glutInitWindowPosition(300,50);
    glutCreateWindow("Final Term Project");
    
        setup();
        
     // initializing callbacks
     glutReshapeFunc(reshape);
     glutDisplayFunc(display);
     glutMouseFunc(mouse);
     glutMotionFunc(motion);
     
     glutSpecialFunc(specialKey); //register a special keyboard function.
    //Creates Menu on Right Click
      CreateMenu();
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glutMainLoop();
     return 0;

}

