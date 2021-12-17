/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Author:       Gavin Crisologo                                           */
/*    Created:      10/16/21                                                  */
/*    v1:  Drive train code only.                                             */
/*   v.4:  Toggleable tank drive and graph. Two options for each.             */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
// ----GLOBAL VARIABLES----
  int autonChoice = 1;
  int autonLimit = 2;

  bool pBA = false;
  bool pBX = false;
  bool pBA_old = false;
  bool pBX_old = false;
  bool isLocked = false;
  bool hookUp = false;
  string msg = "No";

  float torqueLim = 0.4;

  bool defClaws = true;
  bool showFront = true;

  int printChoice = 1;
// ----GLOBAL VARIABLES

// ----FUNCTION DECLARATIONS----
void changeAuton();
void toggleLock();
void autoCalibrateUC();

void switchFront();
// ----FUNCTION DECLARATIONS----

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Might not be needed, but just in case...

  /*
  Controller1.ButtonA.pressed(changeAuton);
  Controller1.ButtonX.pressed(toggleLock);
  //*/

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

// ----PREAUTONOMOUS FUNCTIONS----
  void changeAuton() {
        if(autonChoice < autonLimit) autonChoice++;
        else autonChoice = 1;

        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Auton Choice: %d", autonChoice);
  }

  void toggleLock() {
    msg = isLocked? "Yes" : "No";
  }
// ----PREAUTONOMOUS FUNCTIONS----


// ----AUTONOMOUS FUNCTIONS----
  // dist = in inches
  void autonCalibrate() {
    bool calibratedFront = false;
    bool calibratedBack = false;
    bool frontOnly = true;
    bool frontTorque;
    bool backTorque;
   // FrontClaw.spin(reverse, 80, pct);
    // BackClaw.spin(reverse, 80, pct);
    while(!calibratedFront) {
    //  frontTorque = FrontClaw.torque();
      // backTorque = BackClaw.torque();
      if(frontTorque >= torqueLim) {
        /*
        FrontClaw.stop();
        FrontClaw.resetRotation();
        //*/
        calibratedFront = true;
      }
    }
  }

  void moveDrive(double dist, int pwr) {
    double amt = dist / (4*M_PI);
    LeftDriveB.spinFor(fwd, amt, rev, pwr, velocityUnits::pct); 
    LeftDriveF.spinFor(fwd, amt, rev, pwr, velocityUnits::pct);
    RightDriveB.spinFor(fwd, amt, rev, pwr, velocityUnits::pct);
    RightDriveF.spinFor(fwd, amt, rev, pwr, velocityUnits::pct);
  }

  void turnDrive(double degrees, int pwr) {
    // s = theta * r
    // r = Radius of the wheelbase
    double r = 9;
    double theta = degrees * (M_PI/180);
    double s = theta * r;

    double amt = s / (4*M_PI);

    LeftDriveB.spinFor(fwd, amt, rev, pwr, velocityUnits::pct);
    LeftDriveF.spinFor(fwd, amt, rev, pwr, velocityUnits::pct);
    RightDriveB.spinFor(fwd, -amt, rev, pwr, velocityUnits::pct);
    RightDriveF.spinFor(fwd, -amt, rev, pwr, velocityUnits::pct);
  }
// ----AUTONOMOUS FUNCTIONS----

void autonomous(void) {
  // Testing pt 1:
  autonCalibrate();

  moveDrive(50, 30);
  moveDrive(-50, 30);
  moveDrive(30, 80);
  moveDrive(50, 70);
  //

  

  // Testing pt 2:

  /*
  moveDrive(15,50);
  //*/

  // Testing pt 3:

  /*
  turnDrive(90,50);
  //*/

  /*
  turnDrive(90, 30);
  //*/


  /*
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("AutonCalibrate              ");
  wait(0.5, sec);
  autonCalibrate();
  wait(1,sec);


  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("moveDrive(15, 50)            ");
  wait(0.5, sec);
  moveDrive(15, 50);
  wait(1,sec);

  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("turnDrive(90, 50)           ");
  wait(0.5, sec);
  turnDrive(90, 50);
  */
}

// ----GLOBALS----
  const int c = 16;
// ----GLOBALS----

// ----PRINT FUNCTIONS----

  void masterPrint(char x) {
    string output;
    string outHelp;

    switch(x) {
      case 'D': // Line 1 'D'rive
        /*
        Controller1.Screen.setCursor(1,1);
        outHelp = isTank? "Drive: Tank   " : "Drive: Arcade   ";
        //output = "Drive: " + outHelp + "             ";
        Controller1.Screen.print(outHelp.c_str());
        //*/
        break;
      case 'G': // Line 2 'G'raph
        /*
        Controller1.Screen.setCursor(2,1);
        outHelp = isSteep? "Velocity: Regular   " : "Velocity: Half Power    ";
        output = "Velocity: " + outHelp + "             ";
        Controller1.Screen.print(outHelp.c_str());
        //*/
        break;
      default:
        Controller1.rumble("....");
        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("ERROR: SEE PROGRAM");
        break;
  
    }
  }

  /*
  void printTorque() {
    Controller1.rumble("..");
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("Front Torque: %s", FrontClaw.torque());
  }
  //*/
// ----PRINT FUNCTIONS----

// ----DRIVE FUNCTIONS----
  float getDriveOutput(int val) {
    return val*exp(((abs(val)-100)*c)/1000);
  }
// ----DRIVE FUNCTIONS----

// ----HOOK FUNCTIONS----
  void moveHook() {
    hookUp = !hookUp; 
    Hook.spinToPosition(hookUp? 220*3 : 0,deg,80,velocityUnits::pct,false);
    // Hook.spinToPosition(170*hookUp*3,deg,80,velocityUnits::pct,false);
  }
// ----HOOK FUNCTIONS----

// ----CALLBACK FUNCTIONS----
  void autoCalibrateUC() {
    /*
    float frontTorqueVal = FrontClaw.torque(); 
    float backTorqueVal = BackClaw.torque();
    //*/
  }

  void switchFront() {
    defClaws = !defClaws;
    Controller1.rumble("..");
  }

  void manualCalibrateFront() {
   // FrontClaw.resetRotation();
    Controller1.rumble("-.");
  }

  void manualCalibrateBack() {
   // BackClaw.resetRotation();
    Controller1.rumble(".-");
  }


  void changeFront() {
    showFront = !showFront;
  }

  void togglePrint() {
    if(printChoice < 3) {
      printChoice++;
    } else {
      printChoice = 1;
    }
    Controller1.rumble(rumbleShort);
  }
// ----CALLBACK FUNCTIONS----

void usercontrol(void) {
  // ----SET UP SCREEN----
    Controller1.Screen.clearScreen();
  // ----SET UP SCREEN----

  // ----DEFINE OLDS----
    //* 
    // Right Buttons
    bool BX_old = false;
    bool BY_old = false;
    bool BA_old = false;
    bool BB_old = false;

    // Left Buttons
    bool BU_old = false;
    bool BL_old = false;
    bool BR_old = false;
    bool BD_old = false;


    // Shoulder Buttons (1's are closer)
    bool L2_old = false;
    bool R1_old = false;
    bool R2_old = false;
    bool L1_old = false;
    //*/
  // ----DEFINE OLDS----
  
  // ----DEFINE PRESETS----
    LeftDriveB.setStopping(brake); // brake / coast / hold
    LeftDriveF.setStopping(brake);
    RightDriveB.setStopping(brake);
    RightDriveF.setStopping(brake);
  /*
    FrontClaw.setStopping(hold);
    BackClaw.setStopping(hold);
    //*/
    Arm.setStopping(hold);
    Hook.setStopping(hold);

    int rightDriveInput;
    int leftDriveInput;
  // ----DEFINE PRESETS----

  // ----DEFINE CALLBACKS----
    //Controller1.ButtonX.pressed(calibrateUC);
    Controller1.ButtonA.pressed(switchFront);
    Controller1.ButtonX.pressed(moveHook);
    Controller1.ButtonY.pressed(togglePrint);
    /*
    Controller1.ButtonUp.pressed(manualCalibrateFront);
    Controller1.ButtonDown.pressed(manualCalibrateBack);
    //*/
    // Controller1.ButtonLeft.pressed(printTorque);
  // ----DEFINE CALLBACKS

  // ----DEFINE VARIABLES----
    bool hookStopping = false;
    bool armCalibrating = false;
  // ----DEFINE VARIABLES----

  int timeCt = 0;

  while (1) {
    // ----DEFINE VALUES----
      // ----DEFINE INTERFACE----
        // Controller Joystick Vals
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
      // ----DEFINE INTERFACE----
    // ----DEFINE VALUES----

    // ----CONTROLLER ADJUSTMENTS----
      /*
      if(L1 && !L1_old) toggleDriveOption();s
      if(L2 && !L2_old) toggleOutputCurve();
      //*/
    // ----CONTROLLER ADJUSTMENTS----

    // ----DRIVE SYSTEM----
      rightDriveInput = J2;
      leftDriveInput = J3;

      RightDriveB.spin(fwd,getDriveOutput(rightDriveInput),pct);
      RightDriveF.spin(fwd,getDriveOutput(rightDriveInput),pct);

      LeftDriveB.spin(fwd,getDriveOutput(leftDriveInput),pct);
      LeftDriveF.spin(fwd,getDriveOutput(leftDriveInput),pct);
      /*

      RightDriveB.spin(fwd,getDriveOutput(3/5*(defClaws?rightDriveInput:-leftDriveInput)), pct);
      RightDriveM.spin(fwd,getDriveOutput(defClaws?rightDriveInput:-leftDriveInput), pct);
      RightDriveF.spin(fwd,getDriveOutput(3/5*(defClaws?rightDriveInput:-leftDriveInput)), pct);

      LeftDriveB.spin(fwd,getDriveOutput(3/5*(defClaws?leftDriveInput:-rightDriveInput)), pct);
      LeftDriveM.spin(fwd,getDriveOutput(defClaws?leftDriveInput:-rightDriveInput), pct);
      LeftDriveF.spin(fwd,getDriveOutput(3/5*(defClaws?leftDriveInput:-rightDriveInput)), pct);
      //*/ 

        /*
           100
           3 / 5 * 5 
        */

      /*
      RightDriveB.spin(fwd,rightDriveInput*3.0/5.0, pct); // WOrks
      RightDriveM.spin(fwd,rightDriveInput, pct);
      RightDriveF.spin(fwd, rightDriveInput*3.0/5.0,pct);

      LeftDriveB.spin(fwd,leftDriveInput*3.0/5.0, pct);
      LeftDriveM.spin(fwd,leftDriveInput, pct);
      LeftDriveF.spin(fwd,leftDriveInput*3.0/5.0, pct);
      //*/

      
    // ----DRIVE SYSTEM----


    // ----ARM SYSTEM----

      if(armCalibrating && Arm.isDone()) {
        armCalibrating = false;
      }

      if(R1) {
        Arm.spin(fwd,70, pct);
      } else if(R2) {
        Arm.spin(fwd, -70, pct);
      } else if(!armCalibrating) {
        Arm.stop();
      }

      /* 
      if(!armCalibrating) {
          if(R1) {
          Arm.spin(fwd,70, pct);
        } else if(R2) {
          Arm.spin(fwd, -70, pct);
        } else {
          Arm.stop();
        }
      //*/

        /* Arm code toggle:
          if(R1) {
            Arm.spinToPosition()
          }
        //*/
      //}
      //*/

      //* Not needed if using toggle
      if(Arm.position(degrees) < 80) {
        Controller1.rumble(rumbleShort);
        armCalibrating = true;
        Arm.spinToPosition(118, degrees, 70, velocityUnits::pct);
      }
      //*/

      /*
      if(Arm.torque() > 0.4) {
        armCalibrating = true;
        Arm.spinFor(10, degrees, 70, velocityUnits::pct);
      }
      //*/
    // ----ARM SYSTEM----

    // ----HOOK SYSTEM----
    if(L2) {
      Hook.spin(fwd, 80, pct);
    } else if(L1) {
      Hook.spin(fwd, -80, pct);
    } else {
      Hook.stop();
    }
    
    //* To Do: Fix with stopping thing to add.

    if(hookStopping && !Hook.isSpinning()) {
      hookStopping = false;
    }
    if(Hook.torque() > 0.4 && !hookStopping) {
      Hook.stop();
      hookUp = !hookUp;
      hookStopping = true;
    }


    //*/
    // ----HOOK SYSTEM----

    // ----FrontClaw SYSTEM----
      /*
      bool frontUp = L1; //defClaws? L1 : R1;6
      bool frontDown = L2; //defClaws? L2 : R2;
      bool backUp = R1; // defClaws? R1 : L1;
      bool backDown = R2; //defClaws? R2 : R1;
      //*/

      /*
      if(frontUp) {
        FrontClaw.spin(fwd, 80, pct);
      } else if (frontDown) {
        FrontClaw.spin(reverse, 80, pct);
      } else {
        FrontClaw.stop();
      }
    // ----FrontClaw SYSTEM----

    // ----BackClaw SYSTEM----
      if(backUp) {
        BackClaw.spin(fwd, 80,pct);
      } else if (backDown) {
        BackClaw.spin(reverse, 80, pct);
      }  else {
        BackClaw.stop();
      }
    // ----BackClaw SYSTEM----

    //*/

    // ----Print Statements----
      //    
      //*

      // Left Insight
      //*

      switch(printChoice) {
        case 3:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("Torques:         ");

          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm Torque: %.2f        ", Arm.torque());

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("Hook Torque: %.2f       ", Hook.torque());
          break;
        case 2:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("Positions:         ");

          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm Position: %.2f°", Arm.position(degrees));

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("Hook Position: %.2f°", Hook.position(degrees));
          break;
        
        case 1:
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("----Driver's Key----");

          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print("Arm: R1 Up, R2 Down        ", Arm.position(degrees));

          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("Clamp: X↓↑          ", Hook.position(degrees));
          break;

        default:
          Controller1.Screen.clearScreen();
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("ERROR SEE GAVIN     ");
          break;

      }

      /*
      if(printChoice == 1) {
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Torques:        ");

        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("Arm Torque: %.2f        ", Arm.torque());

        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("Hook Torque: %.2f       ", Hook.torque());
      } else if(printChoice == 2) {
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Positions:");

        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("Arm Position: %.2f°", Arm.position(degrees));

        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("Hook Position: %.2f°", Hook.position(degrees));
      } else if(printChoice == 3) {
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("----Driver's Key----");

        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("Arm: R1↑ R2↓        ", Arm.position(degrees));

        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("Clamp: X↓↑          ", Hook.position(degrees));
      }
      //*/
      if(timeCt >= 250) {
        /*
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("armTorque: %.2f", Arm.torque());
        //*/

        /*
        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("rDrive: %.2f",RightDriveB.velocity(pct));
        //*/
        timeCt = 0;
      }
      /*
      if(BA) {
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("armTorque: %.2f", Arm.torque());
      }
      Controller1.Screen.setCursor(2,1);
      Controller1.Screen.print("hookAgl %.2f", Hook.position(degrees));
      //*/
      //*/
    // ----Print Statements----

    // ----UPDATE OLDS----
      /* 
      // Right Buttons
      BX_old = BX;
      BY_old = BY;
      BA_old = BA;
      BB_old = BB;

      // Left Buttons
      BU_old = BU;
      BL_old = BL;
      BR_old = BR;
      BD_old = BD;


      // Shoulder Buttons (1's are closer)
      L2_old = L2;
      R1_old = R1;
      R2_old = R2;
      L1_old = L1;
      // Controller1.Screen.setCursor(3,1);
      //*/
    // ----UPDATE OLDS----

 
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
    timeCt += 20;
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