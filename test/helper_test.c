#include "test.h"

char *str_escape(char str[])
{
    if (str == NULL)
        return (NULL);

    int len = strlen(str);
    char *buffer = (char*)malloc(sizeof(char) * len * 2);
    bzero(buffer, len);

    int j = 0;
    for (int i = 0; i < len; i++ ) {
        if (str[i] == '\t') {
            buffer[j++] = '\\';
            buffer[j++] = 't'; 
        }
        else if (str[i] == '\n') {
            buffer[j++] = '\\';
            buffer[j++] = 'n'; 
        }
        else if (str[i] == '\v') {
            buffer[j++] = '\\';
            buffer[j++] = 'v'; 
        }
        else if (str[i] == '\f') {
            buffer[j++] = '\\';
            buffer[j++] = 'f'; 
        }
        else if (str[i] == '\r') {
            buffer[j++] = '\\';
            buffer[j++] = 'r'; 
        }
        else
            buffer[j++] = str[i];
    }
    return buffer;
}

void tester(char *s, int test(), void error()) {
    if (test_function != NULL && strcmp(s, test_function)) return ;
    dprintf(1, "\033[36;1m%22s:\t", s);
    if (test()) error();
    dprintf(1, "\n");
    if (test_function) exit(0);
}