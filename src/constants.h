typedef enum {
    UP   =1, // 0001
    RIGHT=2, // 0010
    DOWN =4, // 0100
    LEFT =8  // 1000
} Direction;

#define TRUE 1
#define FALSE 0

#define CLOCKWISE -1
#define COUNTER_CLOCKWISE 1

#define PI 3.14159

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define PLAYER_WIDTH  10
#define PLAYER_HEIGHT 10
#define PLAYER_MOVE_SPEED 100
#define PLAYER_ROTATION_SPEED 1.3
#define PLAYER_JUMP_VELOCITY 200;
#define GRAVITY_ACCELERATION 550;

#define RAYS_NUMBER (WINDOW_WIDTH)
//#define FOV (3.5 * PI / 5)
#define FOV (PI / 3)
//#define FOV (2*PI)

#define FIRST_PERSON 1
#define MOUSE_SENSITIVITY 2
#define FLOOR_SIZE (WINDOW_HEIGHT/2)
#define WALL_SIZE 50
