#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "xeraph.h"

char* read_file(char* name, size_t* out_size) {
    FILE* file = fopen(name, "r");
    if(!file) {
        fprintf(stderr, "Error: no such file %s\n", name);
        return NULL;
    }
    size_t length;
    char* contents;
    *out_size = 0;
    if(fseeko(file, 0, SEEK_END) != 0) {
        return NULL;
    }
    length = ftello(file);
    if(fseeko(file, 0, SEEK_SET) != 0) {
        return NULL;
    }
    contents = malloc(length * sizeof(char));
    fread(contents, sizeof(char), length, file);
    fclose(file);
    *out_size = length;
    return contents;
}

int* rectify(char* str, size_t str_size, size_t* out_width, size_t* out_height) {
    // remove all carriage returns
    for(size_t i = 0; i < str_size; i++) {
        if(str[i] == '\r') {
            memmove(str + i, str + i + 1, (str_size - i - 1) * sizeof(char));
            str_size--;
            i--;
        }
    }
    size_t max_width = 0, cur_width = 0, height = 0;
    // printf("new section = ");
    for(size_t i = 0; i <= str_size; i++) {
        if(i == str_size || str[i] == '\n') {
            if(cur_width > max_width)
                max_width = cur_width;
            height++;
            cur_width = 0;
            // printf("\nnew section = ");
        } else {
            cur_width++;
            // putchar(str[i]);
        }
    }
    
    // printf("MAX_WIDTH = %i\n", max_width);
    
    int* res = malloc(sizeof(int) * (max_width * height + 1));
    res[max_width * height] = '\0';
    *out_width = max_width;
    *out_height = height;
    
    cur_width = 0;
    for(size_t i = 0, rpos = 0; i <= str_size; i++) {
        if(i == str_size || str[i] == '\n') {
            // pad with spaces
            while(cur_width < max_width) {
                cur_width++;
                res[rpos++] = ' ';
            }
            cur_width = 0;
        } else {
            res[rpos++] = str[i];
            cur_width++;
        }
    }
    return res;
}

XERAPH xeraph_new(char* str, size_t size) {
    XERAPH res;
    res.code = rectify(str, size, &res.width, &res.height);
    res.x = 0;
    res.y = 0;
    res.dx = 1;
    res.dy = 0;
    res.mem_ind = 0;
    res.size = res.width * res.height;
    res.running = 1;
    res.head = '~';
    return res;
}

char xeraph_charat(XERAPH* inst, size_t x, size_t y) {
    return inst->code[y * inst->width + x];
}

void xeraph_advance(XERAPH* inst) {
    if(inst->x == 0 && inst->dx < 0) {
        inst->x = inst->width - 1;
    }
    else if(inst->x + inst->dx >= inst->width) {
        inst->x = 0;
    }
    else {
        inst->x += inst->dx;
    }
    
    if(inst->y == 0 && inst->dy < 0) {
        inst->y = inst->height - 1;
    }
    else if(inst->y + inst->dy >= inst->height) {
        inst->y = 0;
    }
    else {
        inst->y += inst->dy;
    }
}

void xeraph_step(XERAPH* inst) {
    char cur = xeraph_charat(inst, inst->x, inst->y);
    // printf("current@(%i,%i) = %c;\n", inst->x, inst->y, cur);
    if(cur == '+') {
        inst->code[inst->mem_ind]++;
    }
    else if(cur == '-') {
        inst->code[inst->mem_ind]--;
    }
    else if(cur == 'l') {
        if(inst->mem_ind == 0)
            inst->mem_ind = inst->size - 1;
        else
            inst->mem_ind--;
    }
    else if(cur == 'r') {
        if(inst->mem_ind == inst->size - 1)
            inst->mem_ind = 0;
        else
            inst->mem_ind++;
    }
    else if(cur == '.') {
        putchar(inst->code[inst->mem_ind]);
    }
    else if(cur == '#') {
        printf("%i",inst->code[inst->mem_ind]);
    }
    else if(cur == ',') {
        inst->code[inst->mem_ind] = getchar();
    }
    else if(cur == ';') {
        inst->head = inst->code[inst->mem_ind];
    }
    else if(cur == ':') {
        xeraph_advance(inst);
        inst->head = xeraph_charat(inst, inst->x, inst->y);
    }
    else if(cur == 'D') {
        printf("memhead = %i; width = %i;\n", inst->mem_ind, inst->width);
        for(size_t i = 0; i < inst->size; i++) {
            putchar(inst->code[i]);
            if((i + 1) % inst->width == 0)
                putchar('\n');
        }
        fflush(stdout);
    }
    else if(cur == '|') {
        if(inst->code[inst->mem_ind] == inst->head) {
            
            inst->dx = 0;
            inst->dy = 1;
        }
        else {
            inst->dx = 0;
            inst->dy = -1;
        }
    }
    else if(cur == '0') {
        inst->head = '\0';
    }
    else if(cur == '_') {
        if(inst->code[inst->mem_ind] == inst->head) {
            inst->dx = -1;
            inst->dy = 0;
        }
        else {
            inst->dx = 1;
            inst->dy = 0;
        }
    }
    else if(cur == '!') {
        xeraph_advance(inst);
    }
    else if(cur == '?') {
        if(inst->code[inst->mem_ind] != inst->head) {
            xeraph_advance(inst);
        }
    }
    else if(cur == '^') {
        inst->dx = 0;
        inst->dy = -1;
    }
    else if(cur == 'v') {
        inst->dx = 0;
        inst->dy = 1;
    }
    else if(cur == '<') {
        inst->dx = -1;
        inst->dy = 0;
    }
    else if(cur == '>') {
        inst->dx = 1;
        inst->dy = 0;
    }
    else if(cur == '$') {
        inst->running = 0;
    }
    xeraph_advance(inst);
}

void xeraph_run(XERAPH* inst) {
    while(inst->running) {
        xeraph_step(inst);
        
        // getchar();fflush(stdin);
    }
}

int main(int argc, char** argv) {
    size_t len;
    char* content = read_file(argv[1], &len);
    if(!content)
        return 1;
    XERAPH k = xeraph_new(content, len);
    xeraph_run(&k);
    // printf("%i x %i\n", k.width, k.height);
    // write(1, k.code, k.size);
    // char* str = "123445\n..\nsdf";
    // size_t len = strlen(str), w,h;
    // char* rect = rectify(str,len,&w,&h);
    // for(int i = 0; i < w*h; i+=w) {
        // write(1,rect+i,w);putchar(10);
    // }
}