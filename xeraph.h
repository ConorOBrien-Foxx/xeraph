#ifndef XERAPH_INCL
#define XERAPH_INCL

typedef struct XERAPH {
    int* code;
    char head;
    size_t x, y,
           width, height,
           mem_ind, size;
    int dx, dy;
    int running;
} XERAPH;


char* read_file(char*, size_t*);

XERAPH xeraph_new(char*, size_t);
char xeraph_charat(XERAPH*, size_t, size_t);
void xeraph_advance(XERAPH*);
void xeraph_step(XERAPH*);
void xeraph_run(XERAPH*);

#endif