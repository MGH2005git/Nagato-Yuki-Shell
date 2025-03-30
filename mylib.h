#ifndef MYLIB_H
#define MYLIB_H

#define MAXLINE 1000
#define MAXARG 32
#define HISTORY 10

// Directory handling
#ifdef _WIN32
    #include <direct.h>
    #define chdir _chdir
    #define getcwd _getcwd
#else
    #include <unistd.h>
#endif

/*=======================================================================================================*/

struct Commanddef {
    const char *command;
    const char *def;
};

struct Commandnum {
    const char *command;
    const unsigned int number;
};

extern struct Commanddef help[];
extern struct Commandnum ALLcommands[];

/*=======================================================================================================*/

void welcome(void);
void gline(char *line);
int iswhite(int c);
int gettoken(const char *line,char argv[MAXARG][MAXLINE]);
int lower(const char c);
int len(const char *line);
int checkword(const char *s1,const char *s2);
int whatindex(const char *line,const char *word);
int compare(const char *s1,const char *s2);
int Syntaxerror(const char *command);
void copys(char *copyline,const char *line);
void clearstr(char *s);
void addhistory(const char *command,char commandhistory[HISTORY][MAXARG]);
void catf(const char *filename);
void touchfile(const char *filename);
void grep(const char *filename,const char *word);
void showhelp(void);
void showlist(void);
int Correctfile(const char *filename,const char *filetag);
void showversion(void);
int block(const char argv[MAXARG][MAXLINE]);
void clearhistory(char commandhistory[HISTORY][MAXARG]);

/*=======================================================================================================*/
//VIP
void currentdirectory(char *curdir);
void show_current_dir(void);
int changedir(const char *path);
void display_time(void);
int is_valid_name(const char *name);
int make_dir(const char *path);
void memoryinfo(void);


/*=======================================================================================================*/
void GuessMyAge(void);







#endif