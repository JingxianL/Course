/*
 Qi (Sara) Zhang
 Program Name: Project OS2
 
 Purpose:
 
 Input: 
    1) Input from keyboard:
        Filename
    2) Input files:
 
 Output:
    1) Output on console:
        Prompt to enter input filename if enabled, otherwise only the input filename will show up
        Error messages while file not found
    2) Output files:

 - I have abided by the Wheaton College honor code in this work.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdlib> // Avoid the error related to exit() function
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


using namespace std;

// Function declarations
void * Store_Char_Array(void *);
void * Process_One_Line(void *);
void * Align_Display_Text(void *);
const int total_lines = 200;
const int total_chars = 60;
int count = 0;
pthread_mutex_t mutex1, mutex2, mutex3;
const int outcolumn=50;
int main() {
    
    char ** input_content = new char*[total_lines];
    for (int i = 0; i < total_lines; i++) {
        input_content[i] = new char[total_chars];
    }

    pthread_mutex_init (&mutex1, NULL);
    pthread_mutex_init (&mutex2, NULL);
    pthread_mutex_init (&mutex3, NULL);
    pthread_mutex_lock (&mutex2);
    pthread_mutex_lock (&mutex3);

    
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, Store_Char_Array, (void *) input_content);
    pthread_create(&thread2, NULL, Process_One_Line, (void *) input_content);
    pthread_create(&thread3, NULL, Align_Display_Text, (void *) input_content);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    pthread_mutex_destroy (&mutex1);
    pthread_mutex_destroy (&mutex2);
    pthread_mutex_destroy (&mutex3);

    cout << endl << "Done" << endl;

    // // Declare output stream
    // ofstream output_file;
    
    // // Generate the output into an output file called "finalAnswer.txt"
    // output_file.open("finalAnswer.txt");
    
    // output_file.close();
    
    return 0;
}


void * Store_Char_Array(void * ptr)
{
    /* This function
     - Pre-condition:
     - Post-condition:
     - Return: None
     */
    
    
    // Pass the pointer (pass by reference), and then cast void pointer to struct storeSublist
    char **temp_ptr = (char **) ptr;
    
    // Declare and set input filename
    string filename;
    

    
    // Get input filename from keyboard
    cout << "Please enter filename: (e.g., randomNums.txt)\n";
    getline(cin, filename);
    
    cout << "Filename: " << filename << endl;
    
    // Open input file. Exit if cannot open the file, print error message on console
    ifstream input_file;
    input_file.open(filename.c_str());
    if (!input_file.is_open())
    {
        cout<<"Error, file does not exist";
        exit(-1);
    }
    
    
    int count_line = 0;
    string next_line;
    

    while (getline(input_file, next_line)) {
        // getline(input_file, next_line);
        count++;
        pthread_mutex_lock(&mutex1);
        cout << "lock 1" << endl;
        // count--;

        for(int i=0; i<next_line.length(); i++){
            temp_ptr[count_line][i] = next_line[i];
        }
//            Test:
           // cout << count_line << '\t' << temp_ptr[count_line][i] << endl;

        
        count_line++;
        pthread_mutex_unlock(&mutex2);
        // count++;
        cout << count << endl;
        cout << "unlock 2" << endl;

        cout << count << endl;
    }
    

}


void * Process_One_Line(void * ptr)
{
    /* This function 
     - Pre-condition:
     - Post-condition:
     - Return: None
     */
    
    // Pass the pointer (pass by reference), and then cast void pointer to struct storeSublist
    char **temp_ptr = (char **) ptr;



    int j=0;

    for (int i=0; i <= count; i++){
        pthread_mutex_lock(&mutex2);
        // count ++;
        cout << "lock 2" << endl;
        

        // cout << i << endl;
        j=0;
        while (temp_ptr[i][j]!= 0){
            // cout << i << endl;
            // cout << temp_ptr[i][j] << endl;
            if (temp_ptr[i][j]=='\\'){
                if (temp_ptr[i][j+1]=='c'){
                    temp_ptr[i][j+2] = toupper(temp_ptr[i][j+2]);
                    temp_ptr[i][j] = ' ';
                    temp_ptr[i][j+1] =' ';
                }
                else if (temp_ptr[i][j+1]=='C'){
                    
                    int temp = j;
                    while(temp_ptr[i][temp+2] != ' ' && temp_ptr[i][temp+2] != 0){
                        temp_ptr[i][temp+2] = toupper(temp_ptr[i][temp+2]);
                        temp++;
                    }
                    temp_ptr[i][j] = ' ';
                    temp_ptr[i][j+1] = ' ';
                    

                }
                else if(temp_ptr[i][j+1]=='u'){
                    temp_ptr[i][j] = '_';
                    int temp = j;
                    while(temp_ptr[i][temp+1]!=' ' && temp_ptr[i][temp+1] != '\n'){
                        temp_ptr[i][temp+1] = temp_ptr[i][temp+2];
                        temp++;
                    }
                    temp_ptr[i][temp] = '_';
                }
            }
            j++;
        }
        // cout  << temp_ptr[i][0] << endl;
        // count --;
        pthread_mutex_unlock(&mutex3);
        
        cout << "unlock 3" << endl;
    }

    
}


void * Align_Display_Text(void * ptr)
{
    /* This function
     - Pre-condition:
     - Post-condition:
     - Return: None
     */
    
    // Pass the pointer (pass by reference), and then cast void pointer to struct storeSublist
    // Declare output stream
    // ofstream output_file;
    
    // Generate the output into an output file called "finalAnswer.txt"
    char ** temp_ptr = (char **) ptr;

    ofstream outfile;
    outfile.open("finalAnswer.txt");


    for(int i = 0; i <= count; i++){
        pthread_mutex_lock (&mutex3);
        // count ++;
        cout << "lock 3 test" << endl;

        int countnum = 0;
        while(temp_ptr[i][countnum] != 0){
            countnum++;
        }
        cout << "countnum  " << countnum << endl;
        int index = 0;
        for(int j = 0; j < countnum; j++){
            if(temp_ptr[i][j] == ' '){
                index = j;
            }
        }
        cout << "index  " << index << endl;

        for(int j = 0; j < index; j++){
            // ptr2[i][j] = temp_ptr[i][j];
            cout << temp_ptr[i][j];
            outfile << temp_ptr[i][j];  
        }
        
        for(int k = index; k < (outcolumn - countnum + index); k++){
        //  ptr2[i][k] = ' ';
            cout << ' ';
            outfile << ' ';
        }
        
        for(int l = (outcolumn - countnum + index); l < outcolumn; l++){
        //  ptr2[i][l] = temp_ptr[i][l-(50 - countnum)];
            cout << temp_ptr[i][l-(50 - countnum)];
            outfile << temp_ptr[i][l-(outcolumn - countnum)];
        }

        outfile << endl;

        
        cout << endl;
        count--;
        pthread_mutex_unlock (&mutex1);
        // count--;
        cout << "unlock 1 test" << endl;

        cout<< count <<"count" << endl;
        cout << i << "i" << endl;
    }
    // cout << "hahaha" << endl;
    outfile.close();

    
}
