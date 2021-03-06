#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

void GetLoginName(){       //获取登录名
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    printf("[%s@", pwd->pw_name);
}     

void GetHostName(){        //获取主机名
    char name[100] = {0};
    gethostname(name, sizeof(name)-1);
    printf("%s", name);
}

void GetDir(){            //获取当前工作目录
    char pwd[100] = {0};
    getcwd(pwd, sizeof(pwd)-1);
    int len = strlen(pwd);
    char* p = pwd+len;
    while(*p!='/' && len--){
        p--;
    }
    p++;
    printf(" %s]$ ", p);
}

void ParseArg(char* input, char* output[]){
    //按照空格进行字符串切分
    char* p = strtok(input, " ");
    int output_size = 0;
    while(p != NULL){
        output[output_size++] = p;
        p = strtok(NULL," ");
    }
    output[output_size] = NULL;
}

void Exec(char* argv[]){
    int ret = fork();
    if(ret > 0){
        //father
        wait(NULL);
    }
    else if(ret == 0){
        //child
        execvp(argv[0], argv);
        perror(argv[0]);
        exit(0);
    }
    else{
        perror("fork");
    }
    return;
}

void myshell(){
    while(1){
        GetLoginName();     //获取登录名
        GetHostName();      //获取主机名
        GetDir();           //获取当前工作目录
        fflush(stdout);
        //1.从标准输入读一行字符串
        char buf[1024] = {0};
        gets(buf);
        //2.解析字符串，解析出指令和参数
        char* argv[100] = {NULL};
        ParseArg(buf, argv);
        //3.创建子进程，子进程进行程序替换，父进程进行等待
        Exec(argv);
    }
}


int main(){
    myshell();
    return 0;
}
