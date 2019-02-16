// Nhan Cao
// CS 470
// Shell Project

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <ctype.h>
#define MAX_LINE 80

using namespace std;

void exec_command(istringstream &stream);
void printHistory(int coms_num, vector<string> history);

int main(void)
{
    int should_run = 1;
    
    int coms_num = 0;
    vector<string> history(10);
    
    while(should_run)
    {
        cout<<"\e[1mosh> \e[0m";
        string in;
        getline(cin, in);
        
        istringstream stream(in);
        
        // Parsing the command out of the input line
        string command;
        stream >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        
        // Check if the user wants to quit
        if(command == "exit")
            break;
        
        // Check if the user wants to see the history
        int st_index = coms_num - min(10, coms_num) + 1;
        if(command == "history")
        {
            printHistory(coms_num, history);

        }
        // User want to execute inputed line
        else
        {
            // Check if the user wants to retrieve past command
            if(command[0] == '!')
            {
                int wanted_com = 0;
                
                // Get the most recent command
                if(command[1] == '!' and command.length() == 2)
                {
                    // Print out error if there is nothing in history
                    // and continue to send the command to the child process
                    if (coms_num == 0)
                    {
                        cout<<"\e[1mNo commands in history.\e[0m"<<endl;
                        continue;
                    }
                    // Otherwise assign the number of current command to the wanted_com
                    wanted_com = coms_num;
                } else
                {
                    // Check to see if ! is followed by a number
                    if(isdigit(command[1]))
                    {
                        // Read the number and check if it is in the acceptable range
                        // Print err message if it is and continue to send the command to the child process
                        wanted_com = stoi(command.substr(1, command.length()-1));
                        if(wanted_com < st_index or wanted_com > coms_num) {
                            cout<<"\e[1mNo such command in history.\e[0m"<<endl;
                            continue;
                        }
                    }
                }
                // If it is a valid history retrieving command, get the command and print it out to the screen
                if (wanted_com != 0)
                {
                    in = history[(wanted_com-1)%10];
                    cout<<"> "<<in<<endl;
                }
            }
            
            // Save the command in the history
            coms_num++;
            history[(coms_num-1)%10] = in;
            
            // Sending the fresh stream of input to the exec_command function
            istringstream stream2(in);
            exec_command(stream2);
        }
    }
    //wait(NULL);
    
    return 0;
}

// Executing the user's input
void exec_command(istringstream &stream)
{
    char *args[MAX_LINE/2 + 1];
    string arg;
    bool isWait = true;
    int index = 0;

    // Parsing the input into arguments
    int argc = 0;
    while(stream>>arg)
    {
        args[index] = new char [arg.length()+1];
        strcpy(args[index], arg.c_str());
        index++;
        argc++;
    }
    
    // Checking for the "&" signal
    if(index > 0 and !strcmp(args[index-1],"&"))
    {
        isWait = false;
        index--;
    }
    
    // Add a NULL object in the end of the argument list
    args[index] = NULL;
    
    // fork() for a child process
    pid_t pid = fork();
    if(pid == 0)
    {
        // If the user wishes to wait for the child process
        if(isWait)
        {
            execvp(args[0], args);
            exit(1);
        }
        // If the user wants to run it in the background
        // Applying double fork technique
        else
        {
            pid_t pid2 = fork();
            if(pid2 == 0) {
                execvp(args[0], args);
                exit(1);
            } else {
                exit(0);
            }
        }
    } else
        wait(NULL);
    
    // Deallocate the memory of the arguments
    for(int arg_index = argc - 1; arg_index >= 0; arg_index--) { delete [] args[arg_index]; }
}


// Print out the latest history in specified order
void printHistory(int coms_num, vector<string> history)
{
    for(int com_index=0; com_index < min(10, coms_num); com_index++)
    {
        cout
        <<setw(3)
        <<coms_num - com_index<<" "
        <<history[(coms_num - com_index - 1) % 10]
        <<endl;
    }
}

