#ifndef FILE_H
#define FILE_H

int isvalid(const char *filename);
int isvalidtype(char *filename,const char *tag);
void touch(const char *filename);
void cat(const char *filename);
void grep(const char *filename,const char *word,const int mode);
void copyfile(const char *filename);
void renamefile(const char *filename,const char *newfilename);


#endif