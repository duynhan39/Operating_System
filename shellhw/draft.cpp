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

int main(void)
{
    //string args;
    int should_run = 1;
    //    char *args[MAX_LINE/2 + 1];
    
    int coms_num = 0;
    vector<string> history(10);
    
    //char *c;
    //string s = "aga";
    //c = new char [s.length()+1];
    //strcpy(c, s.c_str());
    //c = s.c_str();
    //cout<<c<<endl;
    //char *command = "ls";
    //char *argv[3] = {"-a", "../../..", NULL};
    //execlp("ls", ".", NULL);
    
    //history[0] = "st cool";
    
    //cout<<"Flag"<<endl;
    
    while(should_run)
    {
        cout<<"\e[1mosh> \e[0m";
        string in;
        getline(cin, in);
        
        //ecout<<"Flag1 "<<in<<endl;
        
        //cout<<in<<endl;
        istringstream stream(in);
        
        //cout<<"Flag1.1 "<<in<<endl;
        
        string command;
        stream >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        //cout<<command<<endl;
        
        
        
        //cout<<"Flag2 "<<command<<endl;
        
        if(command == "exit")
            break;
        
        int st_index = coms_num - min(10, coms_num) + 1;
        if(command == "history") {
            
            for(int com_index=st_index; com_index <= coms_num; com_index++) {
                cout
                <<setw(3)
                <<com_index<<" "
                <<history[(com_index - 1) % 10]
                <<endl;
            }
        } else {
            if(command[0] == '!') {
                int wanted_com = 0;
                
                //cout<<"COM "<<command<<" : "<<command.length()<<endl;
                if(command[1] == '!' and command.length() == 2) {
                    if (coms_num == 0) {
                        cout<<"\e[1mNo commands in history.\e[0m"<<endl;
                        continue;
                        //} else {
                        //in = history[(coms_num-1)%10];
                    }
                    wanted_com = coms_num;
                } else {
                    if(isdigit(command[1])) {
                        wanted_com = stoi(command.substr(1, command.length()-1));
                        if(wanted_com < st_index or wanted_com > coms_num) {
                            cout<<"\e[1mNo such command in history.\e[0m"<<endl;
                            continue;
                            //} else {
                            //    in = history[(wanted_com-1)%10];
                        }
                    }
                }
                if (wanted_com != 0) {
                    in = history[(wanted_com-1)%10];
                    cout<<"> "<<in<<endl;
                }
            }
            //cout<<"Flag3 "<<com_num<<endl;
            
            coms_num++;
            
            //cout<<"Flag4 "<<(com_num-1)%10<<endl;
            
            history[(coms_num-1)%10] = in;
            
            //cout<<"Flag5 "<<history[(com_num-1)%10]<<endl;
            
            //cout<<history[(com_num-1)%10]<<endl;
            //cout<<"IN "<<in<<endl;
            istringstream stream2(in);
            
            exec_command(stream2);
        }
        
        //        vector<string> argv;
        //        bool isWait = true;
        //        int index = 0;
        //        while(stream>>arg)
        //        {
        //            //cout<<index<<endl;
        //            args[index] = new char [arg.length()+1];
        //            strcpy(args[index], arg.c_str());
        //            //args[index] =  arg.c_str();
        //            index++;
        //            //cout<<"Flag "<<index<<endl;
        //        }
        //        if(index > 0 and !strcmp(args[index-1],"&"))
        //        {
        //            isWait = false;
        //            index--;
        //        }
        //
        //        if (index == 0) {
        //            args[index] = new char [2];
        //            strcpy(args[index], ".");
        //            index++;
        //        }
        //        args[index] = NULL;
        
        //        exec_command(command, stream);
        
        //    pid_t pid = fork();
        //    if(pid == 0)
        //    {
        //      execvp(command.c_str(), args);
        //      exit(1);
        //    }
        //    else if(isWait)
        //    {
        //      wait(NULL);
        //    }
        
    }
    wait(NULL);
    
    return 0;
}

void exec_command(istringstream &stream) {
    //vector<string> argv;
    //cout<<"FLAG"<<endl;
    
    char *args[MAX_LINE/2 + 1];
    string arg;
    bool isWait = true;
    int index = 0;
    
    //cout<<"FLAG1"<<endl;
    //string command;
    //stream>>command;
    //transform(command.begin(), command.end(), command.begin(), ::tolower);
    //cout<<"CM "<<command<<endl;
    
    //cout<<"FLAG2"<<endl;
    
    
    
    //if(command == "ls") {
    //args[0] = new char [2];
    //strcpy(args[0], ".");
    //    index++;
    //}
    
    //cout<<"FLAG2"<<endl;
    
    while(stream>>arg)
    {
        //cout<<"FLAG3"<<endl;
        //cout<<"ARG: "<<arg<<endl;
        //cout<<index<<endl;
        args[index] = new char [arg.length()+1];
        strcpy(args[index], arg.c_str());
        //args[index] =  arg.c_str();
        index++;
        //cout<<"Flag "<<index<<endl;
    }
    //cout<<"FLAG4"<<endl;
    if(index > 0 and !strcmp(args[index-1],"&"))
    {
        //cout<<"REduce index\n";
        isWait = false;
        index--;
    }
    
    
    //cout<<"FLAG5"<<endl;
    //    if (command == "ls" and index == 1) {
    //        //cout<<"Current\n";
    //        //args[0] = NULL;
    //        args[1] = new char [2];
    //        strcpy(args[1], ".");
    //        index++;
    //    }
    args[index] = NULL;
    
    //cout<<"INDEX "<<index<<endl;
    //cout<<"ARG "<<arg[0]<<endl;
    //cout<<"Command"<<command<<endl;
    int i=0;
    while(args[i] != NULL)
    {
        //cout<<"===ARG "<<i+1<<" "<<args[i]<<endl;
        i++;
    }
    
    pid_t pid = fork();
    if(pid == 0)
    {
        execvp(args[0], args);
        exit(1);
    }
    else if(isWait)
    {
        wait(NULL);
    }
}

