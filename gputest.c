#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lib/gpulib.h"
#include <time.h>
#include "enderecos.h"
#include <fcntl.h>
#include <sys/mman.h>

#define DEVICE_PATH "/dev/gpu123"
#define MOUSE_DEVICE "/dev/input/event0"
#define MAX 511

int xsoma = 0, ysoma = 0;
int fd_mouse;
pthread_mutex_t lock;
struct input_event ev;

#define LW_BRIDGE_BASE         0xFF200000
#define LW_BRIDGE_SPAN         0x00005000

#define nove 0b0010000


#define oito 0b0000000


#define sete 0b1111000


#define seis 0b0000010


#define cinco 0b0010010


#define quatro 0b0011001

#define tres 0b0110000

#define dois 0b0100100

#define zero 0b1000000

#define um 0b1111001


// void draw_apple() {
//     color_t color = {6, 7, 7};
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10000, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10001, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10002, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10003, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10004, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10005, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10006, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10007, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10008, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10009, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10010, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10011, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10012, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10013, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10014, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10015, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10016, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10017, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10018, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10019, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10020, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10021, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10022, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10023, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10024, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10025, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10026, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10027, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10028, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10029, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10030, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10031, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10032, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10033, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10034, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10035, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10036, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10037, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10038, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10039, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10040, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10041, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10042, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10043, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10044, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10045, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10046, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10047, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10048, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10049, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10050, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10051, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10052, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10053, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10054, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10055, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10056, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10057, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10058, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10059, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10060, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10061, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10062, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10063, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10064, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10065, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10066, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10067, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10068, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10069, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10070, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10071, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10072, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10073, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10074, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10075, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10076, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10077, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10078, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10079, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10080, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10081, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10082, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10083, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10084, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10085, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10086, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10087, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10088, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10089, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10090, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10091, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10092, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10093, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10094, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10095, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10096, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10097, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10098, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10099, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10100, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10101, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10102, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10103, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10104, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10105, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10106, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10107, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10108, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10109, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10110, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10111, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10112, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10113, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10114, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10115, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10116, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10117, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10118, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10119, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10120, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10121, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10122, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10123, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10124, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10125, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10126, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10127, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10128, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10129, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10130, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10131, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10132, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10133, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10134, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10135, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10136, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10137, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10138, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10139, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10140, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10141, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10142, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10143, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10144, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10145, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10146, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10147, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10148, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10149, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10150, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10151, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10152, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10153, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10154, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10155, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10156, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10157, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10158, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10159, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10160, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10161, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10162, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10163, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10164, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10165, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10166, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10167, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10168, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10169, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10170, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10171, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10172, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10173, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10174, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10175, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10176, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10177, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10178, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10179, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10180, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10181, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10182, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10183, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10184, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10185, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10186, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10187, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10188, color); 
// color.R = 7; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10189, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10190, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10191, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10192, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10193, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10194, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10195, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10196, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10197, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10198, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10199, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10200, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10201, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10202, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10203, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10204, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10205, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10206, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10207, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10208, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10209, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10210, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10211, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10212, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10213, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10214, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10215, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10216, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10217, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10218, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10219, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10220, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10221, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10222, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10223, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10224, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10225, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10226, color); 
// color.R = 4; color.G = 2; color.B = 1; 
// setPixelOnSpriteMemory(10227, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10228, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10229, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10230, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10231, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10232, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10233, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10234, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10235, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10236, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10237, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10238, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10239, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10240, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10241, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10242, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10243, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10244, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10245, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10246, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10247, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10248, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10249, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10250, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10251, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10252, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10253, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10254, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10255, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10256, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10257, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10258, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10259, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10260, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10261, color); 
// color.R = 0; color.G = 2; color.B = 0; 
// setPixelOnSpriteMemory(10262, color); 
// color.R = 0; color.G = 2; color.B = 0; 
// setPixelOnSpriteMemory(10263, color); 
// color.R = 0; color.G = 2; color.B = 0; 
// setPixelOnSpriteMemory(10264, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10265, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10266, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10267, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10268, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10269, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10270, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10271, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10272, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10273, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10274, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10275, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10276, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10277, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10278, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10279, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10280, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10281, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10282, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10283, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10284, color); 
// color.R = 1; color.G = 5; color.B = 2; 
// setPixelOnSpriteMemory(10285, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10286, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10287, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10288, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10289, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10290, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10291, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10292, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10293, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10294, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10295, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10296, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10297, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10298, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10299, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10300, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10301, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10302, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10303, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10304, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10305, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10306, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10307, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10308, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10309, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10310, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10311, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10312, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10313, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10314, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10315, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10316, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10317, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10318, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10319, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10320, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10321, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10322, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10323, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10324, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10325, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10326, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10327, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10328, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10329, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10330, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10331, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10332, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10333, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10334, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10335, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10336, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10337, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10338, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10339, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10340, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10341, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10342, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10343, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10344, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10345, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10346, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10347, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10348, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10349, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10350, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10351, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10352, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10353, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10354, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10355, color); 
// color.R = 7; color.G = 0; color.B = 1; 
// setPixelOnSpriteMemory(10356, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10357, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10358, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10359, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10360, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10361, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10362, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10363, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10364, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10365, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10366, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10367, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10368, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10369, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10370, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10371, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10372, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10373, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10374, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10375, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10376, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10377, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10378, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10379, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10380, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10381, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10382, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10383, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10384, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10385, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10386, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10387, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10388, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10389, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10390, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10391, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10392, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10393, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10394, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10395, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10396, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10397, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10398, color); 
// color.R = 6; color.G = 7; color.B = 7; 
// setPixelOnSpriteMemory(10399, color); 
// }



int i, j;
color_t green = {0,7,0};
color_t red = {7, 0, 0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {6, 7, 7};

// void ladrao(){
    //-----------------------------------------------------------------------------------------------------------------------//   
//LADRÃO PADRÃO

// int x, y;
// color_t cor;
// for (y = 0; y < 21; y++) {
//     for (x = 0; x < 20; x++) {
//         int endereco = 10000 + y * 20 + x;
//         int azul = 7, verde = 7, vermelho = 6; // Cor padrão (transparente)

//         //Condições para preto
//         if (
//             ((x == 5 || x == 15) && (y >= 8 && y <= 11)) ||

//             ((x == 6 || x == 14 ) && ((y >= 3 && y <= 5) || (y >= 7 && y <= 11))) ||

//             (x == 7 && ((y >= 2 && y <= 9) || (y >= 12 && y <= 18))) ||

//             (x == 8 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 8) || (y >= 12 && y <= 18))) ||

//             (x == 9 && ((y >= 1 && y <= 3) || (y >= 5 && y <= 7) || (y >= 12 && y <= 18))) ||

//             (x == 10 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 6) || (y >= 11 && y <= 13))) ||

//             (x == 11 && ((y >= 1 && y <= 3) || (y >= 5 && y <= 6) || (y >= 10 && y <= 18))) ||

//             (x == 12 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 6) || (y >= 9 && y <= 18))) ||

//             (x == 13 && ((y >= 2 && y <= 6) || (y >= 8 && y <= 18)))
//         ) {
//             vermelho = 0;
//             verde = 0;
//             azul = 0;
//         }

//         //Condições para marrom
//         else if (
//             (x == 7 && (y >= 10 && y <= 11)) ||

//             (x == 8 && (y >= 9 && y <= 11)) ||

//             (x == 9 && (y >= 8 && y <= 11)) ||

//             (x == 10 && (y >= 7 && y <= 10)) ||

//             (x == 11 && (y >= 7 && y <= 9)) ||

//             (x == 12 && (y >= 7 && y <= 8)) ||

//             (x == 13 && y == 7)
//         ) {
//             vermelho = 4;
//             verde = 2;
//             azul = 2;
//         }

//         //Condições para rosa cor de pele
//         else if (
//             ((x == 8 || x == 12 || x == 10) && (y >= 3 && y <= 4)) ||
//             ((x == 9 || x == 11) && y == 4) ||
//             ((x == 5 || x == 6 || x == 14 || x == 15)  && y == 12)
//         ) {
//             vermelho = 7;
//             verde = 6;
//             azul = 5;
//         }

//         if (y == 0 || y == 20) {
//             azul = 7; 
//             verde = 7; 
//             vermelho = 6;
//         }

//         cor.R = vermelho;
//         cor.G = verde;
//         cor.B = azul;
//         setPixelOnSpriteMemory(endereco, cor);
//     }

// }
// }

void draw_apple(){
    int x, y, endereco;
    color_t cor = {6, 7, 7};
    color_t blue = {0,0,7};


    // for (x = 0; x < 20; x++) {
    //     for (y = 0; y < 21; y++) {
    //         int endereco = 10000 + y*20 +x;
    //         setPixelOnSpriteMemory(endereco, cor);
    //     }
    // }

    for (x = 0; x < 20; x++) {
        for (y = 0; y < 20; y++) {
            endereco = 10000 + y*20 +x;

            // Corpo da maçã
            if ((x >= 6 && x <= 13 && y >= 7 && y <= 13) ||
                ((x == 5 || x == 14) && (y >= 8 && y <= 12)) ||
                ((x == 4 || x == 15) && (y >= 9 && y <= 11))) {
                setPixelOnSpriteMemory(endereco, red);
            }

        }
    }

    // setPixelOnSpriteMemory(10000+6*20+8, green);
    // setPixelOnSpriteMemory(10000+6*20+11, green);

    for (x = 0; x < 20; x++) {
        for (y = 0; y < 20; y++) {
            endereco = 10000 + y*20 +x;
            // Talo da maçã
            if ((x == 9 || x == 10) && y == 6) {
                setPixelOnSpriteMemory(endereco, brown);
            }
        }
    }

    for (x = 0; x < 20; x++) {
        for (y = 0; y < 20; y++) {
            endereco = 10000 + y*20 +x;
            // Folha da maçã
            if ((x == 8 && y == 5) || (x == 11 && y == 5)) {
                setPixelOnSpriteMemory(endereco, green);
            }
            
        }
    }



}

int open_physicall (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physicall (int fd) {
    close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physicall(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }
    return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physicall(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }
    return 0;
}




// Define as cores
// void draw_apple() {
//     int x, y;
//     for (y = 0; y < 20; y++) {
//         for (x = 0; x < 20; x++) {
//             int endereco = 10000 + y * 20 + x;
//             color_t cor;
//             cor.R = 6;
//             cor.G = 7;
//             cor.B = 7;


//             // pixels da maçã
//             if ((x - 10) * (x - 10) + (y - 10) * (y - 10) <= 36) {
//                 cor.R = 7;
//                 cor.G = 0;
//                 cor.B = 0;
//             }

//             // caule
//             if ((x >= 9 && x <= 11) && (y >= 2 && y <= 6)) {
//                 cor.R = 6;
//                 cor.G = 3;
//                 cor.B = 0;
//             }

//             // folha
//             if ((x >= 5 && x <= 7) && (y >= 2 && y <= 4)) {
//                 cor.R = 0;
//                 cor.G = 7;
//                 cor.B = 0;
//             }
//             // cor.R = 0;
//             // cor.G = 0;
//             // cor.B = 0;
//             setPixelOnSpriteMemory(endereco, cor);
//         }
//     }
// }

/**
#define BACKGROUND_COLOR {7, 7, 7}
#define APPLE_COLOR {7, 0, 0}
#define ORANGE_COLOR {7, 7, 0}
#define PEAR_COLOR {0, 7, 0}
#define STEM_COLOR {6, 3, 0}
#define LEAF_COLOR {0, 7, 0}

*/

void draw_orange() {
    int x, y;
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            int endereco = 10400 + y * 20 + x;
            // color_t cor = {6,7,7};

            // pixels da laranja
            if ((x - 10) * (x - 10) + (y - 10) * (y - 10) <= 36) {
                color_t cor1 = {7,0,0};
            
                setPixelOnSpriteMemory(endereco, cor1);
            }

            // caule
            if ((x >= 9 && x <= 11) && (y >= 2 && y <= 6)) {
              
                color_t cor2 = {6,3,0};
                setPixelOnSpriteMemory(endereco, cor2);
            }

            // setPixelOnSpriteMemory(endereco, cor);
        }
    }
}




void draw_x() {
    int x, y;
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            int endereco = 10000 + y * 20 + x;
            color_t cor = {0,7,0};


            // pixels da laranja
            if (y==x) {
                setPixelOnSpriteMemory(endereco, cor);

            }

            // caule
            if (y==20 - 1-x) {
                setPixelOnSpriteMemory(endereco, cor);
            }

            // setPixelOnSpriteMemory(endereco, cor);
        }
    }
}

void draw_pear() {
    int x, y;
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            int endereco = 10800 + y * 20 + x;
            color_t cor = {6, 7, 7};

            // pixels da pêra
            if ((x >= 7 && x <= 14) && (y >= 8 && y <= 16) &&
                !(x >= 9 && x <= 12 && y >= 10 && y <= 14)) {
                cor.R = 0;
                cor.G = 7;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }

            // caule
            if ((x >= 10 && x <= 12) && (y >= 2 && y <= 6)) {
                cor.R = 6;
                cor.G = 3;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }

            // folha
            if ((x >= 7 && x <= 9) && (y >= 1 && y <= 3)) {
                cor.R = 0;
                cor.G = 7;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }


            // setPixelOnSpriteMemory(endereco, cor);
        }
    }
}




int verificarNumero(int numero) {
    switch (numero) {
        case 9:
            return nove;
        case 8:
            return oito;
        case 7:
            return sete;
        case 6:
            return seis;
        case 5:
            return cinco;
        case 4:
            return quatro;
        case 3:
            return tres;
        case 2:
            return dois;
        case 0:
            return zero;
        case 1:
            return um;
        default:
            printf("Número %d não tem correspondência definida.\n", numero);
            break;
    }
}


int tiros;




unsigned int click2 = 0;

int collision_between_sprites(sprite_t spr1, sprite_t spr2) {
    return (spr1.rel_x >= (int) (spr2.rel_x -10) && spr1.rel_x <= spr2.rel_x + 10 && spr1.rel_y >= (int) (spr2.rel_y -10) && spr1.rel_y <= spr2.rel_y + 10 &&(spr1.visible==1 && spr2.visible==1));
}

// Função da thread para leitura do mouse
void* read_mouse(void* arg) {
    
    ssize_t n;
    int x, y;

    while (1) {
        n  = read(fd_mouse, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Error reading");
            continue;
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Error: read %ld bytes, expecting %ld\n", n, sizeof(ev));
            continue;
        }

        pthread_mutex_lock(&lock);

        if (ev.type == EV_REL && ev.code == REL_X) {
            x = ev.value;
            xsoma += x;
        }
        if (ev.type == EV_REL && ev.code == REL_Y) {
            y = ev.value;
            ysoma += y;
        }

        // Limitar as coordenadas acumuladas para evitar overflow
        if (xsoma < 0) xsoma = 0;
        if (xsoma > 619) xsoma = 619;
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
            click2 = ev.value;
            
        
        }

        

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int x, y;
    int fd_gpu;
    sprite_t spr;
    sprite_t beam;

    int vidas=5;

    sprite_t enemy;
    sprite_t enemy1;
    sprite_t enemy2;
    sprite_t fruit;
    sprite_t fruit1;
    sprite_t fruit2;

    color_t color = {0, 3, 0};
    color_t click = {0, 7, 0};
    unsigned int block;
    int i;
    color_t reset = {6, 7, 7};
    pthread_t mouse_thread;

    //draw_x();
    draw_apple();
    //  draw_orange();
    //  draw_pear();
    

    fruit.address = 9;
    fruit.rel_x = 0;
    fruit.rel_y = 20;
    fruit.variation = 25;
    fruit.visible = 0;

    fruit1.address = 10;
    fruit1.rel_x = 0;
    fruit1.rel_y = 20;
    fruit1.variation = 25;
    fruit1.visible = 0;

    fruit2.address = 11;
    fruit2.rel_x = 0;
    fruit2.rel_y = 20;
    fruit2.variation = 25;
    fruit2.visible = 0;


    enemy.address = 8;
    enemy.rel_x = 0;
    enemy.rel_y = 20;
    enemy.variation = BOMB;
    enemy.visible = 0;

    enemy1.address =2;
    enemy1.rel_x = 0;
    enemy1.rel_y = 20;
    enemy1.variation = 25;
    enemy1.visible = 0;




    enemy2.address =3;
    enemy2.rel_x = 0;
    enemy2.rel_y = 20;
    enemy2.variation = DIAMOND;
    enemy2.visible = 0;


    spr.address = 5;
    spr.rel_y = 459;
    spr.variation = ENEMY_1;
    spr.visible = 1;

    beam.address = 4;
    beam.rel_y = 0;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 0;

    setBackground(color);

    // Abrir o dispositivo do mouse
    fd_mouse = open(MOUSE_DEVICE, O_RDONLY );
    if (fd_mouse == -1) {
        perror("Cannot open input device");
        return 1;
    }

    // Abrir o dispositivo da GPU
    fd_gpu = open(DEVICE_PATH, O_WRONLY);
    if (fd_gpu == -1) {
        perror("Cannot open GPU device");
        close(fd_mouse);
        return 1;
    }




    volatile int * DISPLAY_ptr;
    volatile int * DISPLAY_ptrum;
    volatile int * DISPLAY_ptrdois;
    volatile int * DISPLAY_ptrtres;
    volatile int * DISPLAY_ptrquarto;
    volatile int * DISPLAY_ptrquinto;
    volatile int * chave;

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physicall (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physicall (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX0_BASE);
   
   DISPLAY_ptrum = (unsigned int *) (LW_virtual + HEX1_BASE);

   DISPLAY_ptrdois = (unsigned int *) (LW_virtual + HEX2_BASE);
   
   DISPLAY_ptrtres = (unsigned int *) (LW_virtual + HEX3_BASE);

   DISPLAY_ptrquarto = (unsigned int *) (LW_virtual + HEX4_BASE);
   
   DISPLAY_ptrquinto = (unsigned int *) (LW_virtual + HEX5_BASE);



    // Inicializar o mutex
    pthread_mutex_init(&lock, NULL);

    // Criar a thread para leitura do mouse
    pthread_create(&mouse_thread, NULL, read_mouse, NULL);
    int primeironum;
    int segundonum;
    int terceironum;






    *DISPLAY_ptrtres=verificarNumero(5); 
    *DISPLAY_ptrquarto=verificarNumero(0);    
    *DISPLAY_ptrquinto=verificarNumero(0);	



    int numero;
    double fator = 620.0 / (RAND_MAX + 1.0);  // Calcula o fator de escala

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Gera um número aleatório entre 0 e 619

    int numero_aleatorio;
    int numero_aleatorio1;
    int numero_aleatorio2;



    srand(time(NULL));
    sprite_t lista1[6]={enemy,enemy1,enemy2, fruit, fruit1, fruit2};

    // Loop principal
    int whileCounts[6] = {0};
    while (1) {

        int i;
        for(i=0;i<6;i++){
            whileCounts[i]++;
            if(lista1[i].visible==0){
                
                
                lista1[i].rel_x = (int)(rand() * fator);
                lista1[i].visible=1;    
                
            }
            else {
                if(whileCounts[i] >= 15){
                    lista1[i].rel_y++;
                    whileCounts[i] = 0;

                }

                if(lista1[i].rel_y > 479) {

                    lista1[i].rel_y = 0;
                    lista1[i].rel_x = (int)(rand() * fator);
                    vidas--;
                    setBackground(red);
                    usleep(100000);
                    *DISPLAY_ptrtres=verificarNumero(vidas);    	
                    setBackground(color);
                }
            }
            setSpriteOnScreen(lista1[i]);   
        }

        //if(vidas<0) break;


        // Atualiza as coordenadas acumuladas
        pthread_mutex_lock(&lock);

        
        
        x = xsoma;
        y = ysoma;
        if(click2 == 1){
            beam.visible = 1;
            beam.rel_y = 439;
            beam.rel_x = xsoma;
            click2=0;


            
        }
        
        if (beam.rel_y >= 11 && beam.visible == 1) {
            beam.rel_y -= 1;
        }
        
        int t, u;
        for(t = 0; t<6; t++){
            if(collision_between_sprites(beam, lista1[t]) == 1) {
                
            
                tiros++;
                primeironum = tiros % 10;
                segundonum= (tiros % 100) / 10;
                terceironum = tiros / 100;  

                *DISPLAY_ptr = verificarNumero(primeironum);
                *DISPLAY_ptrum=verificarNumero(segundonum);    
                *DISPLAY_ptrdois=verificarNumero(terceironum);	
                
                printf("colidiu \n");
                lista1[t].visible = 0;
                lista1[t].rel_y = 0;
                beam.visible=0;
            }

            if(collision_between_sprites(spr, lista1[t]) == 1) {
                for(u = 0 ; u < 10; u++){
                    spr.rel_y-= 2;
                    setSpriteOnScreen(spr);
                    usleep(5000);
                }
                for(u = 0 ; u < 10; u++){
                    spr.rel_y+= 2;
                    setSpriteOnScreen(spr);
                    usleep(5000);
                }
                lista1[t].rel_y = 0;
                lista1[t].visible = 0;
                
            }

        }
        
        
        pthread_mutex_unlock(&lock);

        setSpriteOnScreen(beam);


        // Criar a estrutura do sprite com as novas coordenadas
        spr.rel_x = x;


        setSpriteOnScreen(spr);
        // int i;
        // for(i=1;i<32;i++){
        //     enemy.address=i;
        //     enemy.visible=0;
        //     setSpriteOnScreen(enemy);
        // }
        
        
    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);

    // Destruir o mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
