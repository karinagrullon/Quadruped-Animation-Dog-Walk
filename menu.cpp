//
//  menu.cpp
//  CS560-Term-Project
//
//  Created by Karina Grullon on 5/21/21.
//

#include "menu.hpp"
#include"util.h"
#include"display.hpp"

#define ANGLE 3
#define WALKANGLE 5

bool YawBool=false;
bool SlideBool=false;
bool PitchBool=false;
bool BoolKey = true;
bool BoolWalk = false;

//bool toggle = false;
//int walkAngle = false;

void mouse(int button, int state, int x, int y){
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if(state==GLUT_DOWN){
                
            //do something when left mouse button is down
                /*
                if (toggle == false) {
                    lb1rotAngle = lb1rotAngle+10;
                    toggle = true;
                } else {
                    
                    if (toggle == true) {
                        lb1rotAngle = lb1rotAngle-10;
                        toggle = false;
                    }
                }
                 */
        }

            break;
        case GLUT_RIGHT_BUTTON:
            break;
        case GLUT_MIDDLE_BUTTON:
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
void motion( int x, int y ){
   // record the position of the mouse.
    glutPostRedisplay();
}

//create menu
void CreateMenu(void){
 
    glutCreateMenu(MenuItemClicked);
    glutAddMenuEntry("Slide+", 1);
    glutAddMenuEntry("Slide-", 2);
    glutAddMenuEntry("Pitch+", 3);
    glutAddMenuEntry("Pitch-", 4);
    glutAddMenuEntry("Yaw+", 5);
    glutAddMenuEntry("Yaw-", 6);
    glutAddMenuEntry("Walk", 7);
  //Attach Menu to RightClick
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void MenuItemClicked(int Value){
    /*
    if (toggle == false) {
        walkAngle = walkAngle+3;
        toggle = true;
    } else {
        
        if (toggle == true) {
            walkAngle = walkAngle-3;
            toggle = false;
        }
    }
    */
    
    switch(Value)
    {
        case 1:
      SlideBool=true;
      PitchBool=false;
      YawBool=false;
      BoolWalk=false;
      SlideUp(ANGLE);
          break;
        case 2:
      SlideBool=true;
      PitchBool=false;
      YawBool=false;
      BoolWalk=false;
      SlideDown(ANGLE);
            break;
        case 3:
      SlideBool=false;
      PitchBool=true;
      YawBool=false;
      BoolWalk=false;
      PitchUp(ANGLE);
          break;
        case 4:
      SlideBool=false;
      PitchBool=true;
      YawBool=false;
      BoolWalk=false;
      PitchDown(ANGLE);
            break;
        case 5:
      SlideBool=false;
      PitchBool=false;
      YawBool=true;
      BoolWalk=false;
      PitchUp(ANGLE);
          break;
        case 6:
      SlideBool=false;
      PitchBool=false;
      BoolWalk=false;
      YawBool = true;
      PitchDown(ANGLE);
        case 7:
      SlideBool=false;
      PitchBool=false;
      BoolWalk=true;
      YawBool = false;
      Walk(WALKANGLE);
    break;
            
    }
}

void SubMenuItemClicked(int Value){
     //glutPostRedisplay();
}

void specialKey(int key, int x, int y){
    switch(key){
      case GLUT_KEY_UP:
          if(BoolKey) YawUp(ANGLE);
      break;
      case GLUT_KEY_DOWN:
          if(BoolKey) YawDown(ANGLE);
      break;
      case GLUT_KEY_LEFT:
        if(BoolKey) PitchUp(ANGLE);
        break;
        case GLUT_KEY_RIGHT:
        if(BoolKey) PitchDown(ANGLE);
        break;
        case 'k':
          if(BoolKey) SlideUp(ANGLE);
        break;
      case 'l':
          if(BoolKey) SlideDown(ANGLE);
      break;
        case 'p':
            if(BoolKey) Walk(WALKANGLE);
        break;
  }
    glutPostRedisplay();
}
