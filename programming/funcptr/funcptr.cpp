#include<iostream>

class FuncPtr {
	public:
		FuncPtr(int setbase) : base(setbase) {}

		void set_mult() { func_ptr = &FuncPtr::prod; }
		void set_sum() { func_ptr = &FuncPtr::add; }

		void print_func(int x) {
			std::cout << (this->*func_ptr)(x) << std::endl;
		}

	private:
		int base;
	
		int (FuncPtr::*func_ptr)(int);

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
