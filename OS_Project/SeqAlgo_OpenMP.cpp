#include<iostream>
#include<algorithm>
#include<string.h>
#include<string>
#include<fstream>
#include<omp.h>
#include<iomanip>
using namespace std;

int matchPenalty;
int misMatchPenalty;   
int gapPenalty;
int minimum_penalty;//end result
        //string with least possible value chosen

struct gene_struct {
    string gene1;
    string gene2;
    int rowSize;
    int colSize;
    int **mat;
    int *gene1Result;
    int *gene2Result;
};

void Set_Matrix(struct gene_struct gene_block){

	#pragma omp parallel num_threads(gene_block.rowSize)
	#pragma omp for  
	for (int i = 0 ; i < gene_block.rowSize ; i++)
	{
		gene_block.mat[i][0] = i * gapPenalty;
	}
	
	#pragma omp parallel num_threads(gene_block.colSize)
	#pragma omp for 
	for (int i = 0 ; i < gene_block.colSize ; i++)
	{
		gene_block.mat[0][i] = i * gapPenalty;
	}

	for (int i = 1 ; i < gene_block.rowSize ; i++)
	{
		#pragma omp parallel num_threads(gene_block.colSize)
		#pragma omp for 
		for (int j = 1; j < gene_block.colSize ; j++)
		{
			if (gene_block.gene1[j - 1] == gene_block.gene2[i - 1])    //Similar gene values (A==A ||T==T)
			{
				gene_block.mat[i][j] = gene_block.mat[i - 1][j - 1] + matchPenalty;
			}
			else
			{
				gene_block.mat[i][j] = max({ gene_block.mat[i - 1][j - 1] + misMatchPenalty , gene_block.mat[i - 1][j] + gapPenalty, gene_block.mat[i][j - 1] + gapPenalty });
			}
		}
	}
}
void Final_Resultant_Strings(struct gene_struct gene_block) 
{
	int lenGene2 = gene_block.rowSize - 1;		
	int lenGene1 = gene_block.colSize - 1;	

	int maxLength = lenGene2 + lenGene1;

	int i = lenGene1;								
	int j = lenGene2 ;

	int ypos = maxLength ;
	int xpos = maxLength;
				
	gene_block.gene1Result = new int[maxLength + 1];
	gene_block.gene2Result = new int[maxLength + 1];
	
	while (!(i == 0 || j == 0)) 
	{					
		if (gene_block.gene1[i - 1] == gene_block.gene2[j - 1]) 
		{							
			gene_block.gene1Result[xpos--] = (int)gene_block.gene1[i - 1];
			gene_block.gene2Result[ypos--] = (int)gene_block.gene2[j - 1];
			i--;
			j--;
        		minimum_penalty= minimum_penalty + matchPenalty;
		}
		else if (gene_block.mat[i - 1][j - 1] + misMatchPenalty == gene_block.mat[i][j]) 
		{
			gene_block.gene1Result[xpos--] = (int)gene_block.gene1[i - 1];
			gene_block.gene2Result[ypos--] = (int)gene_block.gene2[j - 1];
			i--; 
			j--;
			minimum_penalty= minimum_penalty + misMatchPenalty;
		}
		else if (gene_block.mat[i - 1][j] + gapPenalty == gene_block.mat[i][j]) 
		{	
			gene_block.gene1Result[xpos--] = (int)gene_block.gene1[i - 1];
			gene_block.gene2Result[ypos--] = (int)'_';
			i--;
			minimum_penalty= minimum_penalty + gapPenalty;
		}
		else if (gene_block.mat[i][j - 1] + gapPenalty == gene_block.mat[i][j]) 
		{	
			gene_block.gene1Result[xpos--] = (int)'_';
			gene_block.gene2Result[ypos--] = (int)gene_block.gene2[j - 1];
			j--;
			minimum_penalty= minimum_penalty + gapPenalty;
		}
		else if (gene_block.mat[i - 1][j - 1] + matchPenalty == gene_block.mat[i][j])
		{
			gene_block.gene1Result[xpos--] = (int)gene_block.gene1[i - 1];
			gene_block.gene2Result[ypos--] = (int)gene_block.gene2[j - 1];
			i--; 
			j--;
			minimum_penalty= minimum_penalty + misMatchPenalty;
		}
	}
    
        //while loops here(for xpos and ypos) are used to fill/create gaps in the start of the string when required
	
	#pragma omp parallel num_threads(maxLength)
	{
		#pragma omp for 	
		
		for (int x = xpos ; x > 0 ; x--)
		{		
			if (i > 0) 
			{
				gene_block.gene1Result[x] = (int)gene_block.gene1[--i];
				minimum_penalty= minimum_penalty + gapPenalty;
			}

			else 
			{
				gene_block.gene1Result[x] = (int)'_';	//Filling the starting gaps 
			}
		}
	}
	#pragma omp parallel num_threads(maxLength)
	{
		#pragma omp for 
	
		for (int y = ypos ; y > 0 ; y--)
		{
			if (j > 0)
			{	
				 gene_block.gene2Result[y] = (int)gene_block.gene2[--j];
				 minimum_penalty= minimum_penalty + gapPenalty;
			}
			else 
			{
				gene_block.gene2Result[y] = (int)'_';
			
			}
		}
	}
	int gapsEncountered = 1;
	 
	for (int i = maxLength; i >= 1; i--)
	{
		if ((char)gene_block.gene1Result[i] == '' && (char)gene_block.gene2Result[i] == '')
		{
			gapsEncountered = i + 1;
			
			break;
		}
	}

	cout<<endl<<"Step02: Deducing the alignment by tracing back the scoring matrix"<<endl<<endl;
	cout << "The Aligned Genes Are :" << endl;
	
	for (i = gapsEncountered; i <= maxLength; i++)
	{
		cout << (char)gene_block.gene1Result[i];
	}
	
	cout << endl;
	
	for (i = gapsEncountered; i <= maxLength; i++)
	{
		cout << (char)gene_block.gene2Result[i];
	}
	
	cout << endl;	
	cout << "Minimum Penalty in aligning the genes = " << minimum_penalty << endl;
		
}	
void Print_Matrix(struct gene_struct gene_block)
{
	cout << endl;
	
	for (int i = 0; i < gene_block.rowSize; i++)
	{
		for (int j = 0; j < gene_block.colSize; j++)
		{
			cout << gene_block.mat[i][j] << "\t";
		}
		cout << "\n";
	}
}

int main(){
	
	double start; 
	double end; 
	start = omp_get_wtime();
	
	string gene1, gene2;
	struct gene_struct gene_block;
	cout<<"Reading Input from file..."<<endl<<endl;  
	fstream fin("Input.txt");
	fin >> gene1;
	fin >> gene2;
	fin >> matchPenalty;
	fin >> misMatchPenalty;
	fin >> gapPenalty;
	
	cout<<"Gene01: "<<gene1<<endl;
	cout<<"Gene02: "<<gene2<<endl;
	cout<<"Match Penalty: "<<matchPenalty<<endl;
	cout<<"Mismatch Penalty: "<<misMatchPenalty<<endl;
	cout<<"Gap Penalty: "<<gapPenalty<<endl;
	
	gene_block.gene1 = gene1;
	gene_block.gene2 = gene2;
	gene_block.rowSize = gene2.length()+1;
	gene_block.colSize = gene1.length()+1;

	gene_block.mat = new int* [gene_block.rowSize];
	
	#pragma omp parallel num_threads(gene_block.rowSize)
	{
		#pragma omp for 
		for (int i = 0; i < gene_block.rowSize; i++)
		{
			gene_block.mat[i] = new int[gene_block.colSize];   
		}	
 	}
	 
	Set_Matrix(gene_block);
	cout<<endl<<endl;
	cout<<"STEP 01: Designing scoring matrix by calculating penalties"<<endl<<endl;
	Print_Matrix(gene_block);
	Final_Resultant_Strings(gene_block);
	end = omp_get_wtime(); 
	cout << endl << "Program Execution Time:" << setprecision(4)<<(end-start)<< " seconds" << endl;	
	fstream fout("EXTime_OpenMP.txt" , ios::out);
	fout << (end-start);
	fout.close();
	return 0;
}
