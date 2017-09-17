#include<stdio.h>
#include<sys/types.h>       /* for fork() */
#include<unistd.h>         

#include<sys/wait.h>        /* for wait() */

#include<math.h>            /* for ceil() */          
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>


/*
*    ProcessChunk is used to find unique words in the file, from specified offset onwards
*                 and writes those words into a file, ProcessChunk creates a new file.
*    Input :=   filename - name of file to write the unique words  
*               offset  - the byteno from where to begin
*               size - size of chunk
*               wordArray1 conatains all the words of the file, to read from
                wsize is its size.
*   Output := No of unique words in the chunk
*             returns -1 on Error like 1. Error in allocating space for DMA 
*                                      2. Error in creating File.   
*/
int ProcessChunk(char *filename, int offset, int size, char **wordArray1, int wsize)
{
    FILE *outfptr;
   
    char **words;  /* words from file are read into words array and then written into new file*/
    int *freq;     /* frequency of each word corresponding to word of ith location */
    int count = 0; /* count of no of unique words in the chunk */
    int i,j, dupWord = 0;
    char *temp;
    if(offset == -1)
      return 0;

    temp = (char *) malloc(100 * sizeof(char));
    freq = (int *) calloc(size, sizeof(int));
    
    words = (char **) malloc(size * sizeof(char *));
    if(words == NULL || freq == NULL)
    {
        printf("\n\t Error in allocating space for words ");
        return -1;
    }


    outfptr = fopen(filename, "w");
    if(outfptr == NULL)
    {
         printf("\n\tError in opening the files ");
         fflush(stdout); 
         return -1;
    }

    
    for(i= 0; i<size && offset+i < wsize; i++)
    {
     
      strcpy(temp, wordArray1[offset+i]);
     
      dupWord = 0;
      for(j = 0 ; j<count; j++)
      {
         if(strcmp(temp , words[j]) == 0)
         {   
            dupWord = 1;
            freq[j]++;
            break;
         } 
      }
      if(dupWord == 0)
      {
         freq[count] = 1; 
         words[count++] = strdup(temp);
      } 
      /*printf("\n\t Word is %s ", words[count-1]);
      fflush(stdout); */
    }     
    
    for(i = 0; i<count; i++)
    {
       fprintf(outfptr,"%s\n",words[i]);
       fprintf(outfptr , "%d\n",freq[i]);
    }
    fclose(outfptr);

    /*Deallocate the memory of words array */
    for(i=0;i<count; i++)
      free(words[i]); 
    free(words);
    free(freq);
    return count;
} 

/*
*    ProcessFiles is a functions which accepts fname as an array of file names and finds the unique
*           words in combination of all the files.
*     Input :=      fname - an array of file names
*                   noOfFiles - a counter of number of files
*                   uniqueWords - an array of strings which will be populated by function with unique
*                                 words from files.
                    freq[] - frequency of each unique word
*      
*     Output := No of unique words.
*
*/
int ProcessFiles(char **fname, int noOfFiles, char **uniqueWords, int *freq)
{
  int i,j;
  
  FILE *fptr;
  int count = 0;
  char temp[100];
  int duplicate = 0;
  int freqCountInTheFile = 0; 

  for(i=0; i<noOfFiles ; i++)
  {
     if((fptr = fopen(fname[i], "r")) == NULL)
     {
        printf("\n\tError in opening file %s, may be the file is not created because of chunk size in fraction ", fname[i]);
        continue;
     }
     while(!feof(fptr))
     {
        if(fscanf(fptr, "%s", temp) != 1)
              break;
        if(fscanf(fptr, "%d", &freqCountInTheFile) != 1)
              break;
        duplicate = 0;
        for(j = 0 ; j<count; j++)
        {
           if(strcmp(temp , uniqueWords[j]) == 0)
           {   
              duplicate = 1;
              freq[j]+= freqCountInTheFile;
              break;
           } 
        }
        if(duplicate == 0)
        {
           uniqueWords[count] = strdup(temp);
           freq[count++] = freqCountInTheFile;
        }
     }
     fclose(fptr);
  }
  return count;
    
}

/*
* 	ComputeIntersection is a function which finds intersection of 2 string arrays, and returns its
*		cardinality
*	Input :=     wArray1[] = is a String array containing Unique words of file1
*		     wArray2[] = is a String array containing Unique words of file2
*		     n1 = is no of words in wArray1
*		     n2 = is no of words in wArray2
*    	Output := Cardinality of Intersection.	
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
   

   pid_t pid; /* To hold the return of fork() */
   pid_t *processIds;   /* To store all pid */
   int k,i,j;

   char **wordArray1, **wordArray2;
   FILE *filePointer1, *filePointer2;   /* 2 file pointers */  
   char fname1[100], fname2[100];      /* for 2 file names */
   int noOfWords1, noOfWords2;         
   int *offsetNoArray1, *offsetNoArray2;   /* Array of possible offsets, after dividing file into chunks*/
   int sizeOfChunk1, sizeOfChunk2;
   int *freq1, *freq2; 
   
   char temp[100], t[10];
   char **filenames1, **filenames2;    /* Array of possible file names, which are created after processing 
                                                                chunks */
   int index;
   int cardinalityA, cardinalityB;
   char **uniqueWords1 , **uniqueWords2;
   

   printf("\n\tEnter First File name :");
   scanf("%s",fname1);
   filePointer1 = fopen(fname1, "r");
   if(filePointer1 == NULL)
   { 
      printf("\n\t Error in opening the file");
      return -1;
   }

   printf("\n\tEnter Second File name :");
   scanf("%s",fname2);
   filePointer2 = fopen(fname2, "r");
   if(filePointer2 == NULL)
   {
      printf("\n\t Error in opening the file");
      return -1 ;
   } 
   fscanf(filePointer1, "%d", &noOfWords1);
   fscanf(filePointer2, "%d", &noOfWords2);

   printf("\n\tEnter no of chunks to split the file :");
   scanf("%d",&k);

   clock_t begin = clock();

   if(k <= 1)
   {
     printf("\n\t k should be +ve integer, greater than 1");
     return 0;
   }
  
   if(k >= noOfWords1 || k >= noOfWords2)
   {
     printf("\n\t Sorry, size of file(s) is not sufficient to divide into given no of chunks");
     return 0;
   }
   processIds = (pid_t *) malloc(2*k*sizeof(pid_t));

   offsetNoArray1 = (int *)calloc(k , sizeof(int));
   offsetNoArray2 = (int *)calloc(k, sizeof(int));

   filenames1 = (char **) malloc(k * sizeof(char *));
   filenames2 = (char **) malloc(k * sizeof(char *));
   if(offsetNoArray1 == NULL || offsetNoArray2 == NULL || filenames1 == NULL || filenames2 == NULL)
   {
     printf("\n\tMemory Allocation issue");
     return -1;
   }
 
   
   wordArray1 = (char **) malloc( noOfWords1 * sizeof(char *));
   
   char word[100]; 
   for(i=0;i<noOfWords1;i++)
   {
     fscanf(filePointer1, "%s", word);
     wordArray1[i]= (char *) malloc((strlen(word) + 1) * sizeof(char));
     strcpy(wordArray1[i], word);
   }

   
   
   wordArray2 = (char **) malloc(noOfWords2 * sizeof(char *));
    
   for(i=0;i<noOfWords2;i++)
   {
     fscanf(filePointer2, "%s", word);
     wordArray2[i]= (char *) malloc((strlen(word) + 1) * sizeof(char));
     strcpy(wordArray2[i], word);
   }


   sizeOfChunk1 = ceil((double)noOfWords1 / k);
   offsetNoArray1[0] = 0;
  
   sizeOfChunk2 = ceil((double)noOfWords2 / k);
   offsetNoArray2[0] = 0;
   
   //printf("\n\t sizeofchunk1 = %d and sizeof chunk2 = %d", sizeOfChunk1, sizeOfChunk2);
   int offsetIndex1 = 0, offsetIndex2 = 0;
   for(i=1;i<k; i++)
   {
      for(j = 1; j<=sizeOfChunk1 ; j++)
       offsetIndex1++;
      if(offsetIndex1 >= noOfWords1)
         offsetNoArray1[i] = -1;
      else
         offsetNoArray1[i]=offsetIndex1;
      
      for(j = 1; j<=sizeOfChunk2  ; j++)
         offsetIndex2++;
      if(offsetIndex2 >= noOfWords2)
            offsetNoArray2[i] = -1;
      else
         offsetNoArray2[i] = offsetIndex2;   
   }
   for(i = 0 ; i<k; i++)
   {
      strcpy(temp, "File1_");
      sprintf(t,"%d",offsetNoArray1[i]);
      strcat(temp , t);
      strcat(temp, ".txt"); 
      filenames1[i] = (char *) malloc((strlen(temp) + 1) * sizeof(char));
      strcpy(filenames1[i],temp);

      strcpy(temp, "File2_");
      sprintf(t, "%d", offsetNoArray2[i]);
      strcat(temp , t);
      strcat(temp, ".txt");
      filenames2[i] = (char *) malloc((strlen(temp) + 1) * sizeof(char));
      strcpy(filenames2[i],temp);
   }

   rewind(filePointer1);
   rewind(filePointer2);
/*
   for(i=0; i<k; i++)
   {
       printf("\n\t First File Chunk %d Offset is %d First Word is :",i, offsetNoArray1[i]);
       fseek(filePointer1, offsetNoArray1[i], SEEK_SET);
       fscanf(filePointer1, "%s", temp);
       printf("%s",temp);
       
       printf("\n\t Second File Chunk %d Offset is %d First Word is :",i, offsetNoArray2[i]);
       fseek(filePointer2, offsetNoArray2[i], SEEK_SET);
       fscanf(filePointer2, "%s", temp);
       printf("%s",temp);
       printf("\n\t %s  %s ", filenames1[i], filenames2[i]);
   } 
   rewind(filePointer1);
   rewind(filePointer2);
*/
   printf("\n");
   fflush(stdout);

   for(i = 0; i < 2*k; i++)
   {
      
      //fflush(stdout);
      pid = fork();
      processIds[i] = pid;
      if(pid == 0)
      {
        // printf("\n\t For i = %d ProcessChunk is executing " , i);
        //fflush(stdout);
         if(i < k)
         {
             ProcessChunk(filenames1[i] , offsetNoArray1[i], sizeOfChunk1, wordArray1,noOfWords1);
           //printf("\t %s unique is %d \n ",filenames1[i], noOfUniqueInFile);
           //fflush(stdout);  
         } 
         else
         {
           index = i % k; 
           ProcessChunk(filenames2[index], offsetNoArray2[index],sizeOfChunk2, wordArray2,noOfWords2);
           //printf("\t %s unique is %d \n",filenames2[index], noOfUniqueInFile);
           //fflush(stdout);
         }   
          
         return 0;
      }
      else if(pid < 0)
      {
           printf("Error in executing fork() call ");
           return -1 ;
      } 
   }
  //sleep(100);
  int *status = (int *) malloc(sizeof(int));
  for(i = 0 ; i < 2*k; i++)
  {
     
    waitpid(processIds[i],status,WUNTRACED );
        /*
    first paramter is processIds, if it is greater than 0, it means
    wait for the child whose process ID is equal to the value of pid.
              WUNTRACED returns if child has stopped
              WNOHANG   returns immediately even if no child has exited. 
              status contains the status of processId child.
        */
    printf(" status =  %d",*status); 
  }
 
  uniqueWords1 = (char **)malloc(noOfWords1 * sizeof(char *));
  uniqueWords2 = (char **)malloc(noOfWords2 * sizeof(char *));
  freq1 = (int *) calloc(noOfWords1, sizeof(int));
  freq2 = (int *) calloc(noOfWords2, sizeof(int));
  if(!uniqueWords1 || !uniqueWords2 || !freq1 || !freq2)
  {
    printf("\n\t Error in memory allocation functions");
    return -1;
  }

  cardinalityA = ProcessFiles(filenames1, k, uniqueWords1, freq1);
  cardinalityB = ProcessFiles(filenames2, k, uniqueWords2, freq2);
  fflush(stdout);
  printf("\t Cardinality of A : %d \n" , cardinalityA);
  fflush(stdout);
  printf("\t Cardinality of B : %d \n", cardinalityB);
  fflush(stdout);
  printf("\n\t A{");
  for(i = 0 ; i < cardinalityA; i++)
  {
     printf("\n\t %s  (%d) ", uniqueWords1[i], freq1[i]); 
  }
  printf("\n\t}");
  printf("\n\t B{");
  for(i = 0 ; i < cardinalityB; i++)
  {
     printf("\n\t %s  (%d) ", uniqueWords2[i], freq2[i]); 
  }
  printf("\n\t}");  






  int cardinality_of_intersect = computeIntersection(uniqueWords1, uniqueWords2, cardinalityA,cardinalityB);
    int cardinality_of_union = cardinalityA + cardinalityB - cardinality_of_intersect;
    float jaccard_coeff = 100 * (float)cardinality_of_intersect / cardinality_of_union;
    
    printf("\t Similarity between A and B by Jaccard Co-efficient J(A, B) = %.3f \n", jaccard_coeff); 
    fflush(stdout);
  //printf("\n\tAll child processes are over");
  free(offsetNoArray1);
  free(offsetNoArray2);
  fclose(filePointer1);
  fclose(filePointer2);

/**   You can also remove all temporary created files using following code.
    */
  for(i = 0 ; i< k; i++)
  {
      remove(filenames1[i]);
      remove(filenames2[i]);
  }





  for( i=0; i < k; i++)
  {
     free(filenames1[i]);
     free(filenames2[i]);
  }
  
  free(filenames1);
  free(filenames2); 

  for(i=0;i<noOfWords1;i++)
    free(uniqueWords1[i]);

  for(i=0;i<noOfWords2;i++)
    free(uniqueWords2[i]);

  free(uniqueWords1);
  free(uniqueWords2);
  clock_t end = clock();
  double time_spent = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
  printf("\n\t Time Elapsed is %lf msec\n", time_spent);  
  return 0;      
}
