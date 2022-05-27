#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	
	
	/*ATCG
ATG
5
-4
-1*/
	
	
	string gene1;
	string gene2;
	int match;
	int miss;
	int gap;
	fstream fileS;
    
    //to create input file first and write content in it
    cout<<"\t** INPUT **\n";
	fileS.open("Input.txt",ios::out);
	if(!fileS)
	{
		cout<<"ERROR,SOURCE FILE NOT FOUND";
	}
	else{
	fflush(stdin);
	cout<<"Enter gene1 to input in file: ";
	getline(cin,gene1);
   	fileS<<gene1;
   	fileS<<endl;
   	cout<<"Enter gene1 to input in file: ";
	getline(cin,gene2);
	fileS<<gene2;
	fileS<<endl;
	cout<<"Enter match penalty,missmatch penalty and gap penalty : ";
	cin>>match>>miss>>gap;
	fileS<<match;
	fileS<<endl;
	fileS<<miss;
	fileS<<endl;
	fileS<<gap;
	fileS<<endl;
	
	fileS.close();
	}
	
    return 0;
}
