/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * autonomous.h - Autonomous point-to-point driving.
 */
#include <stdint.h>

/** Configuration **/
#define TURN_P 1.0
#define TURN_I 0.0
#define TURN_D 0.0

#define SPEED_P 1.0
#define SPEED_I 0.0
#define SPEED_D 0.0

#define EARTH_R 6378137.0 /* Radius of Earth in meters. */
#define MAX_INITIAL_DISTANCE 1000 /* Max. distance allowed for waypoint segment. */
#define MAX_SPEED 3.0 /* Approximate max speed in m/s. */

#define COMPASS_CAL_SPIN_SPEED 70 /* Speed for compass calibration, in deg/s. */

/** Functions **/

/* Start the timer used for autonomous driving. */
void autonomous_init(void);

/* Process autonomy. */
void autonomous(void);