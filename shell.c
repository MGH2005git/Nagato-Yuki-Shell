#include <stdio.h>
#include "mylib.h"
#include "file.h"
#include <stdlib.h> // system
#include <time.h> // showtime

int main(void){

    welcome();

    char curdir[MAXLINE];
    int echoflag = 1;
    char commandhistory[HISTORY][MAXARG];
    clearhistory(commandhistory);
    int CSmode = 0;

    while(1){

        currentdirectory(curdir);

        if(echoflag)
            printf(" YUKI.N %s > ",curdir);
        else
            printf(" YUKI.N > ");

        char line[MAXLINE];
        char argv[MAXARG][MAXLINE];

        gline(line);
        int argc = gettoken(line,argv);

        if(argc==0)
            continue;
        
        if(Syntaxerror(argv[0]))
            continue;

        if(block(argv))
            continue;
        
        int MagicNumber = 0;
        struct Commandnum *pt = ALLcommands;
        while((pt->command)!=NULL){
            if(checkword(pt->command,argv[0])){
                MagicNumber = pt->number;
                break;
            }
            pt++;
        }

        if(MagicNumber==0){
            printf("\tInvalid command : %s\n",argv[0]);
            printf("\tType 'help' for allowed commands..\n\n");
            continue;
        }
        else if(MagicNumber==1) // end/exit/esc
            break;

        /*==================================================================================================================================*/

        else if(MagicNumber==2){ // cls/clear
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            if(argc>1)
                printf("\tNo command accepted after '%s'\n\n",argv[0]);
            if(checkword(argv[0],"cls"))
                addhistory("cls",commandhistory);
            else
                addhistory("clear",commandhistory);
            continue;
        }
        else if(MagicNumber==3){ // echo
            if(argc>1){
                if(checkword(argv[1],"on")){
                    echoflag = 1;
                    addhistory("echo on",commandhistory);
                }
                else if(checkword(argv[1],"off")){
                    addhistory("echo off",commandhistory);
                    echoflag = 0;
                }
                else{
                    int index;
                    index = whatindex(line,"echo",0);
                    index += len("echo");
                    while(iswhite(line[index])&&line[index]!='\0'){
                        index++;
                    }
                    printf("\t%s\n\n",line+index);
                }
            }
            else{
                printf("\tUsage:\n");
                printf("\t  echo [text]       - Prints the text\n");
                printf("\t  echo on           - Shows working directory in prompt\n");
                printf("\t  echo off          - Hides working directory in prompt\n\n");
            }
            addhistory("echo",commandhistory);
            continue;
        }
        else if(MagicNumber==4){ // pwd
            show_current_dir();
            if(argc>1)
                printf("\tNo command accepted after 'pwd'\n");
            addhistory("pwd",commandhistory);
            continue;
        }
        else if(MagicNumber==5){ // dir/ls
            if(argc==1){
                #ifdef _WIN32
                    system("dir");
                #else
                    system("ls");
                #endif
                putchar('\n');
            }
            else
                printf("\tInvalid option: %s\n\n", argv[1]);
            if(checkword(argv[0],"dir"))
                addhistory("dir",commandhistory);
            else
                addhistory("ls",commandhistory);
            continue;
        }
        else if(MagicNumber==6){ // cd
            if(argc==1) {
                printf("\tcd: missing directory argument\n\n");
            }
            else {
                if(changedir(argv[1]) != 0) {
                    printf("\tcd: %s: No such directory\n\n", argv[1]);
                }
            }
            addhistory("cd",commandhistory);
            continue;
        }
        else if(MagicNumber==7){ // cat
            if(argc==1) {
                printf("\tUsage : show file contents\n");
                printf("\tcat [filename1] [filename2] ...\n\n");
            }
            else {
                for(int i = 1;i<argc;i++){
                    printf("\n\t\t%d : %s\n\n",i,argv[i]);
                    cat(argv[i]);
                    putchar('\n');
                }
            }
            addhistory("cat",commandhistory);
            continue;
        }
        else if(MagicNumber==8){ // mkdir
            if(argc==1){
                printf("\tUsage : make a directory\n");
                printf("\tmkdir [directoryname]\n\n");
            }
            else if(argc==2){
                if(make_dir(argv[1])<0)
                    printf("\tError mkdir\n\n");
            }
            else
                printf("\tNot accepted! : %s\n\n",argv[2]);
            addhistory("mkdir",commandhistory);
            continue;
        }
        else if(MagicNumber==9){ // touch
            if(argc==1){
                printf("\tUsage : create new file\n");
                printf("\ttouch [filename1] [filename2] ...\n\n");
            }
            else{
                for(int i = 1;i<argc;i++){
                    touch(argv[i]);
                }
            }
            addhistory("touch",commandhistory);
            continue;
        }
        else if(MagicNumber==10){ // grep/searchfile
            if(argc==1){
                printf("\tUsage : search for a word in a file 'NOT CASE SENSITIVE'\n");
                printf("\tgrep/searchfile [word] [filename]\n");
                printf("\tsearch for a word in a file 'CASE SENSITIVE'\n");
                printf("\tgrep/searchfile -cs [word] [filename]\n\n");
            }
            else if(argc==2){
                printf("\t%s missing argument\n\n",argv[0]);
            }
            else if(argc==3){
                if(!isvalid(argv[2])) {
                    printf("\tInvalid filename. Avoid spaces/special characters.\n\n");
                } else {
                    grep(argv[2], argv[1], 0);
                }
            }
            else if(argc==4){
                if(checkword(argv[1], "-cs")){
                    if(!isvalid(argv[3])) {
                        printf("\tInvalid filename. Avoid spaces/special characters.\n\n");
                    } else {
                        grep(argv[3], argv[2], 1);
                    }
                } else {
                    printf("\tInvalid option: %s\n\n", argv[1]);
                }
            }
            else {
                printf("\tToo many arguments for %s\n\n", argv[0]);
            }
            addhistory(argv[0],commandhistory);
            continue;
        }
        else if(MagicNumber==11){ // time
            display_time();
            if(argc>1)
                printf("\tNo command accepted after 'time'\n\n");
            addhistory("time",commandhistory);
            continue;
        }
        else if(MagicNumber==12){ // whoami
            printf("\t@MGH2005\n\n");
            addhistory("whoami",commandhistory);
            continue;
        }
        else if(MagicNumber==13){ // game
            if(argc==1)
                printf("\tAvailable games : GuessMyAge\n\n");
            else if(checkword(argv[1],"spacegame")){
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            else if(checkword(argv[1],"guessmyage")){ // bulshit game
                GuessMyAge();
            }
            else{
                printf("\tSorry, We don't have %s!\n",argv[1]);
                printf("\tAvailable games : > GuessMyAge >\n");
                printf("\tgame [gamename]\n\n");
            }
            addhistory("game",commandhistory);
            continue;
        }
        else if(MagicNumber==14){ // history
            printf("\tCommand History:\n");
            for(int i = 0;i<HISTORY&&commandhistory[i][0]!='\0'; i++){
                    printf("\t%2d: %s\n", i+1, commandhistory[i]);
            }
            if(argc>1){
                printf("\tNo command accepted after 'history'\n");
            }
            putchar('\n');
            addhistory("history",commandhistory);
            continue;
        }
        else if(MagicNumber==15){ // help
            showhelp();
            if(argc>1)
                printf("\tNo command accepted after 'help'\n\n");
            addhistory("help",commandhistory);
            continue;
        }
        else if(MagicNumber==16){ // list
            if(argc==1)
                showlist();
            else
                printf("\tNo command accepted after 'list'\n\n");
            addhistory("list",commandhistory);
            continue;
        }
        else if(MagicNumber==17){ // memoryinfo
            memoryinfo();
            if(argc>1)
                printf("\tNot accepted! %s\n\n",argv[1]);
            addhistory("memoryinfo",commandhistory);
            continue;
        }
        /*else if(MagicNumber==18){ // man
            if(argc==1)
                printf("\tDisplays complete command info\n\tman [command]\n\n");
            else if(argc==2){
                showmanual(argv[1]);
            }
            else
                printf("\tNot accepted! %s\n\n",argv[2]);
            addhistory("man",commandhistory);
            continue;
        }*/
        else if(MagicNumber==19){
            if(argc==1)
                showversion();
            else
                printf("No commands accepted after 'version");
            addhistory("version",commandhistory);
            continue;
        }
        else if(MagicNumber==20){
            if(argc==1)
                clearhistory(commandhistory);
            else
                printf("No commands accepted after 'clearhistory");
            continue;
        }
        else if(MagicNumber==21){
            if(argc==1){
                printf("Copys a file \n\tcp [filename1] [filename2] ...\n\tfilename --> copy-filename\n\n");
            }
            else{
                for(int i = 1;i<argc;i++)
                    copyfile(argv[i]);
            }
            addhistory("cp",commandhistory);
            continue;
        }
    }

    return 0;
}