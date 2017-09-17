#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<time.h>
/**
*   ReadWords is a function which reads words from a file and stores unique words with respective *	*	frequencies into arrays.
*    Input =>    filep is a file pointer to a file opened in read mode
*		 words[] is String array, which will hold unique words after function completion
*                max_number_of_words  is the counter of maximum no of words  a file can have, this 
*                                      is supplied at the beginning of the input file itself.
*                freq is an array which will hold the frequency of all unique words after completion of *			funciton
*    Output => integer value which gives total number of unique words.		
**/
int ReadWords(FILE *filep, char *words[], int max_number_of_words, int *freq)
{
     
    int i;
    int j,flagDuplicate;
    int wordCount = 0;
    char temp[100]; // assuming the words cannot be too long
    //FILE *f = fopen(filename, "r");
    
    for (i = 0; i < max_number_of_words; ++i)
    {
        // Read a word from the file
        if (fscanf(filep, "%s", temp) != 1)
            break;
        // note: "!=1" checks for end-of-file;

        // Allocate memory for the word, because temp is too temporary
        flagDuplicate = 0;
        for(j=0; j<wordCount; j++)
        {
            if(strcmp(temp , words[j]) == 0 )
            {
               flagDuplicate = 1;
               freq[j]++;
               break;
            }   
        }
        if(!flagDuplicate)
        {
            words[wordCount] = strdup(temp);
            freq[wordCount++] = 1;
        } 
    }
    fclose(filep);

    // The result of this function is the number of unique words in the file
    return wordCount;
}
/*
     computeIntersection finds the common elements between 2 arrays of Strings
     and returns cardinality of intersection.
     Input     wArray1[] = first array of strings
               wArray2[] = second array of strings
               n1 = size of wArray1
               n2 = size of wArray2
    Output    Cardinality of Intersection.
*/
int computeIntersection(char *wArray1[] , char *wArray2[], int n1, int n2)
{
   int noOfCommonElem = 0 ;
   int i,j;
   for(i = 0 ; i < n1 ; i++)
   {
      for(j = 0 ; j < n2; j++)
      {
         if(strcmp(wArray1[i], wArray2[j]) == 0)
            noOfCommonElem++;
      }
   }
   return noOfCommonElem;
}
int main()
{

   FILE *fPointer1, *fPointer2;
   char fName1[50], fName2[50];
   char **wordsArray1 , **wordsArray2;
   int *frequencyCountArray1, *frequencyCountArray2;
   int noOfWords1,noOfWords2;
   int cardinality1, cardinality2;
   int i;
   int cardinality_of_intersect, cardinality_of_union;
   float jaccard_coeff;
   printf("\n\t Enter name of first file :");
   scanf("%s",fName1);
   printf("\n\t Enter name of second file :");
   scanf("%s",fName2);
  
   clock_t begin = clock();
   
   fPointer1 = fopen(fName1 , "r");
   fPointer2 = fopen(fName2 , "r");
   
   if(fPointer1 == NULL || fPointer2 == NULL)
   {
       printf("\n\t Sorry! Error in opening in either of the file");
       
       return -1;       
   }
   
   //  Read the file; store all words in the list
    fscanf(fPointer1, "%d", &noOfWords1);
    fscanf(fPointer2, "%d", &noOfWords2);
    
    
    wordsArray1 = (char **) malloc(noOfWords1 * sizeof(char *));
    wordsArray2 = (char **) malloc(noOfWords2 * sizeof(char *));
    frequencyCountArray1 = (int *)calloc(noOfWords1, sizeof(int ));
    frequencyCountArray2 = (int *)calloc(noOfWords2, sizeof(int));
    if(frequencyCountArray1 == NULL || wordsArray1 == NULL)
    {
       printf("\n\tError in calloc for frequencyCount1");
       return -1;
    }
    
    cardinality1 = ReadWords(fPointer1, wordsArray1, noOfWords1,frequencyCountArray1);
     
    // print the number of occurrences for each word
    printf("\n\t The unique words in file1 are : %d ", cardinality1);
    printf("\n\t A = { ");
    for (i = 0; i < cardinality1; i++)
    {
        printf("\n\t %s  (%d) ", wordsArray1[i],frequencyCountArray1[i]);
    }
    printf("\n\t } ");
    if(frequencyCountArray2 == NULL || wordsArray2 == NULL)
    {
       printf("\n\t Error in calloc for frequencyCount2");
       return -1;
    }  
    cardinality2 = ReadWords(fPointer2, wordsArray2, noOfWords2,frequencyCountArray2);   
    // print the number of occurrences for each word
    printf("\n\t The unique words in file2 are : %d ", cardinality2);
    printf("\n\t B = { ");
    for (i = 0; i < cardinality2; i++)
    {
        printf("\n\t %s  (%d) ", wordsArray2[i],frequencyCountArray2[i]);
    }
    printf("\n\t } ");
    cardinality_of_intersect = computeIntersection(wordsArray1, wordsArray2, cardinality1,cardinality2);
    cardinality_of_union = cardinality1 + cardinality2 - cardinality_of_intersect;
    jaccard_coeff = 100 * (float)cardinality_of_intersect / cardinality_of_union;
    printf("\n\t Similarity between A and B by Jaccard Co-efficient J(A, B) = %.3f ", jaccard_coeff);
    free(wordsArray1);
    free(wordsArray2);
    free(frequencyCountArray1);
    free(frequencyCountArray2);

    printf("\n");
    fcloseall();
    clock_t end = clock();
    double time_spent = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
    printf("\n\t Time Elapsed is %lf msec\n", time_spent);
    return 0 ;



}
