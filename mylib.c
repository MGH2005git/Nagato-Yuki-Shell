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
    {"man [command]","Displays complete command info\n\tman [command]"},
    {"version","Displays information about this shell"},
    {"clearhistory","clears command history"},
    {"cp","Copys a file \n\tcp [filename1] [filename2] ...\n\tfilename --> copy-filename"},
    {"csmode","Change shell Case Sensitive mode 'on' or 'off' \n\tcsmode on | csmode off"},
    {"ren","Renames file (SAFE MODE!!! we only create a newfile and put contents of old file to it!)\n\tren [oldfile] [newfile]"},
    {NULL, NULL}
};

struct Commandnum ALLcommands[] = {
    {"end",1}, {"exit",1}, {"esc",1}, {"cls",2},
    {"clear",2}, {"echo",3}, {"pwd",4}, {"dir",5},
    {"ls",5}, {"cd",6}, {"cat",7}, {"mkdir",8},
    {"touch",9}, {"grep",10}, {"searchfile",10}, {"time",11},
    {"whoami",12}, {"game",13}, {"history",14}, {"help",15},
    {"list",16},{"memoryinfo",17},{"man",18},{"version",19},
    {"clearhistory",20},{"cp",21},{"csmode",22},{"ren",23},
    {NULL,0}
};

/*=======================================================================================================*/

void welcome(void){
    printf("\tWelcome to Yuki Nagato Shell (SAFE MODE)\n");
    printf("\tThis Shell is not case sensitive\n");
    printf("\tDue to (SAFE MODE LOCKED ON) we do NOT support \"rm\" or any risky commands\n");
    printf("\tThis Shell doesn't have access to internet!\n");
    printf("\tCopyright 2025-3-25 Written by @MGH2005git\n\n");
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

int checkword(const char *s1,const char *s2,const int mode){
    if(len(s1)!=len(s2))
        return 0;
    int i;
    if(mode){
        for(i=0;s1[i]!='\0';i++){
            if((s1[i])!=(s2[i]))
                return 0;
        }
    }
    else{
        for(i=0;s1[i]!='\0';i++){
            if(lower(s1[i])!=lower(s2[i]))
                return 0;
        }
    }
    
    return 1;
}

int whatindex(const char *line,const char *word,const int mode){
    int l1 = len(line);
    int l2 = len(word);
    if(l1<l2)
        return -1;
    int i;
    int j = 0;
    int find = 0;
    if(mode==0){
        for(i=0;i<=l1-l2;i++){
            if(lower(line[i])==lower(word[j])){
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
    }
    else{
        for(i=0;i<=l1-l2;i++){
            if((line[i])==(word[j])){
                find = 1;
                for(j=0;j<l2;j++){
                    if((line[i+j])!=(word[j])){
                        find = 0;
                        break;
                    }
                }
                if(find)
                    return i;
            }
        }
    }
    
    return -1;
}

int whatindexchar(const char *line,const char c,const int mode){
    int i;
    if(mode==0){
        for(i=0;line[i]!='\0';i++)
        if(lower(line[i])==lower(c))
            return i;
    }
    else{
        for(i=0;line[i]!='\0';i++)
        if((line[i])==(c))
            return i;
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
    struct Commandnum *pt1 = ALLcommands;
    while((pt1->command)!=NULL){
        if(checkword(command,pt1->command,0)){
            return 0;
        }
        pt1++;
    }
    struct Commandnum *pt2 = ALLcommands;
    while((pt2->command)!=NULL){
        if(compare(command,pt2->command)){
            printf("\tSyntax error did you mean '%s'?\n\n",pt2->command);
            return 1;
        }
        pt2++;
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
        if(checkword(command,commandhistory[i],0))
            return;
    }
    for(j = 0;j<HISTORY;j++){
        if(commandhistory[j][0]=='\0'){
            copys(commandhistory[j],command);
            return;
        }
    }
    for(int t=0;t<HISTORY-1;t++){
        clearstr(commandhistory[t]);
        copys(commandhistory[t],commandhistory[t+1]);
    }
    copys(commandhistory[HISTORY-1],command);
    return;
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

void showversion(void){
    welcome();
    printf("\tLast update > 2025-3-30\n\n");
    printf("\t YUKI.N> -- Version_1.1\n\n");
}

int block(const char argv[MAXARG][MAXLINE]){

    const char *blockedcommands[] = {
        "rm", "chmod", "chown", "dd", 
        "wget", "curl", "sudo", "kill",
        "exec", "source", "shred",
        NULL
    };
    for(int i = 0;argv[i][0]!='\0';i++){
        for (int j=0;blockedcommands[j]!=NULL;j++){
            if(checkword(argv[i],blockedcommands[j],0)){
                printf("\tcommand '%s' is blocked!\n\n",argv[i]);
                return 1;
            }
        }
    }
    return 0;
}

void clearhistory(char commandhistory[HISTORY][MAXARG]){
    int i;
    for(i=0;i<HISTORY;i++){
        clearstr(commandhistory[i]);
    }
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

int make_dir(const char *path) { // VIP
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


