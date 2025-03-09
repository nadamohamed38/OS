#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>  
#include <climits>
using namespace std;
struct Process{
char name;
int arrival_time,service_time,turnaround_time,finish_time,periority,p;
int waiting_time = 0 , serv = 0;
float normturn_time , ratio = 0.0;
bool isWaiting = false;
};

void printTrace(vector<Process> processes , string algoName){

    if(algoName.length() == 4){
        cout << algoName << "  ";
    }else if(algoName.length() == 5){
        cout << algoName << " ";
    }
    else{
        cout << algoName << "   ";
    }


    cout << "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";
    cout << "------------------------------------------------\n";


    const int rows = processes.size();;
    const int cols = 21;


    for (int i = 0; i < rows; i++) {
        char rowLabel = processes[i].name;
        cout << rowLabel << "     |";
        int w = processes[i].waiting_time +processes[i].arrival_time;
        for (int j = 0; j < cols; j++) {
          if(j < processes[i].arrival_time){
              cout << " |"; 
            }
            else if (w > j) { 
              if(j == cols-1){
              cout << ".";
              }
                else {cout << ".|"; } 
            } else if (processes[i].finish_time > j && processes[i].waiting_time <= j) {
                if(j == cols-1) {cout << "*"; } // Process is running
                else {cout << "*|";} 
            } else {
                if(j == cols-1) cout << " ";
                else cout << " |";  // Process has finished
            }
            
        }
        cout << endl;
    }

    // Footer
    cout << "------------------------------------------------\n";
     cout << "\n";
}

void printStats(vector<Process>& processes , string mode) {
  
  
    cout << mode << endl;
    cout << "Process    |";
    float total_turnaround = 0, total_norm_turnaround = 0;
    for (int i=0;i<processes.size();i++) {
        total_turnaround += processes[i].turnaround_time;
        total_norm_turnaround += processes[i].normturn_time;
    }
    for (int i=0;i<processes.size();i++) {
        cout << setw(3)<< processes[i].name <<setw(3)<< "|";
    }
    cout << endl;

    cout << "Arrival    |";
    for (int i=0;i<processes.size();i++) {
        std::string str = std::to_string(processes[i].arrival_time);
        if(str.length() <4)
        cout << setw(3)<< processes[i].arrival_time<<setw(3) << "|";
        else
        cout << setw(5)<< processes[i].arrival_time<< "|";
    }
    cout << endl;

    cout << "Service    |";
    for (int i=0;i<processes.size();i++) {
    std::string str = std::to_string(processes[i].service_time);
        if(str.length() <4)
        cout << setw(3)<< processes[i].service_time<<setw(3) << "|";
        else
        cout << setw(5)<< processes[i].service_time<< "|";
    }
     cout << " Mean|";
    cout << endl;

    cout << "Finish     |";
    for (int i=0;i<processes.size();i++) {
    std::string str = std::to_string(processes[i].finish_time);
        if(str.length() <4)
        cout << setw(3)<< processes[i].finish_time<<setw(3) << "|";
        else
        cout << setw(5)<< processes[i].finish_time<< "|";
    }
    cout << "-----|";
    cout << endl;

    cout << "Turnaround |";
    for (int i=0;i<processes.size();i++) {
    std::string str = std::to_string(processes[i].turnaround_time);
        if(str.length() <4)
        cout << setw(3)<< processes[i].turnaround_time<<setw(3) << "|";
        else
        cout << setw(5)<< processes[i].turnaround_time<< "|";
    }

    cout << setw(5) << fixed << setprecision(2) << (total_turnaround / processes.size()) << "|";
    cout << endl;

    cout << "NormTurn   |";
    for (int i=0;i<processes.size();i++) {
    std::string str = std::to_string(processes[i].normturn_time);
        if(str.length() <4)
        cout << setw(3)<< processes[i].normturn_time << "|";
        else
        cout << setw(5)<< processes[i].normturn_time<< "|";
    }
    cout << setw(5) << fixed << setprecision(2) << (total_norm_turnaround / processes.size()) << "|";
    cout << endl;
    cout << endl;
   
}

void setServ(vector<Process>& p){
      for(int i = 0 ; i <p.size() ; i++){
                p[i].serv = 0;
      }
}

void simulateFCFS(vector<Process> processes, string& mode){
   
    for(int i=0;i<processes.size();i++)
    {if(i==0)
    {processes[i].finish_time=processes[i].arrival_time+processes[i].service_time;
    processes[i].turnaround_time=processes[i].finish_time-processes[i].arrival_time;
  processes[i].normturn_time=static_cast<float>(processes[i].turnaround_time)/processes[i].service_time;
  
    }
    else
    {processes[i].finish_time=processes[i-1].finish_time+processes[i].service_time;
     processes[i].turnaround_time=processes[i].finish_time-processes[i].arrival_time;
    if(processes[i-1].finish_time>processes[i].arrival_time)
    {processes[i].waiting_time=processes[i-1].finish_time-processes[i].arrival_time;
    }
    processes[i].normturn_time=static_cast<float>(processes[i].turnaround_time)/processes[i].service_time;
    }
}
 if(mode.compare("trace")==0)
    {
  printTrace(processes,"FCFS");
}
else if(mode.compare("stats")==0)
    {
  printStats(processes,"FCFS");
}

}

void simulateRR(vector<Process> processes, string mode, int simulation_end_time, int q_time) {
    vector<Process> p; // Ready queue
    int current = 0; // Current time
    int j = 0; // Index to track added processes
    vector<char> output(simulation_end_time, '.'); // Output timeline
    vector<Process> p2; // Temporary queue for processes arriving during execution
setServ(processes);
    // Initial population of the ready queue
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].arrival_time <= current) {
            p.push_back(processes[i]);
            j++;
        }
    }

    // Simulation loop for round robin
    while (current < simulation_end_time) {
  
        if (!p.empty()) {
            Process currentProcess = p.front();
            p.erase(p.begin());  

            int min_time = min(q_time, currentProcess.service_time - currentProcess.serv);

            // Simulate the process execution
            for (int i = 0; i < min_time; i++) {
                if (current < simulation_end_time) {
                    output[current] = currentProcess.name;
                    current++;
                }
            }

            currentProcess.serv += min_time;
            // Update the process information
            auto it = find_if(processes.begin(), processes.end(), [&](Process& p) {
                return p.name == currentProcess.name;
            });
            if (it != processes.end()) {
                *it = currentProcess;
            }

            // If the process is not finished, add it back to the queue
            if (currentProcess.serv < currentProcess.service_time) {
                // Check for new arrivals during the process execution
                for (int i = j; i < processes.size(); i++) {
                    if (processes[i].arrival_time <= current && processes[i].serv == 0) {
                        p2.push_back(processes[i]);
                        j++;
                    }
                }
                p2.push_back(currentProcess);  
                p.insert(p.end(), p2.begin(), p2.end());  
                p2.clear();

            } else {
                for (int i = j; i < processes.size(); i++) {
                    if (processes[i].arrival_time <= current && processes[i].serv == 0) {
                        p.push_back(processes[i]);
                        j++;
                    }
                }

            }
        } else {
            current++;  // If no process is ready, increment time
            // Add new arrivals to the queue
            for (int i = j; i < processes.size(); i++) {
                if (processes[i].arrival_time <= current && processes[i].serv == 0) {
                    p.push_back(processes[i]);
                    j++;
                }
            }
        }
    }


    // Calculate finish times, turnaround times, and waiting times
    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < simulation_end_time; j++) {
            if (output[j] == processes[i].name) {
                processes[i].finish_time = j + 1;
            }
        }
    }

    // Calculate turnaround times, waiting times, and normalized turnaround times
    for (int i = 0; i < processes.size(); i++) {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        if (i > 0 && processes[i - 1].finish_time > processes[i].arrival_time) {
            processes[i].waiting_time = processes[i - 1].finish_time - processes[i].arrival_time;
        }
        processes[i].normturn_time = static_cast<float>(processes[i].turnaround_time) / processes[i].service_time;
    }

    // Print trace or stats based on mode
    if (mode == "trace") {
        // Print trace table header
        cout << "RR-" << q_time << "  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";
        cout << "------------------------------------------------\n";

        // Print each process's execution timeline
        for (int i = 0; i < processes.size(); i++) {
            cout << processes[i].name << "     |";
            for (int j = 0; j < simulation_end_time; j++) {
                if (output[j] == processes[i].name) {
                    cout << "*|";  // Process is executing
                } else if (j >= processes[i].arrival_time && j < processes[i].finish_time) {
                    cout << ".|";  // Process is idle
                } else {
                    cout << " |";  // No execution at this time unit
                }
            }
            cout << " " << endl;
        }

        cout << "------------------------------------------------\n";
        cout << endl;
    } else if (mode == "stats") {
    std::string str = "RR-" + std::to_string(q_time);
        printStats(processes, str);
    }
}


void simulateSPN(vector<Process> processes, string mode){

int current = 0,count=0;
    setServ(processes);
    while (count < processes.size()) {
        int num_of_processes = -1;
        int min_service = INT_MAX;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current && processes[i].serv < processes[i].service_time) 
            {
               if (processes[i].service_time < min_service) {
                    min_service = processes[i].service_time;
                    num_of_processes = i;
                }
            }
        }
      
       if (num_of_processes !=-1) {
            current += (processes[num_of_processes].service_time - processes[num_of_processes].serv);
            processes[num_of_processes ].finish_time = current;
            processes[num_of_processes ].turnaround_time = processes[num_of_processes ].finish_time - processes[num_of_processes ].arrival_time;
            processes[num_of_processes ].waiting_time = processes[num_of_processes ].turnaround_time - processes[num_of_processes ].service_time;
            processes[num_of_processes ].normturn_time = static_cast<float>(processes[num_of_processes ].turnaround_time) / processes[num_of_processes].service_time;
            processes[num_of_processes ].serv = processes[num_of_processes ].service_time;
            count++;
        } else {
            current++;
        }
    }
    if (mode.compare("trace") == 0) {
        printTrace(processes, "SPN");
  } else if (mode.compare("stats") == 0) {
        printStats(processes, "SPN");
    }
}

void simulateSRT(vector<Process>& processes, string mode ,int simulation_end_time) {
    
int current = 0;
    int count = 0;
    int n = processes.size();
    vector<int> remaining_time(n);
    vector<bool> is_completed(n, false);
    vector<char> output(simulation_end_time, ' ');
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].service_time;
    }

    while (count < processes.size()) {
        int num_of_processes = -1;
        int min_remaining= INT_MAX;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current && !is_completed[i] && remaining_time[i] < min_remaining) {
                min_remaining = remaining_time[i];
                num_of_processes = i;
            }
        }

        if (num_of_processes != -1) {
            output[current] = processes[num_of_processes].name;
            remaining_time[num_of_processes]--;
            current++;
            if (remaining_time[num_of_processes] == 0) {
                processes[num_of_processes].finish_time = current;
                processes[num_of_processes].turnaround_time = processes[num_of_processes].finish_time - processes[num_of_processes].arrival_time;
                processes[num_of_processes].normturn_time = static_cast<float>(processes[num_of_processes].turnaround_time) / processes[num_of_processes].service_time;
                processes[num_of_processes].waiting_time = processes[num_of_processes].turnaround_time - processes[num_of_processes].service_time;
                is_completed[num_of_processes] = true;
                count++;
            }
        } else {
            current++;
        }
    }

    
if (mode.compare("trace") == 0) {
  
 cout << "SRT   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";

    cout << "------------------------------------------------\n";

    for (int i=0;i<processes.size();i++) {

        cout << processes[i].name << "     |";

        for (int j = 0; j < output.size(); j++) {

            if (output[j] == processes[i].name) {

                cout << "*|";

            } else if (processes[i].arrival_time <= j && j < processes[i].finish_time) {

                cout << ".|"; 

            } else {

                cout << " |"; 

            }

        }

        cout <<" "<< endl;

    }

    cout << "------------------------------------------------\n";
    cout << endl;
    }
    else if (mode.compare("stats") == 0) {
        printStats(processes, "SRT");
    }

}

int calcRatio(vector<Process>& p, int x){
    for (int i = 1 ; i < p.size() ; i++){
        if(p[i].isWaiting){
        if(p[x].finish_time < p[i].arrival_time){
            p[i].waiting_time = 0;
        }
        else{
        p[i].waiting_time = p[x].finish_time-p[i].arrival_time;
        }
        p[i].ratio = static_cast<float>(p[i].waiting_time +p[i].service_time)/p[i].service_time;
        p[i].isWaiting = true;
        }
    }
    int max = 0;
    for (int i = 1 ; i < p.size() ; i++){
    
        if(p[i].isWaiting){
        if(p[max].ratio < p[i].ratio)
        max = i;
        
        }
    }
    return max;
}
void simulateHRRN(vector<Process> processes, string mode){
    int i = 0;
    int n = processes.size();
    int j = 0;
    int x = 0;
    std::vector<Process> p;
    for (int i = 1 ; i < n ; i++){ 
        processes[i].isWaiting = true;
    }
    while (j < n){
      if(i == 0){
        processes[i].finish_time=processes[i].arrival_time+processes[i].service_time;
        processes[i].turnaround_time=processes[i].finish_time-processes[i].arrival_time;
         processes[i].normturn_time=static_cast<float>(processes[i].turnaround_time)/processes[i].service_time;
         processes[i].isWaiting = false;
         p.push_back(processes[i]);
         i++;
      }
      else{
         i = calcRatio(processes,x);
         processes[i].isWaiting = false;
          p.push_back(processes[i]);
          p[j].finish_time=p[j-1].finish_time+p[j].service_time;
          p[j].turnaround_time=p[j].finish_time-p[j].arrival_time;
          p[j].normturn_time=static_cast<float>(p[j].turnaround_time)/p[j].service_time;
           processes[i].finish_time=p[j].finish_time;
        processes[i].turnaround_time=p[j].turnaround_time;
         processes[i].normturn_time=p[j].normturn_time;
          x=i;
      }
      
      j++;
    }
   if(mode.compare("trace")==0)
    {
  printTrace(processes,"HRRN");
}
else if(mode.compare("stats")==0)
    {
  printStats(processes,"HRRN");
}
}


void simulateFB1(vector<Process>& processes, string mode, int simulation_end_time) {
    vector<Process> p;  
    int current = 0;  
    int i = 0; 
    vector<char> output(simulation_end_time, '.'); 
    vector<Process> p2;
    setServ(processes);
    while (current < simulation_end_time) {
        while (i < processes.size() && processes[i].arrival_time == current) {
            output[current] = processes[i].name;
            current++;
            p.push_back(processes[i]);
            i++; 
        }
      
        if (!p.empty()) {
            Process& currentProcess = p.front();
            output[current] = currentProcess.name;  
            current++;
            int num = 0;
            for(int j = 0 ; j < simulation_end_time ; j++){
            if(output[j] == currentProcess.name){
            num++;
            }
            }
            if (num < currentProcess.service_time) {
               currentProcess.serv++;
                p.push_back(currentProcess);
                p.erase(p.begin());
                int j = 0;
                while (j < p.size() && processes[i-1].name != p.front().name){
                p.push_back(p.front());
                p.erase(p.begin());
                j++;
                }
                for ( j = 0 ; j < p.size()-1 ; j++){
                    if(p[j].serv == p[j+1].serv){
                    int y = -1 , x = -1;
                    for(int l = 0 ; l < processes.size() ; l++){
                    if(p[j].name == processes[l].name)
                    x = l;
                    if(p[j+1].name == processes[l].name)
                    y = l;
                    }
                    if(y < x){
                    Process temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                    
                    }
                    }
                }
            }
            else
            p.erase(p.begin());  
        }
        
        if (p.empty() && i < processes.size() && processes[i].arrival_time > current) {
            current++;
        }
        
    
    }
    
    
    for(int i=0;i<processes.size();i++){
    for(int j = 0 ; j < simulation_end_time ; j++){
            if(output[j] == processes[i].name){
            processes[i].finish_time = j+1;
            }
            }
            }

    for(int i=0;i<processes.size();i++)
    {
    if(i==0)
    {
    processes[i].turnaround_time=processes[i].finish_time-processes[i].arrival_time;
    processes[i].normturn_time=static_cast<float>(processes[i].turnaround_time)/processes[i].service_time;
  
    }
    else
    {
     processes[i].turnaround_time=processes[i].finish_time-processes[i].arrival_time;
    if(processes[i-1].finish_time>processes[i].arrival_time)
    {processes[i].waiting_time=processes[i-1].finish_time-processes[i].arrival_time;
    }
    processes[i].normturn_time=static_cast<float>(processes[i].turnaround_time)/processes[i].service_time;
    }
}

if(mode.compare("stats")==0)
    {
  printStats(processes,"FB-1");
}
    else{
    cout << "FB-1  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";
    cout << "------------------------------------------------\n";

    // Print each process's execution timeline
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].name << "     |";

        for (int j = 0; j < simulation_end_time; j++) {
            if (output[j] == processes[i].name) {
                cout << "*|";  // Process is executing
            } else if (j >= processes[i].arrival_time && j < processes[i].finish_time) {
                cout << ".|";  // Process is idle
            } else {
                cout << " |";  // No execution at this time unit
            }
            
        }
        cout<< " " << endl;
    }

    cout << "------------------------------------------------\n";
    cout << endl;

}
    
}



void simulateFB2i(vector<Process>& processes, string mode, int simulation_end_time) {
    vector<Process> p;  
    int current = 0;  
    int i = 0; 
    vector<char> output(simulation_end_time, '.'); 
    setServ(processes);
    while (current < simulation_end_time) {
    if(i == 0){
     while (processes[i].arrival_time < current) {
            output[current] = processes[i].name; 
            current++; 
        }
        processes[i].serv = 1; 
        p.push_back(processes[i]);
            i++;
    }
      while (i < processes.size() && processes[i].arrival_time <= current ) {
            output[current] = processes[i].name;
            current++;
            processes[i].serv = 1;  
            p.push_back(processes[i]);
            i++;
        }
     
        
        if (!p.empty()) {
            Process& currentProcess = p.front();
            int num = 0;
             for (int h = 0; h < simulation_end_time; h++) {
                if (currentProcess.name == output[h]) {
                    num++;
                }
            }
            

            for (int h = 0; h < pow(2, currentProcess.serv); h++) {
                if (num < currentProcess.service_time) {
                    output[current] = currentProcess.name;
                    current++;
                    num++;
                }
            }
            
              if (num < currentProcess.service_time) {
               currentProcess.serv++;
                p.push_back(currentProcess);
                p.erase(p.begin());
                int j = 0;
                while (j < p.size() && processes[i-1].serv != p.front().serv){
                p.push_back(p.front());
                p.erase(p.begin());
                j++;
                }
                 for ( j = 0 ; j < p.size()-1 ; j++){
                    if(p[j].serv == p[j+1].serv){
                    int y = -1 , x = -1;
                    for(int l = 0 ; l < processes.size() ; l++){
                    if(p[j].name == processes[l].name)
                    x = l;
                    if(p[j+1].name == processes[l].name)
                    y = l;
                    }
                    if(y < x){
                    Process temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                    
                    }
                    }
                }
               
            } else {
              
                p.erase(p.begin());
            }
        }
      
        if (p.empty() && i < processes.size() && processes[i].arrival_time > current) {
            current++;  
        }
       
    }


    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < simulation_end_time; j++) {
            if (output[j] == processes[i].name) {
                processes[i].finish_time = j + 1;
            }
        }
    }


    for (int i = 0; i < processes.size(); i++) {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].normturn_time = static_cast<float>(processes[i].turnaround_time) / processes[i].service_time;

        if (i > 0) {
            if (processes[i - 1].finish_time > processes[i].arrival_time) {
                processes[i].waiting_time = processes[i - 1].finish_time - processes[i].arrival_time;
            }
        }
    }

    // Print statistics
    if (mode.compare("stats") == 0) {
        printStats(processes, "FB-2i");
    } else {
        
        cout << "FB-2i 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";
        cout << "------------------------------------------------\n";

        for (int i = 0; i < processes.size(); i++) {
            cout << processes[i].name << "     |";
            for (int j = 0; j < simulation_end_time; j++) {
                if (output[j] == processes[i].name) {
                    cout << "*|";
                } else if (j >= processes[i].arrival_time && j < processes[i].finish_time) {
                    cout << ".|"; 
                } else {
                    cout << " |"; 
                }
            }
            cout << " " << endl;
        }

        cout << "------------------------------------------------\n";
        cout << endl;
    }
}


int getMax(vector<Process> processes){
    int max = 0;
    for(int i = 1 ; i < processes.size() ; i++){
    if(processes[i].isWaiting){
        if(processes[i].p > processes[max].p){
            max = i;
        }
         else if(processes[i].p == processes[max].p  && processes[i].waiting_time > processes[max].waiting_time){
         max = i;
        }
    }
      
    }
    return max;
}
void setPer(vector<Process>& processes){

    for(int i =1 ; i < processes.size() ; i++){
      if(processes[i].isWaiting && processes[i].serv != 1){
          processes[i].p++; 
          processes[i].waiting_time++;
      }else{
      processes[i].serv = 0;
      }
    }
    
   

}
void setIsWaiting(vector<Process>& p,int current){
      for(int i = 1 ; i <p.size() ; i++){
            if(p[i].arrival_time <= current && !p[i].isWaiting){
                p[i].isWaiting = true;
            }
      }
}


void simulateAging(vector<Process>& processes, int simulation_end_time) {
    vector<char> output(simulation_end_time, '.');
    int current = 0; 
    int i = 0;
    char names[processes.size()];  
    setServ(processes);
    for (int j = 0; j < processes.size(); j++) {
        names[j] = processes[j].name;  
    }
    while(current < simulation_end_time){  
        setIsWaiting(processes,current); 
        int max = getMax(processes);
        if(max == i){  
            processes[i].p = processes[i].periority;
            processes[i].isWaiting = false;
            processes[i].waiting_time = 0;
            setPer(processes);
            max = getMax(processes);
            if(max == i)
            output[current] = processes[i].name;
            else{
               processes[i].serv = 1;
            Process p2 =processes.front();
            processes.erase(processes.begin());
            processes.push_back(p2);
            processes[processes.size()-1].isWaiting = true;
            int y = 0;
           
            while (y < max-1){
            p2 =processes.front();
            processes.erase(processes.begin());
            processes.push_back(p2);
            y++;
            
          }               

            processes[i].isWaiting = false;
            processes[i].p = processes[i].periority;
            output[current] = processes[i].name;
            processes[i].waiting_time = 0;
            setPer(processes);
            }
        }
        else if (max != i){ 
            processes[i].serv = 1;
            Process p2 =processes.front();
            processes.erase(processes.begin());
            processes.push_back(p2);
            processes[processes.size()-1].isWaiting = true;
            int y = 0;
           
            while (y < max-1){
            p2 =processes.front();
            processes.erase(processes.begin());
            processes.push_back(p2);
            y++;
            
          }               

            processes[i].isWaiting = false;
            processes[i].p = processes[i].periority;
            processes[i].waiting_time = 0;
            output[current] = processes[i].name; 
            setPer(processes); 
            }
       current++; 
    }
    cout << "Aging 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 \n";
    cout << "------------------------------------------------\n";
     const int rows = processes.size();
    for (i = 0; i < rows; i++) {
        char rowLabel = names[i];
        cout << rowLabel << "     |";
        int x = 0;
        for (int j = 0; j < simulation_end_time; j++) {
        for(int k = 0 ; k < processes.size() ; k++){
            if(rowLabel == processes[k].name){
            x=k;
            }
        }
          if(j < processes[x].arrival_time){
              cout << " |"; 
            }
            else if (output[j] == rowLabel) { 
               
                cout << "*|"; 
            } 
            else {
                cout << ".|"; 
            } }
            cout << " ";
            cout << endl;
        }
        
    cout << "------------------------------------------------\n";
    cout << endl;
    
}

int main() {
     std::string line;
    std::string mode; // For "trace" or "stats"
    int simulation_end_time;
    int num_processes;
    std::vector<Process> processes;

    // Line 1: "trace" or "stats"
    std::getline(std::cin, mode);
    std::string algorithm;
    std::getline(std::cin,algorithm);
 

   // Line 3: Simulation end time
    std::cin >> simulation_end_time;

    // Line 4: Number of processes
    std::cin >> num_processes;
std::getline(std::cin,line);
    // Process details (lines 5 onwards)
    for (int i = 0; i < num_processes; ++i) {
        Process p;
        std::getline(std::cin,line);
        
        p.name=line[0];
        p.arrival_time=line[2]-48;
          if(algorithm[0]-48 == 8){
           p.periority = line[4] - 48;
           p.p = p.periority;
    } else {p.service_time=line[4]-48;}
         processes.push_back(p);
    }
for(int i = 0 ; i < algorithm.length() ; i+=2){
    if (algorithm[i]-48 == 1) {
        simulateFCFS(processes, mode);

    } else if (algorithm[i]-48 == 2) {
    i = i+2;
        simulateRR(processes, mode,simulation_end_time,algorithm[i]-48);

    } else if (algorithm[i]-48 == 3) {

        simulateSPN(processes, mode);

    } else if (algorithm[i]-48 == 4) {

        simulateSRT(processes, mode,simulation_end_time);

    } else if (algorithm[i]-48 == 5) {

        simulateHRRN(processes, mode);

    } else if (algorithm[i]-48 == 6) {

        simulateFB1(processes, mode,simulation_end_time);

    } else if (algorithm[i]-48 == 7) {

        simulateFB2i(processes, mode,simulation_end_time);

    } else if (algorithm[i]-48 == 8) {
     i = i+2;
        simulateAging(processes, simulation_end_time);

    } else {

        cerr << "Error: Invalid algorithm selection." << endl;

    }
    }
    return 0;
} 
