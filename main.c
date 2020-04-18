#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 500

int splitToken(char const *input, char const delim, char *Commands[]);
int Search_in_File(char *fname, char *str);
void *ConcurrentCommands(void *arg);
void SequentialCommands(char command[100]);
int Insert_In_File(char *fname, char *str, char *target, char command);
char *replaceLine(char *input,  char *wordToExtract,  char *wordToPut);

int main(int argc, char *argv[])
{
    Prompt();
}



void Prompt()
{

    char input[50];
    char command[50];
    char *inputptr;
    int i;
    pthread_t pth;
    char *commandsSeq[1000];
    char *commandsThreaded[1000]; //handle

    inputptr = input;

    printf("myshell:>");
    //scanf("%s",inputptr);
    gets(inputptr);

    int cikis = 0;
    //printf("%c\n", *++inputptr);

    while (cikis == 0)
    {

        //sprintf(command, "echo \"%s\"", input);


        //printf("PR_out:%s\n", command);
        //system(command);
        //system(inputptr);
        int commandsThreadedLen = 0;
        int commandsSeqLen = splitToken(inputptr, ':', commandsSeq);
        if (commandsSeqLen == 0)
        {
            printf("prompt>");
            //scanf("%s",inputptr);
            gets(inputptr);
            continue;
        }


        for (int i = 0; i < commandsSeqLen; i++)
        {


            //printf("devamke  \n");

            commandsThreadedLen = splitToken(commandsSeq[i], ';', commandsThreaded);

            int threadsLen = 0;


            for (int j = 0; j < commandsThreadedLen; j++)
            {
                if (strstr(commandsThreaded[j], "quit") != NULL || strstr(commandsThreaded[j], "exit") != NULL)
                {
                    cikis = 1;
                    continue;
                }
                if(commandsThreadedLen != 1)
                {
                    //printf("CommandsThreaded are : %s \n", commandsThreaded[j]);

                }
            }
        }

        for (int i = 0; i < commandsSeqLen; i++)
        {
            printf("Commands are : %s \n", commandsSeq[i]);
            SequentialCommands(commandsSeq[i]);

        }
        for (int j = 0; j < commandsThreadedLen; j++)
        {
            printf("CommandsThreaded are : %s \n", commandsThreaded[j]);
            pthread_t threads[commandsThreadedLen];
            int threadsLen = 0;

            int rc = pthread_create(&threads[threadsLen++], NULL, ConcurrentCommands, (void *)commandsThreaded[j]);
            if (rc)
            {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
            for (int i = 0; i < threadsLen; i++)
            {
                pthread_join(threads[i], NULL);
            }
        }

        if (cikis == 1)
        {
            continue;
        }
        printf("prompt>");
        //scanf("%s",inputptr);
        gets(inputptr);
    }
}



void SequentialCommands(char command[100])
{

    int i = 0;
    char *token = strtok(command, " ");


    if(strcmp(token, "search") == 0)
    {
        token = strtok(NULL, " ");
        char* param1 = token;
        token = strtok(NULL, " ");
        char* fileName = token;
        printf("word to search is %s  file name is %s \n", param1,fileName);
        Search_in_File(fileName, param1);
    }
    else if(strcmp(token, "lineCount") == 0)
    {
        token = strtok(NULL, " ");
        char* fileName = token;
        printf("Line count is : %d \n",lineCount(fileName));
    }
    else if(strcmp(command,"insert") == 0)
    {

        token = strtok(NULL, " ");
        char* str = token;
        token = strtok(NULL, " ");
        char* target = token;
        token = strtok(NULL," ");
        char* command = token;
        token = strtok(NULL," ");
        char* fileName = token;
        printf("%s %s %s %s,",str,target,fileName,command);
        Insert_In_File(fileName,str,target,command);
    }
   else if(strcmp(command, "replace")==0){
        command = strtok(NULL, " ");
        char* wordToExtract = command;
        command = strtok(NULL, " ");
        char* wordToPut = command;
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE * fp;
        FILE * fTemp;
        char line[BUFFER_SIZE];
        fp  = fopen(fileName , "r");
        fTemp = fopen("newFile.txt", "w");

        if (fp != NULL)
        {
            while ((fgets(line, BUFFER_SIZE, fp)) != NULL){
           fputs(replaceLine(line, wordToExtract, wordToPut),fTemp);
            }
        printf("the word %s successfuly replaced with %s \n",wordToPut,wordToExtract);

        }else{
            fprintf(stderr,"There is an issue with the file please check filename again..\n");
        }


    }
    else if (strcmp(command, "split") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE *fp;
        char ch;
        fp=fopen(fileName,"r");
        int counter=0;
        if(fp==NULL)
        {
            printf("File \"%s\" does not exist!!!\n",fileName);
            return -1;
        }
        //read character by character and check for new line
        while(!feof(fp))
        {
            ch=fgetc(fp);
            printf("%c",ch);
            counter++;
            // printf("counter is %d and Character count is %d",counter,splitNumber);
            if(counter == splitNumber)
            {
                printf("\n");
                counter=0;

            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "head") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE *fp;
        char *ch;
        int i=0;
        fp=fopen(fileName,"r");

        if (fp)
        {
            while (((ch = getc(fp)) != EOF) && i<splitNumber)
            {
                printf("%c",ch);
                if(ch=='\n')
                {
                    i++;
                }
            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "tail") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        int totalLine=lineCount(fileName);
        FILE *fp;
        char *ch;
        int start=totalLine-splitNumber;
        int i=1;
        fp=fopen(fileName,"r");

        if (fp)
        {
            while (((ch = getc(fp)) != EOF))
            {
                if(ch != '\n' && i>start)
                {

                    printf("%c",ch);
                }
                else if(ch == '\n' && i>start)
                {
                    printf("\n");
                }
                else if(ch == '\n')
                {
                    i++;
                }


            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "mid") == 0)
    {
        command = strtok(NULL, " ");
        char* startLine = command;
        int start = atoi(startLine);
        command = strtok(NULL, " ");
        char* finisLine = command;
        int finish = atoi(finisLine);
        command = strtok(NULL, " ");
        char* fileName = command;
        printf("start is %d and finish is %d ",start,finish);
        FILE *fp;
        char *ch;
        int i=1;
        int totalLine=lineCount(fileName);
        fp=fopen(fileName,"r");

        if (fp)
        {
            while ((ch = getc(fp)) != EOF)
            {
                if(ch != '\n' && i>start && i<finish)
                {

                    printf("%c",ch);
                }
                else if(ch == '\n' && i>start && i<finish)
                {
                    printf("\n");
                    i++;
                }
                else if(ch == '\n')
                {
                    i++;
                }
            }
        }
        fclose(fp);
    }

}
int Insert_In_File(char *fname, char *str, char *target, char command)
{

    FILE *fp;
    FILE *fTemp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    fTemp = fopen("inserted.txt", "a");

    if((fp = fopen(fname, "r")) == NULL)
    {
        printf("Unable to open file");
        return(-1);;
    }


    while(fgets(temp, 512, fp) != NULL)
    {
        fputs(temp,fTemp);
        if((strstr(temp, target)) != NULL)
        {
            char *token = strtok(temp, " ");
            while( token != NULL )
            {
                if(strcmp(token, target) == 0){
                    fprintf(fTemp,str);
                    fprintf(fTemp,token);
                }
                token = strtok(NULL, " ");
            }
            //while(strtok())
            //fputs(str,fp);
            //fseek ?
            //fwrite ?
            printf("target inserted");
        }
        line_num++;
    }


    //Close the file if still open.
    if(fp)
    {
        fclose(fp);
    }
    return(0);

}

int splitToken(char const *input, char const delim, char *Commands[])
{

    char *tofree = malloc(sizeof(char) * strlen(input));

    if (input != NULL)
    {
        char *token;

        strcpy(tofree, input);
        token = strtok(tofree, &delim);
        if (token == NULL)
        {
            printf("%s is not a valid command. Try again\n", input);
            return 0;
        }
        Commands[0] = malloc(sizeof(char) * strlen(token));
        strcpy(Commands[0], token);

        int i = 1;
        while (token != NULL)
        {
            token = strtok(NULL, &delim);
            if (token == NULL)
            {
                break;
            }
            Commands[i] = malloc(sizeof(char) * strlen(token));
            strcpy(Commands[i], token);
            i++;
        }
        return i;
    }
}

int Search_in_File(char *fname, char *str)
{

    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];


    if((fp = fopen("input.txt", "r")) == NULL)
    {
        printf("Unable to open file \n");
        return(-1);;
    }


    while(fgets(temp, 512, fp) != NULL)
    {
        if((strstr(temp, str)) != NULL)
        {
            printf("A match found on line: %d\n", line_num);
            printf("%s \n-----\n", temp);
            find_result++;
        }
        line_num++;
    }
    printf("Match has been found %d times. \n", find_result);
    printf("%s \n-----\n");
    if(find_result == 0)
    {
        printf("\nSorry, couldn't find a match.\n");
    }

    //Close the file if still open.
    if(fp)
    {
        fclose(fp);
    }
    return(0);
}

int lineCount(char* filename)
{
    FILE *fp;
    char ch;
    int linesCount=0;
    //open file in read more
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File \"%s\" does not exist!!!\n",filename);
        return -1;
    }
    //read character by character and check for new line
    while((ch=fgetc(fp))!=EOF)
    {
        if(ch=='\n')
            linesCount++;
    }
    //close the file
    fclose(fp);
    //print number of lines
    return linesCount;
}

void *head(void *arg)
{


    //printf("I'm in the head thread");
    char *str;
    str=(char*)arg;
    char *command=strtok(str, " ");
    command = strtok(NULL, " ");
    char* characterCount = command;
    int splitNumber = atoi(characterCount);
    //printf("character count is %d",splitNumber);
    command = strtok(NULL, " ");
    char* fileName = command;
    FILE *fp;
    char *ch;
    int i=0;
    fp=fopen(fileName,"r");

    if (fp)
    {
        while (((ch = getc(fp)) != EOF) && i<splitNumber)
        {
            printf("%c",ch);
            if(ch=='\n')
            {
                i++;
            }
        }
    }
    fclose(fp);


    return NULL;
}
void *mid(void *arg)
{
    printf("I'm in the head thread");
    char *str;
    str=(char*)arg;
    char *command=strtok(str, " ");
    command = strtok(NULL, " ");
    char* startLine = command;
    int start = atoi(startLine);
    command = strtok(NULL, " ");
    char* finisLine = command;
    int finish = atoi(finisLine);
    command = strtok(NULL, " ");
    char* fileName = command;
    printf("start is %d and finish is %d ",start,finish);
    FILE *fp;
    char *ch;
    int i=1;
    int totalLine=lineCount(fileName);
    fp=fopen(fileName,"r");

    if (fp)
    {
        while ((ch = getc(fp)) != EOF)
        {
            if(ch != '\n' && i>start && i<finish)
            {

                printf("%c",ch);
            }
            else if(ch == '\n' && i>start && i<finish)
            {
                printf("\n");
                i++;
            }
            else if(ch == '\n')
            {
                i++;
            }
        }
    }
    fclose(fp);


    return NULL;
}

void *tail(void *arg)
{
    char *str;
    str=(char*)arg;
    char *command=strtok(str, " ");
    command = strtok(NULL, " ");
    char* characterCount = command;
    int splitNumber = atoi(characterCount);
    //printf("character count is %d",splitNumber);
    command = strtok(NULL, " ");
    char* fileName = command;
    int totalLine=lineCount(fileName);
    FILE *fp;
    char *ch;
    int start=totalLine-splitNumber;
    int i=1;
    fp=fopen(fileName,"r");

    if (fp)
    {
        while (((ch = getc(fp)) != EOF))
        {
            if(ch != '\n' && i>start)
            {

                printf("%c",ch);
            }
            else if(ch == '\n' && i>start)
            {
                printf("\n");
            }
            else if(ch == '\n')
            {
                i++;
            }


        }
    }
    fclose(fp);


    return NULL;
}

void *split(void *arg)
{
    printf("I'm in the thread");
    char *str;
    str=(char*)arg;
    char *command=strtok(str, " ");
    command = strtok(NULL, " ");
    char* characterCount = command;
    int splitNumber = atoi(characterCount);
    //printf("character count is %d",splitNumber);
    command = strtok(NULL, " ");
    char* fileName = command;
    FILE *fp;
    char ch;
    fp=fopen(fileName,"r");
    int counter=0;
    if(fp==NULL)
    {
        printf("File \"%s\" does not exist!!!\n",fileName);
        return -1;
    }
    //read character by character and check for new line
    while(!feof(fp))
    {
        ch=fgetc(fp);
        printf("%c",ch);
        counter++;
        // printf("counter is %d and Character count is %d",counter,splitNumber);
        if(counter == splitNumber)
        {
            printf("\n");
            counter=0;

        }
    }
    fclose(fp);


    return NULL;
}

char *replaceLine(char *input,  char *wordToExtract,  char *wordToPut)
{
    char *pos, tempArr[BUFFER_SIZE];
    int index = 0;
    int wordLength;
    wordLength = strlen(wordToExtract);
    while ((pos = strstr(input, wordToExtract)) != NULL)
    {
        strcpy(tempArr, input); // temp line for input
        index = pos - input; // index of the word
        input[index] = '\0'; // str is terminated after word is found
        strcat(input, wordToPut); // new word and input is concataneted
        strcat(input, tempArr + index + wordLength);
    }
    return input; // replaced line is returend...
}

void *ConcurrentCommands(void *arg)
{
    char *str;
    str=(char*)arg;
    char *command = strtok(str, " ");

    if(strcmp(command, "search")==0)
    {
        command = strtok(NULL, " ");
        char* wordToSearch = command;
        command = strtok(NULL, " ");
        char* fileName = command;
        //printf("command is %s word to search is %s and file name is %s \n",command,wordToSearch,fileName);
        FILE *fp;
        int line_num = 1;
        int find_result = 0;
        char temp[512];
        printf("word you are looking for is %s \n",wordToSearch);

        if((fp = fopen(fileName, "r")) == NULL)
        {
            printf("Unable to open file \n");
            return(-1);;
        }


        while(fgets(temp, 512, fp) != NULL)
        {
            if((strstr(temp, wordToSearch)) != NULL)
            {
                printf("A match found on line: %d\n", line_num);
                printf("%s \n-----\n", temp);
                find_result++;
            }
            line_num++;
        }
        printf("Match has been found %d times.", find_result);
        printf("%s \n-----\n");
        if(find_result == 0)
        {
            printf("\nSorry, couldn't find a match.\n");
        }

        //Close the file if still open.
        if(fp)
        {
            fclose(fp);
        }
    }
    else if(strcmp(command, "replace")==0)
    {
        command = strtok(NULL, " ");
        char* wordToExtract = command;
        command = strtok(NULL, " ");
        char* wordToPut = command;
        command = strtok(NULL, " ");
        char* fileName = command;
        printf("word to replace is %s  with the new word name is %s \n", wordToExtract,wordToPut);

        FILE * fPtr;
        FILE * fTemp;
        char line[BUFFER_SIZE];


        fPtr  = fopen(fileName, "r");
        fTemp = fopen("newFile.txt", "w");

        if (fPtr == NULL)
        {

            printf("\nUnable to open file.\n");
            printf("Please check whether file exists and you have read/write privilege.\n");
            exit(EXIT_SUCCESS);
        }

        while ((fgets(line, BUFFER_SIZE, fPtr)) != NULL)
        {
            // Replace all occurrence of word from current line
            replaceLine(line, wordToExtract, wordToPut);

            // After replacing write it to temp file.
            //fputs(buffer, fTemp);
        }
    }
    else if (strcmp(command, "insert") == 0)
    {

        token = strtok(NULL, " ");
        char* str = token;
        token = strtok(NULL, " ");
        char* target = token;
        token = strtok(NULL," ");
        char* command = token;
        token = strtok(NULL," ");
        char* fileName = token;
        printf("%s %s %s %s,",str,target,fileName,command);
        Insert_In_File(fileName,str,target,command);






    }
    else if (strcmp(command, "lineCount") == 0)
    {
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE *fp;
        char ch;
        int linesCount=0;
        //open file in read more
        fp=fopen(fileName,"r");
        if(fp==NULL)
        {
            printf("File \"%s\" does not exist!!!\n",fileName);
            return -1;
        }
        //read character by character and check for new line
        while((ch=fgetc(fp))!=EOF)
        {
            if(ch=='\n')
                linesCount++;
        }
        //close the file
        printf("There are %d number of lines.. \n",linesCount);
        fclose(fp);
    }
    else if (strcmp(command, "split") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE *fp;
        char ch;
        fp=fopen(fileName,"r");
        int counter=0;
        if(fp==NULL)
        {
            printf("File \"%s\" does not exist!!!\n",fileName);
            return -1;
        }
        //read character by character and check for new line
        while(!feof(fp))
        {
            ch=fgetc(fp);
            printf("%c",ch);
            counter++;
            // printf("counter is %d and Character count is %d",counter,splitNumber);
            if(counter == splitNumber)
            {
                printf("\n");
                counter=0;

            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "head") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        FILE *fp;
        char *ch;
        int i=0;
        fp=fopen(fileName,"r");

        if (fp)
        {
            while (((ch = getc(fp)) != EOF) && i<splitNumber)
            {
                printf("%c",ch);
                if(ch=='\n')
                {
                    i++;
                }
            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "tail") == 0)
    {
        command = strtok(NULL, " ");
        char* characterCount = command;
        int splitNumber = atoi(characterCount);
        //printf("character count is %d",splitNumber);
        command = strtok(NULL, " ");
        char* fileName = command;
        int totalLine=lineCount(fileName);
        FILE *fp;
        char *ch;
        int start=totalLine-splitNumber;
        int i=1;
        fp=fopen(fileName,"r");

        if (fp)
        {
            while (((ch = getc(fp)) != EOF))
            {
                if(ch != '\n' && i>start)
                {

                    printf("%c",ch);
                }
                else if(ch == '\n' && i>start)
                {
                    printf("\n");
                }
                else if(ch == '\n')
                {
                    i++;
                }


            }
        }
        fclose(fp);

    }
    else if (strcmp(command, "mid") == 0)
    {
        command = strtok(NULL, " ");
        char* startLine = command;
        int start = atoi(startLine);
        command = strtok(NULL, " ");
        char* finisLine = command;
        int finish = atoi(finisLine);
        command = strtok(NULL, " ");
        char* fileName = command;
        printf("start is %d and finish is %d ",start,finish);
        FILE *fp;
        char *ch;
        int i=1;
        int totalLine=lineCount(fileName);
        fp=fopen(fileName,"r");

        if (fp)
        {
            while ((ch = getc(fp)) != EOF)
            {
                if(ch != '\n' && i>start && i<finish)
                {

                    printf("%c",ch);
                }
                else if(ch == '\n' && i>start && i<finish)
                {
                    printf("\n");
                    i++;
                }
                else if(ch == '\n')
                {
                    i++;
                }
            }
        }
        fclose(fp);
    }

}








