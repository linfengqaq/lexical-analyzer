#define _CRT_SECURE_NO_WARNINGS
// main.c
#include "scanner.h"
#include <stdio.h>
#define N 100
static void run(const char* source) {
    initScanner(source);
    int line = -1;
    // ��ӡToken, ����TOKEN_EOFΪֹ
    for (;;) {
        //  ����Token��
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
    // ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
    // repl��"read evaluate print loop"����д
    char s[N];
    while (fgets(s, N, stdin)) {
        run(s);
    }
}

static char* readFile(const char* path) {
    // �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
    // ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Open %s failed.\n", path);
        return NULL;
    }

    // ȷ���ļ��Ĵ�С
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);

    // ����һ�����ʴ�С���ַ�����
    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        printf("malloc failed in readFile\n");
        fclose(fp);
        return NULL;
    }

    rewind(fp); // �ص��ļ��Ŀ�ͷ

    int n = fread(buffer, 1, fileSize + 1, fp);
    buffer[n] = '\0';

    // �ر���
    fclose(fp);
    return buffer;
}

static void runFile(const char* path) {
    // ����'.c'�ļ�:�û������ļ��������������ļ�������������
    char* source = readFile(path);
    run(source);
    free(source);
}

int main(int argc, const char* argv[]) {
    if (argc == 1) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
        // ./scanner û�в���,����뽻��ʽ����
        repl();
    }
    else if (argc == 2) {
        // ./scanner file �����һ������,����������ļ�
        runFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}