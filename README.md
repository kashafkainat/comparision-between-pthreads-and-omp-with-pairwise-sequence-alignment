#Pairwise-Sequence-Alignment

The project is made to compare the performance of the pairwise sequence algorithm through serial, pthreads and open mp in order to come up with the fastest and most efficient solution. And for that comparision we have used the exectution time of each method and created a graph on python to viually plot its performance.

The project is created on ubuntu 16.04 / linux operating system

#Execution our input is taken by the user than stored in the file Input.txt where it can be read by other cpp files for execution our files can be run separately as well as altogether through final.sh file. the command to run all the methods altogether is : ./final.sh

to run the files separately use:

g++ -o Serial SeqAlgo_Serial.cpp ./Serial

g++ -o Threads SeqAlgo_Pthreads.cpp -lpthread ./Threads

g++ -o OMP SeqAlgo_OpenMP.cpp -fopenmp ./OMP
