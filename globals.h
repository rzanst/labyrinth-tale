#ifndef LABYRINTH_GLOBALS_H
#define LABYRINTH_GLOBALS_H

#define MAX 100
#define MAXV 500
// Globals:

extern int n, m, k;         // ابعاد واقعی نقشه (خانه‌ها)
extern int core_x, core_y;  // موقعیت هسته نور
extern char grid[MAX][MAX]; // نقشه بزرگ برای نمایش گرافیکی (با دیوارها و گوشه‌ها)
extern int runnersID[MAX];  // موقعیت رانرها (1 اگر باشه)
extern int huntersID[MAX];  // موقعیت هانترها (1 اگر باشه)
extern int runners[MAX][2];
extern int hunters[MAX][2];
extern int tmpWall;
extern int tempWallsPlaced[MAX];
extern int seed;
extern int R, H;
extern int box[MAX][MAX];
extern int pnp_just_happened;

#endif
