#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    char **dataFiles = {"data1.txt", "data2.txt", "data3.txt", "data4.txt", "data5.txt", "data6.txt", "data7.txt", "data8.txt", "data9.txt", "data10.txt"};
    char *line;
    line = (char *)malloc(sizeof(char) * 1000);

    int fileIndex = 0;
    int lineCount = 0;

    FILE *fp;

    for (fileIndex; fileIndex < 10; fileIndex++)
    {
        fp = fopen(*(dataFiles + fileIndex), "r");
        lineCount = 0;

        if (fp == NULL)
        {
            printf("Failed to open the file %s", *(dataFiles + fileIndex));
            break;
        }

        while (fgets(line, 1000, fp) != NULL)
        {
            line = realloc(line, strlen(line + 1));
            lineCount++;
        }

        printf("%s count = %d\n",*(dataFiles + fileIndex), lineCount);
        fclose(fp);
    }
}
