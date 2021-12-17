#include "robot-config.h"
/*---------------------------------------------------------------------------

       WalshBots 9791 unified competition template                              
       Authors:  Liam Donovan, CJ Crocker
       Contributors:  
                James Pearman - Brain Auton Button Selector
                Pascal Chesnais - multimotor abstractions

                Modification History:
                1/5/2019 - prc - instructional version...
  ---------------------------------------------------------------------------*/

//Creates a competition object that allows access to Competition methods.
vex::competition    Competition;

/*                  GLOBAL DEFINITIONS
 * 
 *      These global variables are used across this program to maintain state (choices)
 *
 */
// storage for our auton selection
int   autonomousSelection = -1;

/* 
        James Pearman autoselect functions and definitions. These are modified for Walsh
*/
// collect data for on screen button and include off and on color feedback for button
// prc - instead of radio approach with one button on or off at a time, each button has
//          a state.  ie shootPreload may be low yellow and high yellow when on.
typedef struct _button {
    int    xpos;
    int    ypos;
    int    width;
    int    height;
    bool   state;
    vex::color offColor;
    vex::color onColor;
    const char *label;
} button;

// Button array definitions for each software button. The purpose of each button data structure
// is defined above.  The array size can be extended, so you can have as many buttons as you 
// wish as long as it fits.
button buttons[] = {
    {   30,  30, 60, 60,  false, 0xE00000, 0x0000E0, "Ally" },
    {  150,  30, 60, 60,  false, 0x303030, 0xD0D0D0, "Start" },
    {  270,  30, 60, 60,  false, 0x303030, 0xF700FF, "Park" },
    {  390,  30, 60, 60,  false, 0x303030, 0xDDDD00, "Shoot" },
    {   30, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "4-" },
    {  150, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "5-" },
    {  270, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "6-" },
    {  390, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "7-" }
};

// forward ref
void displayButtonControls( int index, bool pressed );

/*-----------------------------------------------------------------------------*/
/** @brief      Check if touch is inside button                                */
/*-----------------------------------------------------------------------------*/
int
findButton(  int16_t xpos, int16_t ypos ) {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      button *pButton = &buttons[ index ];
      if( xpos < pButton->xpos || xpos > (pButton->xpos + pButton->width) )
        continue;

      if( ypos < pButton->ypos || ypos > (pButton->ypos + pButton->height) )
        continue;

      return(index);
    }
    return (-1);
}

/*-----------------------------------------------------------------------------*/
/** @brief      Init button states                                             */
/*-----------------------------------------------------------------------------*/
void
initButtons() {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      buttons[index].state = false;
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been touched                                        */
/*-----------------------------------------------------------------------------*/
void
userTouchCallbackPressed() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      displayButtonControls( index, true );
    }

}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been (un)touched                                    */
/*-----------------------------------------------------------------------------*/
void
userTouchCallbackReleased() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      // clear all buttons to false, ie. unselected
      //      initButtons(); 

      // now set this one as true
      if( buttons[index].state == true) {
      buttons[index].state = false; }
      else    {
      buttons[index].state = true;}

      // save as auton selection
      autonomousSelection = index;

      displayButtonControls( index, false );
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Draw all buttons                                               */
/*-----------------------------------------------------------------------------*/
void
displayButtonControls( int index, bool pressed ) {
    vex::color c;
    Brain.Screen.setPenColor( vex::color(0xe0e0e0) );

    for(int i=0;i<sizeof(buttons)/sizeof(button);i++) {

      if( buttons[i].state )
        c = buttons[i].onColor;
      else
        c = buttons[i].offColor;

      Brain.Screen.setFillColor( c );

      // button fill
      if( i == index && pressed == true ) {
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, c );
      }
      else
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height );

      // outline
      Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, vex::color::transparent );

// draw label
      if(  buttons[i].label != NULL )
        Brain.Screen.printAt( buttons[i].xpos + 8, buttons[i].ypos + buttons[i].height - 8, buttons[i].label );
    }
}

/* 9791C helper functions */
void shootPuncher()
{
    /* with red gear cartridge, use 3 revolutions to shoot */
    Leftpuncher.setVelocity(100, vex::velocityUnits::pct);
    Rightpuncher.setVelocity(100, vex::velocityUnits::pct);
    Leftpuncher.rotateFor(3.0, rotationUnits::rev,false);
    Rightpuncher.rotateFor(-3.0, rotationUnits::rev);
}

void lowerFlipper(){
    Flipper.rotateTo(-250,rotationUnits::deg,100,velocityUnits::pct, false);

}
void raiseFlipper(){
    Flipper.rotateTo(0,rotationUnits::deg,100,velocityUnits::pct, false);

}
void midFlipper(){
    Flipper.rotateTo(-175,rotationUnits::deg,100,velocityUnits::pct, false);

}

/* WalshBot Multimotor Drive functions
    void driveSetVelocity (int ) - initializes velocity for all drive motors percent
    void driveRotateFor( float) - allows robot to move forward # of rotations (negative number opposite)
    void driveTurnFor( float) - spin robot about center with drives turning opposite directions 
    void driveTurnDegrees( float) - spin robot about center with drives turning opposite directions 
    */
void driveSetVelocity( double speed)
{
    Leftfront.setVelocity(speed, vex::velocityUnits::pct); 
    Rightfront.setVelocity(speed, vex::velocityUnits::pct);
    Leftback.setVelocity(speed, vex::velocityUnits::pct); 
    Rightback.setVelocity(speed, vex::velocityUnits::pct); 
}

void driveRotateFor(float count)
{
    driveSetVelocity(80.0);    // set the drive velocity for all four motors
    Leftfront.rotateFor(count, rotationUnits::rev,false);
    Leftback.rotateFor(count, rotationUnits::rev,false);
    Rightback.rotateFor(count, rotationUnits::rev,false);
    Rightfront.rotateFor(count, rotationUnits::rev);       // block until complete
    task::sleep(50);   
}

void driveDistance(float distance)
{
    // tested distance for 5 revolutions - travelled 66", so each revolution travels 12.2"
    // so to convert inches to revolutions divide the distance by 12.2
    driveRotateFor(distance*0.082);
}

void driveTurnFor(float count)
{
    Leftfront.rotateFor(-1.0*count, rotationUnits::rev,false);
    Leftback.rotateFor(-1.0*count, rotationUnits::rev,false);
    Rightback.rotateFor(count, rotationUnits::rev,false);
    Rightfront.rotateFor(count, rotationUnits::rev);  // block until complete
    task::sleep(50);   
}

void driveTurnDegrees(float degrees)
{
    float rotdegree = 0.82/90;

    driveSetVelocity(40.0);    // turn more slowly to avoid overshoot

    driveTurnFor( degrees * rotdegree );
 
}

void driveTurnLeftDegrees(float degrees)    // counter clockwise - so negative degrees
{
    driveTurnDegrees( 1.0 * degrees);       
}

void driveTurnRightDegrees(float degrees)   // clockwise
{
    driveTurnDegrees(-1.0 * degrees);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton( void ) {
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

void autonomous( void ) {

    /* initialize capabilities from buttons */
    bool allianceBlue = buttons[0].state;
    bool startTileFar = buttons[1].state;
    bool doPark = buttons[2].state;
    bool shootPreload = buttons[3].state;

 
    /* lower flag bumping code - only if near flag start tile */
    if(!startTileFar){            // Starting tile nearest to the flags/net
        // STEP 1a
        lowerFlipper();
        // if preload needs to be shot in autonomous, do it now
        if(shootPreload){
            shootPuncher();
            if(allianceBlue){
                driveTurnRightDegrees(7); // need to have robot facing more to the left
            }
        }
        raiseFlipper();

        // STEP 2
        // drive into first low flag
        driveDistance(46.0);

        // STEP 3       
        // back up to tile aligned with cap I want to run into 
        driveDistance(-6.0);

        // STEP 4
        // turn to face parallel to net
        if(!allianceBlue)              /* red tile is false and blue is true */
            driveTurnRightDegrees(90.0);
        else
            driveTurnLeftDegrees(90.0); /* rotate in opposite direction */

        // STEP 5
        // drive to next flag
        driveDistance(44);
        
        // more autonomous steps here
        
       if(doPark){
            driveSetVelocity( 50 );   
            if(!allianceBlue)              /* red tile is false and blue is true */
                driveTurnRightDegrees(90.0);
            else
                driveTurnLeftDegrees(90.0*-1.0); /* rotate in opposite direction */
            driveSetVelocity( 80 );          
            driveDistance(60.0);

       }              
    }

    else // far from flag/net starting tile
    {

        /*
        Shoot preload */
        if(shootPreload){
            /* insert code for shooting preload here */
            Brain.Screen.printAt( 60,  125, "Shoot Preload         " );
            // WAIT
            // rotate to proper orientation
        }       
        driveSetVelocity(80);    // set the drive velocity for all four motors

        // Drive routines for far from net/flags starting tile...

        /* Parking routines for start tile farthest from flag/net*/
        if(doPark){
            Brain.Screen.printAt( 60,  125, "Do park         " );
 
        }              
    }

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                              User Control Task                             */
/*                                                                            */
/*  This task is used to control your robot during the user control phase of  */
/*  a VEX Competition.                                                        */
/*                                                                            */
/*  You must modify the code to add your own robot specific commands here.    */
/*----------------------------------------------------------------------------*/

void usercontrol( void ) {
    // In driver control, it will be advantageous to reverse the drive train direction so that the driver can
    // use the back cap flipper.
    bool driveReverseDirection = false;
    int count=0;
    int armSpeedPCT = 50;

    while(true) {

    // change direction of robots "front" and "back" to simplify driver's task
        if(Controller1.ButtonUp.pressing()) {   
            driveReverseDirection= true;
        }  
        if(Controller1.ButtonDown.pressing()) {              
            driveReverseDirection= false;
        }  
        if(!driveReverseDirection) {
            Leftfront.spin(vex::directionType::fwd, Controller1.Axis3.value(), vex::velocityUnits::pct); //(Axis3+Axis4)/2
            Leftback.spin(vex::directionType::fwd, Controller1.Axis3.value(), vex::velocityUnits::pct); //(Axis3+Axis4)/2
            Rightfront.spin(vex::directionType::fwd, Controller1.Axis2.value(), vex::velocityUnits::pct);//(Axis3-Axis4)/2
            Rightback.spin(vex::directionType::fwd, Controller1.Axis2.value(), vex::velocityUnits::pct);//(Axis3-Axis4)/2
        } else {
            // controls will be switched so now left is right side and vice versa.
            // motors spin opposite direction.
            // control sticks are swapped
            Leftfront.spin(vex::directionType::rev, Controller1.Axis2.value(), vex::velocityUnits::pct); //(Axis3+Axis4)/2
            Leftback.spin(vex::directionType::rev, Controller1.Axis2.value(), vex::velocityUnits::pct); //(Axis3+Axis4)/2
            Rightfront.spin(vex::directionType::rev, Controller1.Axis3.value(), vex::velocityUnits::pct);//(Axis3-Axis4)/2
            Rightback.spin(vex::directionType::rev, Controller1.Axis3.value(), vex::velocityUnits::pct);//(Axis3-Axis4)/2        
        }
        vex::task::sleep(20); 
    }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {

    //Run the pre-autonomous function. 
    pre_auton();

    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    // register events for button selection
    Brain.Screen.pressed( userTouchCallbackPressed );
    Brain.Screen.released( userTouchCallbackReleased );

    // make nice background
    Brain.Screen.setFillColor( vex::color(0x404040) );
    Brain.Screen.setPenColor( vex::color(0x404040) );
    Brain.Screen.drawRectangle( 0, 0, 480, 120 );
    Brain.Screen.setFillColor( vex::color(0x808080) );
    Brain.Screen.setPenColor( vex::color(0x808080) );
    Brain.Screen.drawRectangle( 0, 120, 480, 120 );

    // initial display
    displayButtonControls( 0, false );

    while(1) {
        // Allow other tasks to run
        if( !Competition.isEnabled() )
            Brain.Screen.setFont(fontType::mono40);
        Brain.Screen.setFillColor( vex::color(0xFFFFFF) );

        Brain.Screen.setPenColor( vex::color(0xc11f27));
        Brain.Screen.printAt( 0,  135, "  WalshBots Competition  " );
        this_thread::sleep_for(10);
    }
}







