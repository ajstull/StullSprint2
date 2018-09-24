#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 256

struct index{
    int maxSizeArray;
    int totalOccurences;
    int *pageNumbers;
    char *word;
};

/*function to split a command based on the delimiter*/
char ** parse_data(char *command,char delim,int *args_c){
    int ct = 0,i;
    char **args = (char **)malloc(sizeof(char *)*(MAXLINE/2+1));
    args[0] = (char *)malloc(MAXLINE);
    *args_c = 0;
    int phrase = 0;
    //check for null or new line character
    for(i=0;command[i]!='\0' && command[i]!='\n';i++){
        //if " then keep on addign to the string
        if(command[i] == '['){
            phrase = 1;
        }
        else if(command[i] == ']'){
            phrase = 0;
        }
        //if not in " " and , then end the string
        else if(phrase==0 && command[i] == delim){
            args[*args_c][ct] = '\0';
            ct = 0;
            *args_c += 1;
            args[*args_c] = (char *)malloc(MAXLINE);
        }
        //add character to the string
        else{
            if(command[i] >= 'A' && command[i] <= 'Z'){
                command[i] -= 'A' - 'a';
            }
            args[*args_c][ct] = command[i];
            ct++;
        }
    }
    //add NULL to the last char
    args[*args_c][ct] = '\0';
    *args_c += 1;
    return args;
}
char *read_line(FILE *fp)
{
    char * line = NULL;
    size_t len = 0;
    long read;
    if (fp == NULL){
        printf("Unable to open the file\n");
        exit(EXIT_FAILURE);
    }

    if ((read = getline(&line, &len, fp)) != -1) {
        if(strlen(line) > MAXLINE){
            fprintf(stderr, "%s\n", "Size of command exceeded the limit : 512 ");
        }
        line[strlen(line)-1] = 0;
        return line;
    }
    return NULL;
}

int * increaseSize(int oldSize,int newSize, int* array){
    int *newArray = (int *)malloc((unsigned long)newSize * sizeof (int));
    for(int i=0;i<oldSize;i++){
        newArray[i] = array[i];
    }
    return newArray;
}
struct index * increaseSizeIndex(int oldSize,int newSize, struct index* array){
    struct index *newArray = (struct index *)malloc((unsigned long)newSize * sizeof (struct index));
    for(int i=0;i<oldSize;i++){
        newArray[i] = array[i];
    }
    return newArray;
}
int* sort(int *pageNumber,int occurences){
    for(int i=0;i<occurences;i++){
        for(int j=0;j<occurences-1-i;j++){
            if(pageNumber[j] > pageNumber[j+1]){
                int tmp = pageNumber[j];
                pageNumber[j] = pageNumber[j+1];
                pageNumber[j+1] = tmp;
            }
        }
    }
    return pageNumber;
}
struct index* sortIndex(struct index *arr,int num_records){
    for(int i=0;i<num_records;i++){
        for(int j=0;j<num_records-1-i;j++){
            if(strcmp(arr[j].word,arr[j+1].word) > 0){
                struct index tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
    return arr;
}
int main()
{
    int num_records = 0;
    int array_size = 10;
    FILE *inputFile = fopen("/home/student/Desktop/CSE2341-18F-Drew-Stull/Sprint2/input.txt","r");
    char *line = NULL;
    int pageNumber = 0;
    struct index *indexes = NULL;
    indexes = increaseSizeIndex(num_records,array_size,indexes);
    while(1){
        line = read_line(inputFile);
        if(line == NULL) break;
        int numWords = 0;
        char **words = parse_data(line,' ',&numWords);
        if(numWords == 1 && words[0][0] == '<'){
            sscanf(line,"<%d>",&pageNumber);
            if(pageNumber == -1) break;
        }
        else{
            for(int i=0;i<numWords;i++){
                int found= 0;
                for(int j=0;j<num_records;j++){
                    if(strcmp(words[i],indexes[j].word) == 0){
                        if(indexes[j].totalOccurences == indexes[j].maxSizeArray){
                            indexes[j].maxSizeArray += 10;
                            indexes[j].pageNumbers = increaseSize(indexes[j].totalOccurences,indexes[j].maxSizeArray,indexes[j].pageNumbers);
                        }
                        int _found = 0;
                        for(int k=0;k<indexes[j].totalOccurences;k++){
                            if(indexes[j].pageNumbers[k] == pageNumber){
                                _found = 1;
                                break;
                            }
                        }
                        if(!_found){
                            indexes[j].pageNumbers[indexes[j].totalOccurences] = pageNumber;
                            indexes[j].totalOccurences++;
                        }
                        found = 1;
                    }
                }
                if(!found){
                    if(num_records == array_size){
                        array_size += 10;
                        indexes = increaseSizeIndex(num_records,array_size,indexes);
                    }
                    indexes[num_records].word = (char *)malloc(strlen(words[i]+2));
                    strcpy(indexes[num_records].word, words[i]);
                    indexes[num_records].maxSizeArray = 10;
                    indexes[num_records].totalOccurences = 0;
                    indexes[num_records].pageNumbers = increaseSize(indexes[num_records].totalOccurences,indexes[num_records].maxSizeArray,indexes[num_records].pageNumbers);
                    indexes[num_records].pageNumbers[indexes[num_records].totalOccurences] = pageNumber;
                    indexes[num_records].totalOccurences++;
                    num_records ++;
                }
            }
        }
    }
    sortIndex(indexes,num_records);
    char currentIndex = indexes[0].word[0];
    char lastIndex = '-';
    for(int i =0;i<num_records;i++){
        currentIndex = indexes[i].word[0];
        if(lastIndex != currentIndex){
            if(currentIndex >= 'a' && currentIndex <= 'z')
                printf("[%c]\n",currentIndex - ('a' - 'A'));
            else
                printf("[%c]\n",currentIndex);
        }
        sort(indexes[i].pageNumbers,indexes[i].totalOccurences);
        printf("%s: ",indexes[i].word);
        for(int j=0;j<indexes[i].totalOccurences-1;j++){
            printf("%d, ",indexes[i].pageNumbers[j]);
        }
        printf("%d\n",indexes[i].pageNumbers[indexes[i].totalOccurences-1]);
        lastIndex = currentIndex;
    }
    return 0;
}
