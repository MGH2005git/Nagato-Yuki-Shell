#include <stdio.h>
#include "mylib.h"
#include <time.h>// showtime
#include <stdlib.h>// system

/*=======================================================================================================*/

struct Commanddef help[] = {
    {"end/exit/esc", "Exit shell"},
    {"cls/clear", "Clear screen"},
    {"echo [text]", "Print text"},
    {"pwd", "Show current directory"},
    {"dir/ls", "List directory contents"},
    {"cd [path]", "Change directory"},
    {"cat [filename]", "Show file contents"},
    {"mkdir [directoryname]", "Create directory"},
    {"touch [filename]", "Create empty file"},
    {"grep/searchfile [word] > [filename]", "Search file contents"},
    {"time", "Show current time"},
    {"whoami", "Display current user"},
    {"game [gamename]", "GAME"},
    {"history", "Show command history"},
    {"help", "Show this help"},
    {"list", "Display list of commands"},
    {"memoryinfo","Displays memory usage"},
    {"man [command]","Displays command info"},
    {NULL, NULL}
};

struct Commandnum ALLcommands[] = {
    {"end",1}, {"exit",1}, {"esc",1}, {"cls",2},
    {"clear",2}, {"echo",3}, {"pwd",4}, {"dir",5},
    {"ls",5}, {"cd",6}, {"cat",7}, {"mkdir",8},
    {"touch",9}, {"grep",10}, {"searchfile",10}, {"time",11},
    {"whoami",12}, {"game",13}, {"history",14}, {"help",15},
    {"list",16},{"memoryinfo",17},{"man",18},
    {NULL,0}
};

/*=======================================================================================================*/

void welcome(void){
    printf("\tWelcome to my shell(SAFE MODE)\n");
    printf("\tThis shell is not case sensitive\n");
    printf("\tDue to our high SAFETY MODE we do NOT support \"rm\" or any risky commands\n");
    printf("\tThis Shell doesn't have access to internet!\n");
    printf("\tCopyright 2025-3-25 Written by @MGH2005\n");
    printf("\tLast update > 2025-3-29\n\n");
}

void gline(char *line){
    int i = 0,c;
    while(1){
        c = getchar();
        if(c=='\n'||c==EOF){
            line[i] = '\0';
            break;
        }
        if(i<MAXLINE-1)
            line[i++] = c;
        else if(i==MAXLINE-1){
            printf("\tMaximum character reached!\n\n");
            line[i] = '\0';
            while((c = getchar())!='\n'&&c!=EOF);
            break;
        }
    }
}

int iswhite(int c){
    return (c==' '||c=='\t'||c=='\n');
}

int gettoken(const char *line,char argv[MAXARG][MAXLINE]) {
    int i = 0,j = 0;
    while (line[i] !='\0' && j < MAXARG) {
        while (iswhite(line[i]))
            i++;
        if (line[i] == '\0')
            break;
        int k = 0;
        while (!iswhite(line[i]) && line[i] != '\0' && k < MAXLINE - 1)
            argv[j][k++] = line[i++];
        argv[j][k] ='\0';
        j++;
        while (!iswhite(line[i]) && line[i] != '\0')
            i++;
    }
    int t = j;
    while(j<MAXARG)
        argv[j++][0] = '\0';
    return t;
}

int lower(const char c){
    if(c>='A'&&c<='Z')
        return c + 'a' - 'A';
    return c;
}

int len(const char *line){
    int i;
    for(i=0;line[i]!='\0';i++);
    return i;
}

int checkword(const char *s1,const char *s2){
    if(len(s1)!=len(s2))
        return 0;
    int i;
    for(i=0;s1[i]!='\0';i++){
        if(lower(s1[i])!=lower(s2[i]))
            return 0;
    }
    return 1;
}

int whatindex(const char *line,const char *word){
    int l1 = len(line);
    int l2 = len(word);
    if(l1<l2)
        return -1;
    int i;
    int j = 0;
    int find = 0;
    for(i=0;i<=l1-l2;i++){
        if(lower(line[i])==word[j]){
            find = 1;
            for(j=0;j<l2;j++){
                if(lower(line[i+j])!=lower(word[j])){
                    find = 0;
                    break;
                }
            }
            if(find)
                return i;
        }
    }
    return -1;
}

int compare(const char *s1,const char *s2){
    int l1 = len(s1);
    int l2 = len(s2);
    if(l1!=l2)
        return 0;
    else{
        int count = 0;
        for(int i=0;s1[i]!='\0';i++)
            if(lower(s1[i])!=lower(s2[i]))
                count++;
        if(count==1)
            return 1;
    }
    return 0;
}

int Syntaxerror(const char *command){
    struct Commandnum *pt = ALLcommands;
    while((pt->command)!=NULL){
        if(compare(command,pt->command)){
            printf("\tSyntax error did you mean '%s'?\n\n",pt->command);
            return 1;
        }
        pt++;
    }
    return 0;
}

void copys(char *copyline,const char *line){
    int i = 0;
    while((copyline[i] = line[i])!='\0'&&i<MAXLINE-1)
        i++;
    copyline[i] = '\0';
}

void clearstr(char *s){
    s[0] = '\0';
}

void addhistory(const char *command,char commandhistory[HISTORY][MAXARG]){
    int i,j;
    for(i=0;i<HISTORY;i++){
        if(checkword(command,commandhistory[i]))
            return;
    }
    for(j = 0;j<HISTORY;j++){
        if(commandhistory[j][0]=='\0'){
            copys(commandhistory[j],command);
            return;
        }
    }
    for(int t=0;t<HISTORY-1;t++)
        copys(commandhistory[t],commandhistory[t+1]);
    copys(commandhistory[HISTORY-1],command);
    return;
}

void catf(const char *filename){
    if(!is_valid_name(filename)){
        printf("\tInvalid filename. Avoid spaces/special characters.\n");
        return;
    }
    FILE *file = fopen(filename,"r");
    if(file == NULL){
        printf("\t Cannot open file\n");
        return;
    }
    int cf;
    while((cf = fgetc(file))!=EOF)
        putchar(cf);
    fclose(file);
    return;
}

void touchfile(const char *filename){
    if(!is_valid_name(filename)){
        printf("\tInvalid filename. Avoid spaces/special characters.\n\n");
        return;
    }
    FILE *file = fopen(filename,"a");
    if(file!=NULL)
        fclose(file);
    else
        printf("\tError cannot touch file %s\n\n",filename);
}

void grep(const char *filename,const char *word){
    if(!is_valid_name(filename)){
        printf("\tInvalid filename. Avoid spaces/special characters.\n\n");
        return;
    }
    FILE *file = fopen(filename,"r");
    if(file == NULL){
        printf("\t Cannot open file\n\n");
        return;
    }
    char line[MAXLINE];
    int count = 1;
    int i = 0;
    int cf;
    while((cf = fgetc(file))!=EOF){
        if(i<MAXLINE-1)
            line[i++] = (char)cf;
        if(cf=='\n'||i==MAXLINE-1){
            line[i] = '\0';
            if(whatindex(line,word)>=0){
                printf("\t%d - %s\n",count,line);
                count++;
            }
            i = 0;
            clearstr(line);
        }
    }
    if (i > 0) {
        line[i] = '\0';
        if (whatindex(line, word) >= 0)
            printf("\t%d - %s\n",count,line);
    }
    fclose(file);
}

void showhelp(void){
    printf("\n\t===========================Available Commands===========================\n");

    struct Commanddef *pt = help; // Pointer to the first struct in 'help'

    while((pt->command) !=NULL){
        printf("\t%-40s -%s\n",pt->command,pt->def);
        pt++;
    }
    putchar('\n');
}

void showlist(void) {
    struct Commandnum *pt = ALLcommands;
    unsigned int num = 0;

    while(pt->command != NULL){
        if(pt->number != num){
            num = pt->number;
            printf("\t%d-%s", num, pt->command);
        } 
        else
            printf("/%s", pt->command);
        if (pt[1].command != NULL && (pt+1)->number != num)
            printf("\n");
        pt++;
    }
    printf("\n\n");
}

int Correctfile(const char *filename, const char *filetag){
    int i = whatindex(filename, ".");
    if (i <= 0)
        return 0;
    if (filename[i+1] == '\0' || filename[i+1] == '.')
        return 0;
        
    const char *extension = filename + i + 1;
    return checkword(extension, filetag);
}

/*=======================================================================================================*/

void currentdirectory(char *curdir) { // VIP
    char cwd[MAXLINE];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        copys(curdir, cwd);
    else
        copys(curdir, "[directory error]");
}

void show_current_dir(void) { // VIP
    char cwd[MAXLINE];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\tCurrent directory: %s\n\n", cwd);
    } else {
        printf("\tError getting current directory\n\n");
    }
}

int changedir(const char *path) { // VIP
    if(chdir(path) != 0) {
        return -1;
    }
    return 0;
}

void display_time(void) { // VIP
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    printf("\tCurrent time: %s\n\n", buffer);
}

int is_valid_name(const char *name) { // VIP
    if (name[0] == '\0') return 0; // Empty name

    const char forbidden[] = " \t\n;&|*?$<>(){}[]'\"\\"; // Forbidden characters
    for (int i = 0; name[i] != '\0'; i++) {
        for (int j = 0; forbidden[j] != '\0'; j++) {
            if (name[i] == forbidden[j]) {
                return 0; // Invalid character found
            }
        }
    }
    return 1; // Valid name
}

int make_dir(const char *path) { // VIP
    if (!is_valid_name(path)) {
        printf("\tInvalid directory name. Avoid spaces/special characters.\n\n");
        return -1;
    }
    #ifdef _WIN32
        return _mkdir(path);
    #else
        return mkdir(path, 0755); // Safer permissions
    #endif
}

void memoryinfo(void){ // VIP
    #ifdef _WIN32
        system("systeminfo | find \"Available Physical Memory\"");
    #else
        system("free -h");
    #endif
    printf("\n\n");
}

/*=======================================================================================================*/

void GuessMyAge(void){
    int c;
    int age = 0;
    int choice = 3;
    printf("\t\n>>> GuessMyAge, only three chances, pls enter valid number\n");
    while(choice!=0){
        age = 0;
        while(choice!=0){
            c = getchar();
            if(c=='\n'||c==EOF)
                break;
            if(c>'9'||c<'0'){
                choice--;
                printf("\t>>pls enter a natural number!\n");
                continue;
            }
            else
                age = 10*age + c - '0';
        }
        if (age>=40){
            printf("\t>>I'm not old bro!\n");
            choice--;
            continue;
        }
        else if (age>=35){
            printf("\t>>try again!\n");
            choice--;
            continue;
        }
        else if (age>=30){
            printf("\t>>try again!\n");
            choice--;
        }
        else if (age>=25){
            printf("\t>>you're close!\n");
            choice--;
            continue;
        }
        else if (age>=23){
            printf("\t>>you're very close! (I'm not graduated)\n");
            choice--;
            continue;
        }
        else if (age>=21){
            printf("\t>>you're very very close! (I'm in college)\n");
            choice--;
            continue;
        }
        else if(age==20){
            printf("\t>>Right Geuss!\n");
            break;
        }
        else if(age<20){
            printf("\t>>I'm not a kid!\n");
            choice--;
            continue;
        }
        age = 0;
    }
    putchar('\n');
    putchar('\n');
}


