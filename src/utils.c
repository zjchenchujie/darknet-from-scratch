#include "utils.h"

char* fgetl(FILE *file){
    if(file == NULL){
        FILE_ERR("File steam input NULL. \n");
        return 0;
    }
    int size = 512; //默认每行最长为512
    char* line = malloc(sizeof(char)*size);
    if(!fgets(line, size, file)){
        free(line);
        return 0;
    }

    int curr = strlen(line);
    //文本行本身最后一个字符为‘\n’换行符， 若size大于行长，则最后line的最后两位符号为‘\n'和’\0‘
    while(line[curr-1] != '\n'){
        size *= 2;
        line = realloc(line, size*sizeof(char));
        if(line == NULL)
        {
            MEM_ERR("Memory realloc error.\n");
            return 0;
        }
        fgets(&line[curr], size-curr, file);
        curr = strlen(line);
    }
    line[curr-1]='\0';
    return line;

}