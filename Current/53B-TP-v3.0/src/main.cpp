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

typedef void (*AutonFunction) ();

// A global instance of competition
competition Competition;

/* 
NEW LAYOUT:
BackHook1:
OLD LAYOUT:
4-Bar: Right up and down SHOULDER - CHECK
FrontHook - BUTTON -
BackHook1 -  Up and Down BUTTONS - 
BackHook2 - Left up and down SHOULDER - 



//*/

void presets() {
  LeftDriveB.setStopping(brake); // brake / coast / hold
  LeftDriveF.setStopping(brake);
  RightDriveB.setStopping(brake);
  RightDriveF.setStopping(brake);

  Arm.setStopping(hold);
  FrontHook.setStopping(hold);
  BackHook1.setStopping(hold);
  BackHook2.setStopping(hold);
}

// ----GLOBALS----
  // ----CONTROLLER VALUES----
    int J1 = 0;
    int J2 = 0;
    int J3 = 0;
    int J4 = 0;
    
    // Controller Right Button Group Vals
    bool BX = false;
    bool BY = false;
    bool BA = false; 
    bool BB = false;

    // Controller Left Button Group Vals
    bool BU = false;
    bool BL = false;
    bool BR = false;
    bool BD = false;

    // Controller Shoulder Button Group Vals
    bool R1 = false; // Right bumper closest to user
    bool R2 = false;
    bool L1 = false; // Left bumper closest to user
    bool L2 = false;
  // ----CONTROLLER VALUES----
  float convertHook(float desired);
  float frontHookPos = convertHook(100);
  float hookVelocity = 80;
// ----GLOBALS----


// ----AUTONOMOUS GLOBALS----
  int autonChoice = 1;
  const double dDist = sqrt(1152);
  const double sDist = 24;
  const double wCirc = 4*M_PI;
  
// ----AUTONOMOUS GLOBALS----


// ----PREAUTONOMOUS FUNCTIONS----
  void selectAuton() {

  }
// ----PREAUTONOMOUS FUNCTIONS----

// ----HELPER FUNCTIONS----
  float convertHook(float desired) {
    return desired * 7;
  }
// ----HELPER FUNCTIONS



// ----USERCONTROL GLOBALS----
  bool hookStopping = false;
  int hookRecalibrated = false;
  bool armCalibrating = false;
  bool hookIsDown = false;
  int printChoice = 1;
  bool defaultDrive = true;
// ----USERCONTROL GLOBALS----

// ----USERCONTROL FUNCTIONS----
  float getDriveOutput(int val) {
    return val*exp(((abs(val)-100)*16)/1000);
  }

  

  void driveFunction() {
    RightDriveB.spin(fwd,defaultDrive? getDriveOutput(J2):-getDriveOutput(J3),pct);
    RightDriveF.spin(fwd,defaultDrive? getDriveOutput(J2):-getDriveOutput(J3),pct);

    LeftDriveB.spin(fwd,defaultDrive?getDriveOutput(J3):-getDriveOutput(J2),pct);
    LeftDriveF.spin(fwd,defaultDrive?getDriveOutput(J3):-getDriveOutput(J2),pct);
  }

  void armFunction() {
    if(armCalibrating && Arm.isDone()) {
      armCalibrating = false;
    }

    if(R1) {
      Arm.spin(fwd, 70, pct);
    } else if(R2) {
      Arm.spin(fwd, -70, pct);
    } else if(!armCalibrating){
      Arm.stop();
    }
    //* 
    if(Arm.position(degrees) < 70) {
      Controller1.rumble(rumbleShort);
      armCalibrating = true;
      Arm.spinToPosition(98, degrees, 70, velocityUnits::pct);
    }
    //*/
  }

  void frontHookFunction() {
    hookIsDown = !hookIsDown;
    FrontHook.spinToPosition(hookIsDown? frontHookPos : 0, deg, hookVelocity, velocityUnits::pct);
  }

  void backHook1Function() {
    if(L2) {
      BackHook1.spin(fwd, 80, pct);
    } else if(L1) {
      BackHook1.spin(fwd, -80, pct);
    } else {
      BackHook1.stop();
    }
  }

  void backHook2Function() {
    if(BU) {
      BackHook2.spin(fwd, 20, pct);
    } else if(BD) {
      BackHook2.spin(fwd, -20, pct);
    } else {
      BackHook2.stop();
    }
  }

  void togglePrint() {
    if(printChoice < 3) {
      printChoice++;
    } else {
      printChoice = 1;
    }
    Controller1.rumble(rumbleShort);
  }

  void printFunction() {
    switch(printChoice) {
        case 3:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("Torques:                       ");
          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm: %.1f         ", Arm.torque());

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("FH/BH1/BH2: %.2f/", FrontHook.torque());
          Controller1.Screen.print("%.2f/", BackHook1.torque());
          Controller1.Screen.print("%.2f                             ", BackHook2.torque());
          break;
        case 2:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("Positions:                ");

          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm: %.2f°                ", Arm.position(degrees));

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("FH/BH1/BH2: %.2f/", FrontHook.position(degrees));
          Controller1.Screen.print("%.2f/", BackHook1.position(degrees));
          Controller1.Screen.print("%.2f                             ", BackHook2.position(degrees));
          break;
        
        case 1:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("-Y--Driver's Key--Y-        ");

          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm: R1-R2 | Drive: A        ", Arm.position(degrees));

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("FH/BH1/BH2: BX/BU-BD/L1-L2     ");
          break;

        default:
          Controller1.Screen.clearScreen();
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("ERROR SEE GAVIN     ");
          break;
      }
  }

  void switchFront() {
    defaultDrive = !defaultDrive;
    Controller1.rumble(rumbleShort);
  }


// ----USERCONTROL FUNCTIONS----

// ----IMPORTANT FUNCTIONS----
  void ucSetup() {
    Controller1.Screen.clearScreen();
  }
// ----IMPORTANT FUNCTIONS----


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
  presets();




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

// ----AUTONOMOUS HELPER FUNCTIONS----
  void moveDrive(double dist, int v) {
    double amt = dist / (4*M_PI);
    LeftDriveB.spinFor(fwd, amt, rev, v, velocityUnits::pct, false); 
    LeftDriveF.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    RightDriveB.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    RightDriveF.spinFor(fwd, amt, rev, v, velocityUnits::pct);
  }

  void moveDrive(double dist, int v, bool isSpecial) {
    double amt = dist / (4*M_PI);
    if(isSpecial) {
      LeftDriveB.resetRotation();
      LeftDriveF.resetRotation();
      RightDriveB.resetRotation();
      RightDriveF.resetRotation();
    }
    LeftDriveB.spinFor(fwd, amt, rev, v, velocityUnits::pct, false); 
    LeftDriveF.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    RightDriveB.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    RightDriveF.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
  }



  void turnDrive(double degrees, int v) {
    // s = theta * r
    // r = Radius of the wheelbase
    double r = 9;
    double theta = degrees * (M_PI/180);
    double s = theta * r;

    double amt = s / (4*M_PI);

    LeftDriveB.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    LeftDriveF.spinFor(fwd, amt, rev, v, velocityUnits::pct, false);
    RightDriveB.spinFor(fwd, -amt, rev, v, velocityUnits::pct, false);
    RightDriveF.spinFor(fwd, -amt, rev, v, velocityUnits::pct);
  }


  void frontHookDrop() {
    FrontHook.spinToPosition(frontHookPos, degrees, hookVelocity, velocityUnits::pct);
  }

  void frontHookDropTorque() {
    FrontHook.spin(fwd, hookVelocity, velocityUnits::pct);
    waitUntil(FrontHook.torque() > 0.4);
    FrontHook.stop();
  }

  void frontHookDropTorque(int vel) {
    FrontHook.spin(fwd, vel, velocityUnits::pct);
    waitUntil(FrontHook.torque() > 0.4);
    FrontHook.stop();
  }

  void frontHookUp() {
    FrontHook.spinToPosition(0, degrees, hookVelocity, velocityUnits::pct);
  }

  /*
  void hookDrop() {
    Hook.spinToPosition(3.0/4*frontHookPos, degrees, hookVelocity, velocityUnits::pct);
  }

  void hookDrop(bool blocking) {
    Hook.spinToPosition(3.0/4*frontHookPos, degrees, hookVelocity, velocityUnits::pct, blocking);
  }

  void hookDown() {
    Hook.spinToPosition(frontHookPos, degrees, hookVelocity, velocityUnits::pct);
  }

  void hookUp() {
    Hook.spinToPosition(0, degrees, hookVelocity, velocityUnits::pct);
  }
  //*/

  /*
  void rightGetMiddle(float amt, float rem) {
    moveDrive(amt, 70, true);
    bool hookActivated = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc;
      if(!hookActivated && amtLeft <= rem) {
        hookDown();
        hookActivated = true;
      }
    }
  }

  

  void rightGoBack(float amt) {
    moveDrive(-dDist/2, 80, true);
    bool hookActivated = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc;
      float pctLeft = amtLeft / amt;
      if(!hookActivated && pctLeft <= 0.5) {
        hookUp();
        hookActivated = true;
      }
    }
  }


  void rightApproachMG(float amt, float rem) {
    moveDrive(amt, 70, true);
    bool hookActivated = false;
    bool didSecondLoop = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc; 
      if(!hookActivated && amtLeft <= rem) {
        hookDrop();
        hookActivated = true;
      } else if(hookActivated && !didSecondLoop) {
        wait(200, msec);
        hookDown();
        didSecondLoop = true;
      }
    }
  }
//*/

  void rightGetMiddle(float amt, float rem) {
    moveDrive(amt, 70, true);
    bool hookActivated = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc;
      if(!hookActivated && amtLeft <= rem) {
        frontHookDropTorque();
        hookActivated = true;
      }
    }
  }

  void rightForwardAndDrop(float amt, float rem) {
    moveDrive(amt, 40, true);
    bool hookActivated = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc;
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("%.2f", amtLeft);
      if(!hookActivated && amtLeft <= rem) {
        frontHookDropTorque();
        hookActivated = true;
      }
    }
  }

  void leftRush(float amt, float rem) {
    moveDrive(amt, 100, true);
    bool hookActivated = false;
    while(!RightDriveF.isDone()) {
      float amtLeft = amt - RightDriveF.rotation(rotationUnits::rev) * wCirc;
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("%.2f", amtLeft);
      if(!hookActivated && amtLeft <= rem) {
        frontHookDropTorque(100);
        hookActivated = true;
        moveDrive(-amt/1.5, 100);
      }
    }

    
    frontHookUp();
    wait(0.1, sec);
    moveDrive(-sDist*0.4, 100);
  }
  
// ----AUTONOMOUS HELPER FUNCTIONS----

// ----AUTONOMOUS FUNCTIONS----
/*
  void rightDefault() {
    rightApproachMG(dDist, 3.5);
    moveDrive(-dDist/2, 70, false);
    wait(200, msec);
    hookUp();
    waitUntil(RightDriveF.isDone());
    hookUp(); 
    turnDrive(-30, 80);
    rightGetMiddle(dDist*3, 4);
    rightGoBack(sDist*2); 
  }
  void leftDefault() {
    hookDrop(false);
    wait(0.5, sec);
    moveDrive(-sDist, 100);
    turnDrive(-30,80);
    rightGetMiddle(2.3*sDist, 4);
    rightGoBack(sDist*1.8);
    
  }
  void rightStretch();
  void leftStretch();
  */

  void leftBasic() {
    rightForwardAndDrop(sDist*0.8, 5);
    /*
    moveDrive(sDist*0.8, 40);
    frontHookDrop();
    //*/
    frontHookUp();
    moveDrive(-sDist*0.4, 40);

    turnDrive(-90, 40);
  }

  void leftDefault() {
    leftBasic();
    turnDrive(-120,40);


  }


  void sDistOne() {
    moveDrive(sDist, 40);
  }

  void sDistOne(float v) {
    moveDrive(sDist, v);
  }

  void rightBasic() {
    moveDrive(sDist*0.85, 40);
    frontHookDrop();
    moveDrive(-sDist*0.85, 40);
    frontHookUp();
  }

  void rightBasicTorque() {
    rightForwardAndDrop(sDist*0.55, 5);
    /*
    moveDrive(sDist*0.55, 40);
    frontHookDropTorque();
    //*/
    moveDrive(-sDist*0.55,40);
    frontHookUp(); 
  }

  void rightDefault() {
    moveDrive(dDist, 80);
    frontHookDropTorque();
    moveDrive(-dDist, 80);
    // turnDrive(-60, 70);
    // rightGetMiddle(2.4*sDist,80);
  }

  void leftBasicTorque() {
    moveDrive(sDist*0.8, 40);
    frontHookDropTorque();
    frontHookUp();
    moveDrive(-sDist*0.8, 40);
  }
  AutonFunction autons[] = {
    /*
    rightDefault, 
    leftDefault,
    rightStretch,
    leftStretch
    */
  };
  
// ----AUTONOMOUS FUNCTIONS----
void autonomous(void) {
  leftRush(2*sDist,16); 
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

void usercontrol(void) {
  ucSetup();
  // User control code here, inside the loop
  // ----CALLBACKS----
  Controller1.ButtonX.pressed(frontHookFunction);
  Controller1.ButtonY.pressed(togglePrint);
  // Controller1.ButtonA.pressed(switchFront);

  while (1) {
    // ----UPDATING CONTROLLER VALUES----
      J1 = Controller1.Axis1.position();
      J2 = Controller1.Axis2.position();
      J3 = Controller1.Axis3.position();
      J4 = Controller1.Axis4.position();

      // Controller Right Button Group Vals
      BX = Controller1.ButtonX.pressing();
      BY = Controller1.ButtonY.pressing();
      BA = Controller1.ButtonA.pressing(); 
      BB = Controller1.ButtonB.pressing();

      // Controller Left Button Group Vals
      BU = Controller1.ButtonUp.pressing();
      BL = Controller1.ButtonLeft.pressing();
      BR = Controller1.ButtonRight.pressing();
      BD = Controller1.ButtonDown.pressing();

      // Controller Shoulder Button Group Vals
      R1 = Controller1.ButtonR1.pressing(); // Right bumper closest to user
      R2 = Controller1.ButtonR2.pressing();
      L1 = Controller1.ButtonL1.pressing(); // Left bumper closest to user
      L2 = Controller1.ButtonL2.pressing();
    // ----UPDATING CONTROLLER VALUES----


    driveFunction();
    armFunction();
    backHook1Function();
    backHook2Function();

    printFunction();
    /*
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("-Y--Driver's Key--Y-");

    Controller1.Screen.setCursor(2,1);
    Controller1.Screen.print("Arm: R1-R2 | Drive: A", Arm.position(degrees));

    Controller1.Screen.setCursor(3,1);
    Controller1.Screen.print("FH/BHU/BHD:X,U-D,L1-2");
    */
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
  bool BX_old = BX;
  bool initialPrinted = false;
  while (true) {
    /*
    if(!Competition.isEnabled()) {
      if(!initialPrinted) {
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Auton ");
        initialPrinted = true;
      }
      if(BX && !BX_old) {

      }
    }
    //*/
    wait(100, msec);
  }
}
