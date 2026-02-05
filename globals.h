//
// Created by duke laptop on 12/19/2025.
//

#ifndef LABYRINTH_GLOBALS_H
#define LABYRINTH_GLOBALS_H

#define MAX 100
#define MAXV 500
// Globals:

extern int n, m;                     // ابعاد واقعی نقشه (خانه‌ها)
extern int core_x, core_y;           // موقعیت هسته نور
extern int num_runners, num_hunters; // تعداد رانر و هانتر
extern char grid[MAX][MAX];          // نقشه بزرگ برای نمایش گرافیکی (با دیوارها و گوشه‌ها)
extern int runnersID[MAX];           // موقعیت رانرها (1 اگر باشه)
extern int huntersID[MAX];           // موقعیت هانترها (1 اگر باشه)
extern int runners[MAX][MAX];
extern int hunters[MAX][MAX];

#endif
