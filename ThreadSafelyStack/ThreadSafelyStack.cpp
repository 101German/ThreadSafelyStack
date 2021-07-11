

#include <iostream>
#include <stack>
#include <thread>
#include<mutex>
#include <exception>




struct empty_stack:std::exception
{
	const char* what() const throw();

};

template<typename T>
class ThreadSafelyStack
{
public:
	
	ThreadSafelyStack(){}
	

	ThreadSafelyStack(const ThreadSafelyStack& other)
	{
		std::lock_guard<std::mutex> lock(other.mtx);
		stack = other.stack;
	}
	void Push(T value)
	{
		std::lock_guard<std::mutex> lock(mtx);
		stack.push(std::move(value));
	}

	std::shared_ptr<T> pop() 
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (stack.empty())
			throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(std::move(stack.top())));
		stack.pop();
		return res;
	}

	bool empty()const 
	{
		std::lock_guard<std::mutex> lock(mtx);
		return stack.empty();
	}

private:
	std::stack<T> stack;
	mutable std::mutex mtx;
};



int main()
{
	
}

const char* empty_stack::what() const throw()
{
	return "stack is empty";
}
