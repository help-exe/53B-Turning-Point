#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// Controller
controller Controller1 = controller(primary); 

// Motors
motor RightDriveB = motor(PORT20, ratio18_1, true); //20
motor RightDriveF = motor(PORT19, ratio18_1, true); 

motor LeftDriveB = motor(PORT11, ratio18_1, false); // 10
motor LeftDriveF = motor(PORT12, ratio18_1, false); // 1

motor Arm = motor(PORT10, ratio18_1, false);
motor Hook = motor(PORT9, ratio18_1, false);
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}