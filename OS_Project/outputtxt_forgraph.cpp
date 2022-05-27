#include<iostream>
#include<fstream>
using namespace std;



int main()
{
	string s,c,context;
	fstream fileS;
    fstream fileD;
	//copy the Source file content into Destination file
    fileS.open("Output_Time_Serial.txt");
    fileD.open("output.txt",ios::out);
  
    while(!fileS.eof())
    {
    	fileD<<"SERIAL ";
        getline(fileS,s);
        fileD <<s<<endl;
    }
    fileS.close();
    fileS.open("Output_Time_Pthread.txt");
    
  
    while(!fileS.eof())
    {
    	fileD<<"Pthread ";
        getline(fileS,s);
        fileD <<s<<endl;
    }
    
    fileS.close();
    
    
    fileS.open("Output_Time_OpenMP.txt");
    
  
    while(!fileS.eof())
    {
    	fileD<<"OPENMP ";
        getline(fileS,s);
        fileD <<s<<endl;
    }
    
    fileS.close();
    
    
    fileD.close();
  
}
