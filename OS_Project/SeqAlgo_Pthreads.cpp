#include<string>
#include<algorithm>
#include<iostream>
#include<string.h>
#include<pthread.h>
#include<fstream>
#include<iomanip>
using namespace std;

//Global variables --shared by all threads
int match_penalty;
int mismatch_penalty;   
int gap_penalty;
int minimum_penalty;//end result
        //string with least possible value chosen

//Multiple arguments passing in threads via struct
struct gene_struct {
    string gene1;
    string gene2;
    int rowSize;
    int *gene1Result,*gene2Result;
    int colSize;
    int **mat;
    int currValue;
};

void *set_matrix_row(void *a)
{
   struct gene_struct *block = (struct gene_struct *) a;
   for (int i = 0; i < block->rowSize; i++)
   {
	block->mat[i][0] = i * gap_penalty;
   }
   
   pthread_exit(0);
}

void *set_matrix_column(void *a)
{
   struct gene_struct *block = (struct gene_struct *) a;
   for (int i = 0; i < block->colSize; i++)
   {
	block->mat[0][i] = i * gap_penalty;
   }
   
   pthread_exit(0);
}

void *set_matrix_remaining_column(void* a)
{
    struct gene_struct *block = (struct gene_struct *) a;
    for(int i = block->currValue+1; i < block->colSize; i++)
    {
      if (block->gene1[i- 1] == block->gene2[block->currValue  - 1])    //Similar gene values (A==A ||T==T)
	{
	   block->mat[block->currValue][i] = block->mat[block->currValue - 1][i - 1]+match_penalty;
	}
	
	else
	{
	   block->mat[block->currValue][i] = max({ block->mat[block->currValue - 1][i - 1] + mismatch_penalty , block->mat[block->currValue -1][i] + gap_penalty, block->mat[block->currValue][i - 1] + gap_penalty });
        }
    }
    
    pthread_exit(0);
}


void *set_matrix_remaining_row(void* a)
{
    struct gene_struct *block = (struct gene_struct *) a;
    for(int i = block->currValue+1; i < block->rowSize; i++)
    {
       if(block->gene1[block->currValue- 1] == block->gene2[i - 1])   
	{
	   block->mat[i][block->currValue] = block->mat[i-1][block->currValue - 1]+match_penalty;
	}
	
	else
	{
	   block->mat[i][block->currValue] = max({block->mat[i-1][block->currValue - 1] + mismatch_penalty , block->mat[i][block->currValue - 1] + gap_penalty, block->mat[i-1][block->currValue] + gap_penalty });
        }
    }
    pthread_exit(0);
}

void set_matrix_diagnol(struct gene_struct block)
{
    pthread_t th1, th2;
    int minimum = min(block.rowSize , block.colSize);
    for (int i = 1; i < minimum; i++)
    {
	if (block.gene1[i - 1] == block.gene2[i - 1])    //Similar gene values (A==A ||T==T)
	{
	    block.mat[i][i] = block.mat[i - 1][i - 1]+match_penalty;
	}
	else
	{
	    block.mat[i][i] = max({block.mat[i - 1][i - 1] + mismatch_penalty , block.mat[i - 1][i] + gap_penalty, block.mat[i][i - 1] + gap_penalty });
        }
     
     block.currValue = i;

     //DATA PARALLELISM
     pthread_create(&th1, NULL, &set_matrix_remaining_row, (void *)&block);
     pthread_create(&th2, NULL ,&set_matrix_remaining_column, (void *)&block);
     pthread_join(th1,NULL); 
     pthread_join(th2,NULL); 
     
     }  
}
void  Final_Resultant_Strings(struct gene_struct block){
		int lenGene2 =block.rowSize - 1;		//Length of gene2
		int lenGene1 = block.colSize - 1;		//length of gene 1
		int maxLength = lenGene2 + lenGene1;
		int i = lenGene1;							
		int j = lenGene2;
		int xpos = maxLength;		
		int ypos = maxLength;
		
		block.gene1Result = new int[maxLength + 1];
		block.gene2Result = new int[maxLength + 1];

		while (!(i == 0 || j == 0)) 
		{
			if (block.gene1[i - 1] == block.gene2[j - 1]) 
			       {

				block.gene1Result[xpos--] = (int)block.gene1[i - 1];
				block.gene2Result[ypos--] = (int)block.gene2[j - 1];
				i--;
				j--;
                               minimum_penalty= minimum_penalty + match_penalty;
			        }

			 else if (block.mat[i - 1][j - 1] + mismatch_penalty == block.mat[i][j]) 
			       {	

				block.gene1Result[xpos--] = (int)block.gene1[i - 1];
				block.gene2Result[ypos--] = (int)block.gene2[j - 1];
				i--; j--;
				minimum_penalty= minimum_penalty + mismatch_penalty;
				
			       }
			       
			 else if (block.mat[i - 1][j] + gap_penalty == block.mat[i][j]) 
			      {	//FOR LEFT CASE
			
				block.gene1Result[xpos--] = (int)block.gene1[i - 1];
				block.gene2Result[ypos--] = (int)'_';
				i--;
				minimum_penalty= minimum_penalty + gap_penalty;
				
			      }
			      
			 else if (block.mat[i][j - 1] + gap_penalty == block.mat[i][j]) 
			      {	//FOR UP CASE.

				block.gene1Result[xpos--] = (int)'_';
				block.gene2Result[ypos--] = (int)block.gene2[j - 1];
				j--;
				minimum_penalty= minimum_penalty + gap_penalty;
			
			     }
			     
			 else if (block.mat[i - 1][j - 1] + match_penalty == block.mat[i][j])
			     {
							
				block.gene1Result[xpos--] = (int)block.gene1[i - 1];
				block.gene2Result[ypos--] = (int)block.gene2[j - 1];				
				i--; j--;
				minimum_penalty= minimum_penalty + mismatch_penalty;
			    
			    }
                    }
                 
                 //while loops here(for xpos and ypos) are used to fill/create gaps in the start of the string when required
		 while (xpos > 0)
		    {		

			if (i > 0) 
			{
				
			  block.gene1Result[xpos--] = (int)block.gene1[--i];
			  minimum_penalty= minimum_penalty + gap_penalty;
				
			}
	
			else block.gene1Result[xpos--] = (int)'_';	//Filling the starting gaps 
			
		    }
		    
		while (ypos > 0)
		{
			
			if (j > 0)
			{
			
			  block.gene2Result[ypos--] = (int)block.gene2[--j];
			  minimum_penalty= minimum_penalty + gap_penalty;
			}
			else 
			{
			
			  block.gene2Result[ypos--] = (int)'_';	//Filling the starting gaps.
			
			}
		}
		
		int gapsEncountered = 1;
		for (int i = maxLength; i >= 1; i--)
		{
			if ( (char)block.gene1Result[i] == '_' && (char)block.gene2Result[i] == '_')		
			{
				gapsEncountered = i + 1;
				break;
			}
		}
		
	    	cout<<endl<<"Step02: Deducing the alignment by tracing back the scoring matrix"<<endl<<endl;
		cout << "The Aligned Genes Are :" << endl;
	        for (i = gapsEncountered; i <= maxLength; i++)
	        {
	          cout<<(char)block.gene1Result[i];
	        }
	        cout << "\n";
	        for (i = gapsEncountered; i <= maxLength; i++)
	        {
	          cout << (char)block.gene2Result[i];
	        }
                
               cout <<endl<< "Minimum Penalty in aligning the genes = ";
	    	cout << minimum_penalty << "\n";
}
 
void Print_Matrix(struct gene_struct block)
{
	      cout<<"\n";
		for (int i = 0; i < block.rowSize; i++)
		{
			for (int j = 0; j < block.colSize; j++)
			{
				cout << block.mat[i][j] << "\t";
			}
			cout << "\n";
		}
}

// Compare the output each time with series.cpp by executing the file like this: time ./run.sh

int main()
{
        pthread_t ptid1, ptid2;
        string g1, g2;
        struct gene_struct block; 
        cout<<"Reading Input from file..."<<endl<<endl;  
	fstream fin("Input.txt");
	fin>>g1;
	fin>>g2;
	fin>>match_penalty;
	fin>>mismatch_penalty;
	fin>>gap_penalty;
	
	
	cout<<"Gene01: "<<g1<<endl;
	cout<<"Gene02: "<<g2<<endl;
	cout<<"Match Penalty: "<<match_penalty<<endl;
	cout<<"Mismatch Penalty: "<<mismatch_penalty<<endl;
	cout<<"Gap Penalty: "<<gap_penalty<<endl;
	
        block.gene1 = g1;
        block.gene2 = g2;
        block.rowSize = g2.length()+1;
        block.colSize = g1.length()+1; 
        block.mat = new int* [block.rowSize];  //col = gene2+1
        for (int i = 0; i < block.rowSize; i++)
        {
		block.mat[i] = new int[block.colSize];
             
	 }
	 
	clock_t start = clock();	
        //DATA PARALLELISM
        pthread_create(&ptid1, NULL, &set_matrix_row, (void *)&block);
        pthread_create(&ptid2, NULL ,&set_matrix_column, (void *)&block);
        pthread_join(ptid1,NULL); 
        pthread_join(ptid2,NULL); 
        set_matrix_diagnol(block);
	cout<<endl<<endl;
	cout<<"STEP 01: Designing scoring matrix by calculating penalties"<<endl<<endl;
        Print_Matrix(block);
        Final_Resultant_Strings(block);
        clock_t end = clock();
        cout << endl << "Program Execution Time: " << setprecision(4)<< double((end-start)/double(CLOCKS_PER_SEC))  << " seconds" << endl;
        fstream fout("EXTime_Pthread.txt",ios::out);
        fout << double((end-start)/double(CLOCKS_PER_SEC));
        fout.close();
}

