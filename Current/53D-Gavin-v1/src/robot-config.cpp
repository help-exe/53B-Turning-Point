#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

controller Controller1 = controller(primary);

motor LeftBack = motor(PORT6, ratio18_1, true);
motor LeftFront = motor(PORT8, ratio18_1, true);
motor RightBack = motor(PORT10, ratio18_1, false);
motor RightFront = motor(PORT7, ratio18_1, false);

motor Clamp = motor(PORT5, ratio18_1, false);
motor FourBar = motor(PORT6, ratio18_1, true);
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}