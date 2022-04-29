#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum Status { X, DONE };

struct Task
{
    unsigned number;
    std::string start_time;
    std::string end_time;
    Status state;
    std::string task;
};

std::vector<std::string> read_strings(std::string filename);
void print_strings(std::vector<std::string> strings);
Task single_string_to_task(std::string str);
std::vector<Task> from_strings_to_tasks(std::vector<std::string> strings);
std::string single_task_to_string(Task  task);
std::vector<std::string> from_tasks_to_strings(std::vector<Task> tasks);
void write_on_file(std::vector<std::string> strings, std::string filename);

unsigned get_decimal(Task task);
void sort(std::vector<Task>& tasks);

bool do_something(std::vector<Task>& tasks);
void add_task(std::vector<Task>& tasks);
void remove_task(std::vector<Task>& tasks);
void task_done(std::vector<Task>& tasks);
void task_undone(std::vector<Task>& tasks);

int main()
{
    std::cout << "TODO LIST" << std::endl << std::endl;
    const std::string filename = "daily.todo";
    bool iterate = true;

    while(iterate)
    {
        std::vector<std::string> strings = read_strings(filename);
        std::vector<Task> tasks = from_strings_to_tasks(strings);
        sort(tasks);
        strings = from_tasks_to_strings(tasks);
        print_strings(strings);
        iterate = do_something(tasks);
        strings = from_tasks_to_strings(tasks);
        write_on_file(strings, filename);
    }
}

// this function takes a filename
// and returns a vector of strings
// each line in the source code 
// is a member of the vector.
std::vector<std::string> read_strings(std::string filename)
{
    std::ifstream fin;
    fin.open(filename);
    if(!fin.is_open())
    {
        std::ofstream fout;
        fout.open(filename);
        Task task;
        task.number = 0;
        task.start_time = "00:00";
        task.end_time = "00:00";
        task.task = "do something";
        task.state = X;
        fout << single_task_to_string(task) << '\n';
        fout.close();
        fin.open(filename);
    }
    std::vector<std::string> result;
    std::string command;
    while(!fin.eof())
    {
        std::getline(fin, command);
        result.push_back(command);
    }
    result.pop_back();
    return result;
}

// this function prints the tasks
// in forms of strings on console
void print_strings(std::vector<std::string> strings)
{
    for(int i{}; i < strings.size(); i++)
    {
        std::cout << strings[i] << std::endl;
    }
    std::cout << std::endl;
}

// this function takes as input a string
// and returns a corresponding task
Task single_string_to_task(std::string str)
{
    Task temp_Task;
    if(str[0] = 0x20) temp_Task.number = str[1] - 0x30;
    else
    {
        temp_Task.number = 10 * (str[0] - 0x30) + str[1] - 0x30;
    }
    temp_Task.start_time = str.substr(5,5);
    temp_Task.end_time = str.substr(13,5);
    std::string temp = str.substr(21,30);
    for(int i = temp.size() - 1; temp[i] == '-'; i--)
    {
        temp.pop_back();
    }
    temp.pop_back();
    temp_Task.task = temp;
    temp_Task.state = str[52] == 'X' ? X : DONE;
    return temp_Task;
}

// this function takes the vector of 
// strings as input and returns a vector
// of objects of type Task
std::vector<Task> from_strings_to_tasks(std::vector<std::string> strings)
{
    std::vector<Task> tasks;
    for(int i{}; i < strings.size(); i++)
    {
        tasks.push_back(single_string_to_task(strings[i]));
    }
    return tasks;
}

// this function takes as input an object
// of type Task and returns a single-line
// string corresponding to it
std::string single_task_to_string(Task  task)
{
    std::string result;
    if(task.number > 9)
    {
        result.push_back(task.number / 10 + 0x30);
        result.push_back(task.number % 10 + 0x30);
    }
    else
    {
        result.push_back(0x20);
        result.push_back(task.number + 0x30);
    }
    result += " | ";
    result += task.start_time;
    result += " - ";
    result += task.end_time;
    result += " | ";
    result += task.task;
    result += " ";
    std::string empty_space;
    for(int i{}; i < 30 - task.task.size() - 1; i++)
    {
        empty_space.push_back('-');
    }
    result += empty_space;
    result += " ";
    if(task.state) result += "DONE";
    else result += "X";
    return result;
}

// this functions takes as input a new
// vector of tasks (after some changes)
// and returns a vector of strings 
// correspondingly
std::vector<std::string> from_tasks_to_strings(std::vector<Task> new_tasks)
{
    std::vector<std::string> new_strings;
    std::string temp;
    for(int i{}; i < new_tasks.size(); i++)
    {
        temp = single_task_to_string(new_tasks[i]);
        new_strings.push_back(temp);   
    }
    return new_strings;
}

// this function outputs the vector
// of strings on the file we pass as
// a parameter
void write_on_file(std::vector<std::string> strings, std::string filename)
{
    std::ofstream fout;
    fout.open(filename);
    if(fout.is_open())
    {
        for(int i{}; i < strings.size(); i++)
        {
            fout << strings[i] << '\n';
        }
    }
}

// this function changes the order of tasks
// according to their starting times
void sort(std::vector<Task>& tasks)
{
    unsigned min{};
    for(int i{}; i < tasks.size() - 1; i++)
    {
        min = i;
        for(int j = i + 1; j < tasks.size(); j++)
        {
            if(get_decimal(tasks[j]) < get_decimal(tasks[min]))
            {
                min = j;
            }
        }
        std::swap(tasks[i],  tasks[min]);
    }
    for(int  i{}; i < tasks.size(); i++)
    {
        tasks[i].number = i + 1;
    }
}

// this function returns the decimal
// appearance of a Task object's starting time
unsigned get_decimal(Task task)
{
    unsigned result{};
    result += (task.start_time[0] + 0x30) * 1000;
    result += (task.start_time[1] + 0x30) * 100;
    result += (task.start_time[3] + 0x30) * 10;
    result += (task.start_time[4] + 0x30);
    return result;
}

// this function takes control from main
// and profides interface for the user to
// do some operations /add/remove/mark as done/ mark as undone/
// and then returns control to main
bool do_something(std::vector<Task>& tasks)
{
    std::cout << "TO MAKE CHANGES PRESS" << std::endl << std::endl;
    std::cout << "a : to add a task" << std::endl;
    std::cout << "r : to remove a task" << std::endl;
    std::cout << "d : to mark a task as done" << std::endl;
    std::cout << "u : to mark a task as undone" << std::endl;
    std::cout << "q : to quit" << "   ";

    char op{};
    std:: cin >> op;
    std::cout << std::endl;
    switch(op)
    {
        case 'a':
        case 'A': add_task(tasks); break;
        case 'r':
        case 'R': remove_task(tasks); break;
        case 'd':
        case 'D': task_done(tasks); break;
        case 'u': 
        case 'U': task_undone(tasks); break;
        case 'q':
        case 'Q': return false;
    }
    return true;
}

// this function takes control from do_something
// makes a task from user input
// and adds the task to the vector of tasks
void add_task(std::vector<Task>& tasks)
{
    Task task;
    task.number = 0;
    std::cout << "ENTER THE INFORMATION ABOUT THE TASK\n" << std::endl;
    std::cout << "Time template : HH:MM " << std::endl;
    std::cout << "State template : done or undone " << std::endl;
    std::cout << "Starting time : ";
    std::cin >> task.start_time;
    std::cout << "Ending time : ";
    std::cin >> task.end_time;
    std::cout << "Task : ";
    std::cin.get();
    std::getline(std::cin, task.task);
    std::cout << "State : ";
    std::string state;
    std::cin >> state;
    if(state == "done") task.state = DONE;
    else task.state = X;
    std::cout << std::endl;

    tasks.push_back(task);
    sort(tasks);
}

// this function takes control from do_something
// then it asks the user to input the number of 
// the task to remove and then removees
void remove_task(std::vector<Task>& tasks)
{
    std::cout << "ENTER THE NUMBER : ";
    unsigned num{};
    std::cin >> num;
    if(num <= tasks.size())
    {
        tasks.erase(tasks.begin() + num - 1);
    }
    sort(tasks);
    std::cout << std::endl;
}

// this function takes control from do_something
// asks the user to input a number and then
// marks the task under that number as done
void task_done(std::vector<Task>& tasks)
{
    std::cout << "ENTER THE NUMBER : ";
    unsigned num{};
    std::cin >> num;
    tasks[num - 1].state = DONE;
    std::cout << std::endl;
}

// this function takes control from do_something
// asks the user to input a number and then
// marks the task under that number as undone
void task_undone(std::vector<Task>& tasks)
{
    std::cout << "ENTER THE NUMBER : ";
    unsigned num{};
    std::cin >> num;
    tasks[num - 1].state = X;
    std::cout << std::endl;
}
