/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
float clampVal;
bool clampValSet = false;
bool isArcade = true;

void setClampVal() {
  Controller1.rumble(rumbleShort);
  if(!clampValSet) clampVal = Clamp.position(degrees);
  clampValSet = !clampValSet;
}

void moveClamp() {
  if(clampValSet) {
    if(Clamp.position(degrees) < 300) {
      Clamp.spinToPosition(clampVal, degrees, 70, velocityUnits::pct);
    } else {
      Clamp.spinToPosition(0, degrees, 70, velocityUnits::pct);
    }
  }
}

void toggleDrive() {
  Controller1.rumble(rumbleShort);
  isArcade = !isArcade;
}


void usercontrol(void) {
  // User control code here, inn   nnside the loop
  Controller1.Screen.clearScreen();
  Controller1.ButtonX.pressed(setClampVal);
  Controller1.ButtonL1.pressed(moveClamp);
  Controller1.ButtonRight.pressed(toggleDrive);

  Clamp.setStopping(hold);
  FourBar.setStopping(hold);

  LeftBack.setStopping(brake);
  LeftFront.setStopping(brake);
  RightBack.setStopping(brake);
  RightFront.setStopping(brake);
  while (1) {
    int J1 = Controller1.Axis1.position();
    int J2 = Controller1.Axis2.position();
    int J3 = Controller1.Axis3.position();
    int J4 = Controller1.Axis4.position();
    
    // Controller Right Button Group Vals
    bool BX = Controller1.ButtonX.pressing();
    bool BY = Controller1.ButtonY.pressing();
    bool BA = Controller1.ButtonA.pressing(); 
    bool BB = Controller1.ButtonB.pressing();

    // Controller Left Button Group Vals
    bool BU = Controller1.ButtonUp.pressing();
    bool BL = Controller1.ButtonLeft.pressing();
    bool BR = Controller1.ButtonRight.pressing();
    bool BD = Controller1.ButtonDown.pressing();

    // Controller Shoulder Button Group Vals
    bool R1 = Controller1.ButtonR1.pressing(); // Right bumper closest to user
    bool R2 = Controller1.ButtonR2.pressing();
    bool L1 = Controller1.ButtonL1.pressing(); // Left bumper closest to user
    bool L2 = Controller1.ButtonL2.pressing();


    // Arcade Drive
    if(isArcade) {
      LeftBack.spin(fwd, J2+J1, pct);
      LeftFront.spin(fwd, J2+J1, pct);
      RightBack.spin(fwd, J2-J1, pct);
      RightFront.spin(fwd, J2-J1, pct);
    } else {
      if(BU) {
        LeftBack.spin(fwd, 5, pct);
      } else if(BD) {
        LeftFront.spin(fwd, 5, pct);
      } else {
        LeftBack.spin(fwd, J3, pct);
        //LeftFront.spin(fwd, J3, pct);
        RightBack.spin(fwd, J2, pct);
        RightFront.spin(fwd, J2, pct);
      }
    }
    
    

    // Clamp
    if(!clampValSet) {
      if(L2) {
        Clamp.spin(fwd, 70, pct);
      } else if(L1) {
        Clamp.spin(fwd, -70, pct);
      } else {
        Clamp.stop();
      }
    } 

    // FourBar (4-Bar)
    if(R1) {
      FourBar.spin(fwd, 70, pct);
    } else if(R2) {
      FourBar.spin(fwd, -70, pct);
    } else {
      FourBar.stop();
    }

    // Printing to the Controller
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("BX: Auto Clamp ");
    Controller1.Screen.print((clampValSet? "(On) " : "(Off)"));
    Controller1.Screen.setCursor(2,1);
    Controller1.Screen.print("BR: Drive ");
    Controller1.Screen.print((isArcade?"(Arcade)":"(Tank)  "));
    Controller1.Screen.setCursor(3,1);
    Controller1.Screen.print("L-Clamp | R-Arm");

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
