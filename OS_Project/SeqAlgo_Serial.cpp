#include<iostream>
#include<string.h>
#include<string>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<unistd.h>
using namespace std;

class Matrix
{
private:
	int r;
	int c;
	int** mat;
	string g1;
	string g2;
	int match;
	int mismatch;
	int gap;
	int min_penalty; //end result
        //string with least possible value chosen
	
public:
	Matrix(int g1Len, int g2Len)
	{
		r = g2Len + 1;			//g2 length
		c = g1Len + 1; 			//g 1 length;
		mat = new int* [r];
		for (int i = 0; i < r; i++)
		{
			mat[i] = new int[c];
		}
		for (int i = 0; i < r; ++i) 
		{
			for (int j = 0; j < c; ++j) 
			{
				mat[i][j] = 0;
			}
		}
		match = 1;
		mismatch = 3;
		gap = 2;
		min_penalty=0;
	}

	void Print_Matrix()
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				cout << mat[i][j] << "\t";
			}
			cout << "\n";
		}
	}

	void setGs(string g1, string g2)
	{
		this->g1 = g1;
		this->g2 = g2;
	}

	void setPenalty(int mismatch, int gp,int match)
	{
		mismatch = mismatch;
		gap = gp;
		match=match;
	}

	void setMatrix()
	{
		for (int i = 0; i < r; i++)
		{
			mat[i][0] = i * gap;
		}

		for (int i = 0; i < c; i++)
		{
			mat[0][i] = i * gap;
		}
		for (int i = 1; i < r; i++)
		{
			for (int j = 1; j < c; j++)
			{
				if (g1[j - 1] == g2[i - 1])    //Similar g values (A==A ||T==T)
				{
					mat[i][j] = mat[i - 1][j - 1]+match;
				}
				else
				{
					mat[i][j] = max({ mat[i - 1][j - 1] + mismatch , mat[i - 1][j] + gap, mat[i][j - 1] + gap });
				}
			}
		}
	}
	
	void FinalResStrings() 
	{
		int lenG2 = r - 1;		
		int lenG1 = c - 1;	
		int maxLength = lenG2 + lenG1;
		int i = lenG1;								
		int j = lenG2;
		int xpos = maxLength;
		int ypos = maxLength;
						
		int* g1Ans = new int[maxLength + 1];
		int* g2Ans = new int[maxLength + 1];

		while (!(i == 0 || j == 0)) 
		{						
			if (this->g1[i - 1] == this->g2[j - 1]) 
			{		
				g1Ans[xpos--] = (int)this->g1[i - 1];
				g2Ans[ypos--] = (int)this->g2[j - 1];
				i--;
				j--;
				min_penalty= min_penalty + match;
			}
			else if (mat[i - 1][j - 1] + mismatch == mat[i][j]) 
			{	
				g1Ans[xpos--] = (int)this->g1[i - 1];
				g2Ans[ypos--] = (int)this->g2[j - 1];
				i--; 
				j--;
				min_penalty= min_penalty + mismatch;
			}
			else if (mat[i - 1][j] + gap == mat[i][j]) 
			{
				g1Ans[xpos--] = (int)this->g1[i - 1];
				g2Ans[ypos--] = (int)'_';
				i--;
				min_penalty= min_penalty + gap;
			}
			else if (mat[i][j - 1] + gap == mat[i][j]) 
			{
				g1Ans[xpos--] = (int)'_';
				g2Ans[ypos--] = (int)this->g2[j - 1];
				j--;
				min_penalty= min_penalty + gap;
			}
			else if (mat[i - 1][j - 1] + match == mat[i][j])
			{
				g1Ans[xpos--] = (int)this->g1[i - 1];
				g2Ans[ypos--] = (int)this->g2[j - 1];
				i--; 
				j--;
				min_penalty= min_penalty + mismatch;
			}
		}
			
		while (xpos > 0)
		{		
			if (i > 0) 
			{	
				g1Ans[xpos--] = (int)g1[--i];
				min_penalty= min_penalty + gap;	
			}
	
			else g1Ans[xpos--] = (int)'_';	//Filling the starting gaps 
		}
		
		while (ypos > 0)
		{
			if (j > 0)
			{	
				g2Ans[ypos--] = (int)g2[--j];
				min_penalty= min_penalty + gap;
			}
			else 
			{
				g2Ans[ypos--] = (int)'_';		
			}
		}
	
		int gapsEncountered = 1;
		for (int i = maxLength; i >= 1; i--)
		{
			if ( (char)g1Ans[i] == '_' && (char)g2Ans[i] == '_')		
			{
				gapsEncountered = i + 1;
				break;
			}
		}
	
		cout<<endl<<"Step02: Deducing the alignment by tracing back the scoring matrix"<<endl<<endl;
		cout << "The Aligned Gs Are :" << endl;
		for (i = gapsEncountered; i <= maxLength; i++)
		{
			cout<<(char)g1Ans[i];
		}
		cout << endl;
		
		for (i = gapsEncountered; i <= maxLength; i++)
		{
			cout << (char)g2Ans[i];
		}
		
		cout <<endl<< "Minimum Penalty in aligning the gs = ";
		cout << min_penalty << "\n";		
	}	
};

int main()
{
	string g1;    
	string g2;
	int matchPenalty;
	int misMatchPenalty;
	int gapPenalty;

	clock_t start = clock();
	cout<<"Reading Input from file..."<<endl<<endl;
	fstream fin("Input.txt");
	fin >> g1;
	fin >> g2;
	fin >> matchPenalty;
	fin >> misMatchPenalty;
	fin >> gapPenalty;
	
	fin.close();
	
	cout<<"G01: "<<g1<<endl;
	cout<<"G02: "<<g2<<endl;
	cout<<"Match Penalty: "<<matchPenalty<<endl;
	cout<<"Mismatch Penalty: "<<misMatchPenalty<<endl;
	cout<<"Gap Penalty: "<<gapPenalty<<endl;
	
	Matrix dp(g1.length(), g2.length());
	dp.setGs(g1, g2);
	dp.setPenalty(misMatchPenalty, gapPenalty,matchPenalty);
	dp.setMatrix();
	cout<<endl<<endl;
	cout<<"STEP 01: Designing scoring matrix by calculating penalties"<<endl<<endl;
	dp.Print_Matrix();
	dp.FinalResStrings();
	clock_t end = clock();
	
	cout << endl << "Program Execution Time: " << setprecision(4)<< (end-start)  << " seconds" << endl;
	
	fstream fout("Output_Time_Serial.txt",ios::out);
	fout << (end-start);
	fout.close();
}

