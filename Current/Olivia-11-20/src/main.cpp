/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// LeftMotor            motor         1               
// RightMotor           motor         2               
// Arm                  motor         3               
// Intake               motor         4               
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

/*
type
*/

int armPos() {
  return Arm.position(degrees);
}

void moveDrive(int numTurns) {
  // Motor.spinFor(direction, value, the unit of the value, waitUntilStopping)
  LeftMotor.spinFor(forward, numTurns, turns, false);
  RightMotor.spinFor(forward, numTurns, turns);
}

void turnDrive(float amt, int v) { 
  //Motor.spinFor   direction, amt, amtUnit, velocity, velocityUnit, waitUntilStopping
  LeftMotor.spinFor(forward, amt, turns, v, velocityUnits::pct, false);
  RightMotor.spinFor(forward, -amt, turns, v, velocityUnits::pct);
} 

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  // ----
  LeftMotor.spinFor(forward, 10, turns);
  RightMotor.spinFor(forward, 10, turns);

  LeftMotor.spinFor(reverse, -10, turns);
  RightMotor.spinFor(reverse, -10, turns);

  moveDrive(10);
  moveDrive(-10);

  turnDrive(2.5, 80);

  // ----
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

bool driveIsForward = true;

void toggleDriveDirection() {
  driveIsForward = !driveIsForward;
}


void moveArmUp() {
  // Move Arm to 80 degrees
  Arm.spinToPosition(80,degrees);

}

void moveArmDown() {
  // Move Arm to 0 degrees
  Arm.spinToPosition(0,degrees);
}

void stopArm() {
  // Stop Arm using Arm.stop()
  Arm.stop();
}

void usercontrol(void) {
  // User control code here, inside the loop
  // Tank Drive
  LeftMotor.spin(fwd, Controller1.Axis3.position(), pct);
  RightMotor.spin(fwd, Controller1.Axis2.position(), pct);

  
  

  Controller1.ButtonB.pressed(toggleDriveDirection);
  Controller1.ButtonA.pressed(moveArmUp);
  Controller1.ButtonB.pressed(moveArmDown);
  Controller1.ButtonX.pressed(stopArm);
  /*
    Using callbacks,
    if button1 is pressed, move arm up to 80 degrees 
    if button2 is pressed, move arm to 0 degrees 
    if button3 is pressed, stop arm.
  */




  bool BB_old = false;
  while (1) {
    bool BB = Controller1.ButtonB.pressing();
    bool BA = Controller1.ButtonA.pressing();

    if(BB) {
      Intake.spin(fwd, 80, pct);
    } else if(BA) {
      Intake.spin(reverse, 80, pct);
    } else {
      Intake.stop();
    }
    
    if(BB && !BB_old) {
      driveIsForward = !driveIsForward;
    }


    BB_old = BB;

    /*
    bool (takes a true or false value)
    */

    // (data type) (identifier) (=) (value)
    int num1 = 30; // num1 != Num1
    int Num1 = 40; // Capitalization matters! 
    int num_2;
    float 3f = 4.0; // Cannot start identifier w/number
    bool forward& positive = true; // Cannot have special characters or spaces.
    char letter = 'E';


    

    // To move arm up
    bool R2 = Controller1.ButtonR2.pressing(); 
    // To move arm down
    bool R1 = Controller1.ButtonR1.pressing();

    // Not pringle, donut.


    // Tank Drive w/variables
    int J2 = Controller1.Axis2.position(); // Right vertical axis 
    int J3 = Controller1.Axis2.position(); // Left vertical axis

    RightMotor.spin(fwd, J2, pct);
    LeftMotor.spin(fwd, J3, pct);

    // Tank Drive w/o variables
    RightMotor.spin(fwd, Controller1.Axis2.position(), pct);
    LeftMotor.spin(fwd, Controller1.Axis3.position(), pct);

    // Motor.spin(direction, velocity, velocityUnit);
    /*
    if(R2) {

    }
    alll the else ifs

    else 
    */

    // 'A' 'B' 'C' 'D'

    

   

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
