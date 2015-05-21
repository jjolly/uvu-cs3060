#include<iostream>
#include<functional>

class FuncPtr {
	public:
		FuncPtr(int setbase) : base(setbase) {}

		void set_mult() { func_ptr = std::bind(&FuncPtr::prod, this, std::placeholders::_1); }
		void set_sum() { func_ptr = std::bind(&FuncPtr::add, this, std::placeholders::_1); }

		void print_func(int x) {
			std::cout << func_ptr(x) << std::endl;
		}

	private:
		int base;
	
		std::function<int(int)> func_ptr;

		int add(int x) {
			return x + base;
		}

		int prod(int x) {
			return x * base;
		}
};

int main() {
	FuncPtr obj(20);
	obj.set_mult();
	obj.print_func(5);
	obj.set_sum();
	obj.print_func(30);
	return 0;
}
