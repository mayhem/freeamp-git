#ifndef FONT_WIDTH_H
#define FONT_WIDTH_H

#ifdef DEFINE_FONT_WIDTHS
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN int smallFontWidth[] 
#ifndef DEFINE_FONT_WIDTHS
;
#else
= {    
4,
3,
4,
7,
6,
10,
7,
2,
4,
4,
5,
7,
4,
4,
3,
5,
6,
5,
6,
6,
6,
6,
6,
6,
6,
6,
2,
3,
6,
7,
6,
6,
10,
7,
7,
7,
7,
6,
5,
7,
6,
4,
5,
6,
5,
8,
7,
8,
6,
8,
6,
7,
7,
7,
7,
10,
7,
7,
7,
4,
5,
4,
7,
7,
3,
6,
6,
6,
6,
6,
5,
6,
6,
2,
4,
5,
2,
9,
6,
6,
6,
6,
4,
5,
5,
6,
6,
7,
6,
6,
5,
5,
5,
5,
7

};
#endif

EXTERN int largeFontWidth[] 
#ifndef DEFINE_FONT_WIDTHS
;
#else
= {    
0,
0,
4,
0,
0,
12,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
12,
7,
12,
11,
11,
11,
11,
12,
12,
12,
5,

12, 
7,
12, 
11,
11,
11,
11,
12,
12,
12,
5,
12

};

#endif

#endif