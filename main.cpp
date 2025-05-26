#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Task
{
	public:
	std::string _description;
	Task(std::string description){
		_description = description;
	}
};

class TodoList
{
	
	public:
	std::vector<Task> todolist;
	TodoList(){}
};

int main() {

    TodoList todo = TodoList();

    return 0;
}
