echo "\nTaking input"
g++ -o input program_input.cpp
./input

echo "\n--------------------------------------------------------------------"
echo "\nRunning Serial Program"
echo "\n--------------------------------------------------------------------\n"
g++ -o Serial SeqAlgo_Serial.cpp 
./Serial

echo "\n\n\n--------------------------------------------------------------------"
echo "\nRunning Using Pthreads"
echo "\n--------------------------------------------------------------------\n"
g++ -o Threads SeqAlgo_Pthreads.cpp -lpthread
./Threads


echo "\n\n\n--------------------------------------------------------------------"
echo "\nRunning Using OpenMP"
echo "\n--------------------------------------------------------------------\n"
g++ -o OMP SeqAlgo_OpenMP.cpp -fopenmp
./OMP
