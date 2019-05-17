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

void strip(char* s){
    int i;
    int len = strlen(s);

    int offset = 0;
    for(i=0; i < len; ++i){
        char c=s[i];
        if(c==' ' || c=='\t' || c=='\n'){
            ++offset;
        }
        else{
            s[i - offset] = c;
        }
    }
    s[len-offset] = '\0';
}

int count_fields(char *line)
{
	int count = 0;
	int done = 0;
    char *c;
	for(c = line; !done; ++c){
		done = (*c == '\0');
		if(*c == ',' || done) ++count;
	}
	return count;
}

double *parse_fields(char *line, int n)
{
	double *field = calloc(n, sizeof(double));
	char *c, *p, *end;
	int count = 0;
	int done = 0;
	for(c = line, p = line; !done; ++c){
		done = (*c == '\0');
		if(*c == ',' || done){
			*c = '\0';
			field[count] = strtod(p, &end);
			if(p == c) field[count] = nan("");
			if(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
			p = c+1;
			++count;
		}
	}
	return field;
}