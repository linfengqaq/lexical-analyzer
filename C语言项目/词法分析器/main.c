#define _CRT_SECURE_NO_WARNINGS
// main.c
#include "scanner.h"
#include <stdio.h>
#define N 100
static void run(const char* source) {
    initScanner(source);
    int line = -1;
    // 打印Token, 遇到TOKEN_EOF为止
    for (;;) {
        //  遍历Token流
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}

static void repl() {
    // 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
    // repl是"read evaluate print loop"的缩写
    char s[N];
    while (fgets(s, N, stdin)) {
        run(s);
    }
}

static char* readFile(const char* path) {
    // 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
    // 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Open %s failed.\n", path);
        return NULL;
    }

    // 确定文件的大小
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);

    // 申请一个合适大小的字符数组
    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        printf("malloc failed in readFile\n");
        fclose(fp);
        return NULL;
    }

    rewind(fp); // 回到文件的开头

    int n = fread(buffer, 1, fileSize + 1, fp);
    buffer[n] = '\0';

    // 关闭流
    fclose(fp);
    return buffer;
}

static void runFile(const char* path) {
    // 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
    char* source = readFile(path);
    run(source);
    free(source);
}

int main(int argc, const char* argv[]) {
    if (argc == 1) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
        // ./scanner 没有参数,则进入交互式界面
        repl();
    }
    else if (argc == 2) {
        // ./scanner file 后面跟一个参数,则分析整个文件
        runFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}