#include "file.h"
#include "mylib.h"
#include <stdio.h>


int isvalid(const char *filename){
    if(len(filename)>=255)
        return 0;
    char *invalidchars =  " \t\n;&|*?$<>(){}[]'\"\\";
    char *invalidnames[]={"CON", "PRN", "AUX", "NUL", "COM1", "COM2", "LPT1", NULL};

    for(int i=0;invalidchars[i]!='\0';i++){
        if(whatindexchar(filename,invalidchars[i],0)>=0)
            return 0;
    }
    for(int i=0;invalidnames[i]!=NULL;i++){
        if (whatindex(filename, invalidnames[i],0) == 0 && len(filename) == len(invalidnames[i])) {
            return 0;
        }
    }
    if(whatindex(filename,".",0)==0)
        return 0;
    return 1;

}

int isvalidtype(char *filename,const char *tag){
    if(!isvalid(filename))
        return 0;
    if(whatindexchar(filename,'.',0)<0)
        return 0;
    char *p = filename;
    int index = whatindexchar(filename,'.',0);
    p += index;
    p++;
    if(checkword(p,tag,1))
        return 1;
    return 0;
}

void touch(const char *filename){
    if(!isvalid(filename)){
        printf("\tInvalid filename entered! %s\n\n",filename);
        return;
    }
    FILE *file;
    file = fopen(filename,"a");
    if(file==NULL){
        printf("\tError Cannot touch %s\n\n",filename);
        return;
    }
    fclose(file);
}

void cat(const char *filename){
    if(!isvalid(filename)){
        printf("\tInvalid filename entered! %s\n\n",filename);
        return;
    }
    FILE *file;
    file = fopen(filename,"r");
    if(file==NULL){
        printf("\tError Cannot open %s\n\n",filename);
        return;
    }
    int cf;
    putchar('\t');
    while((cf = fgetc(file))!=EOF){
        putchar(cf);
    }
    putchar('\n');
}

void grep(const char *filename,const char *word,const int mode){
    if(!isvalid(filename)){
        printf("\tInvalid filename entered! %s\n\n",filename);
        return;
    }
    FILE *file = fopen(filename,"r");
    if(file == NULL){
        printf("\tError Cannot open %s\n\n",filename);
        return;
    }
    int cf;
    int count = 1;
    int maxchars = 100;
    char line[maxchars];
    while(fgets(line,maxchars,file)!=NULL){
        if(whatindex(line,word,mode)>=0){
            printf("\t%d : ", count);
            fputs(line, stdout);
        }
        count++;
    }
    fclose(file);
}

void copyfile(const char *filename){
    if(!isvalid(filename)){
        printf("\tInvalid filename entered! %s\n\n",filename);
        return;
    }
    if(len(filename)>=250){
        printf("\tfilename too long!\n\n");
        return;
    }
    char newname[256] = {"copy-"};
    int i = 5;
    const char *pt = filename;
    while(*pt!='\0'){
        newname[i] = *pt;
        i++;
        pt++;
    }
    newname[i] = '\0';

    FILE *file;
    file = fopen(filename,"r");
    if(file == NULL){
        printf("\tError Cannot open %s\n\n",filename);
        return;
    }

    FILE *filecopy;
    filecopy = fopen(newname,"a");
    if(filecopy == NULL){
        printf("\tFATAL ERROR\n\n");
        return;
    }

    int cf;
    while((cf = fgetc(file))!=EOF){
        fputc(cf,filecopy);
    }
    fclose(file);
    fclose(filecopy);
    
}

void renamefile(const char *filename,const char *newfilename){
    if(!isvalid(filename)){
        printf("\tInvalid filename entered! %s\n\n",filename);
        return;
    }
    if(!isvalid(newfilename)){
        printf("\tInvalid filename entered! %s\n\n",newfilename);
        return;
    }
    FILE *file;
    file = fopen(filename,"r");
    if(file==NULL){
        printf("\tError Cannot open %s\n\n",filename);
        return;
    }
    FILE *newfile;
    newfile = fopen(newfilename,"w");
    if(newfile==NULL){
        printf("\tFATAL ERROR\n\n");
        return;
    }
    char line[1000];
    while((fgets(line,1000,file))!=NULL){
        fputs(line,newfile);
    }
    /*if(rename(filename, newfilename) == 0) {       //       FAST OPTION
        printf("\tSuccessfully renamed '%s' to '%s'\n\n", filename, newfilename);
    } else {
        printf("\tError renaming file: ");
        perror("");  // Print system error message
        putchar('\n');
    }*/
    fclose(file);
    fclose(newfile);

}













