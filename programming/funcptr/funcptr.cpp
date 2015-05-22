#include<iostream>
#include<functional>

class FuncPtr {
	public:
		FuncPtr(int setbase) : base(setbase) {}

		void set_mult() { func_ptr = [&](int x){ return x * base; }; }
		void set_sum() { func_ptr = [&](int x){ return x + base; }; }

		void print_func(int x) {
			std::cout << func_ptr(x) << std::endl;
		}

	private:
		int base;
	
		std::function<int(int)> func_ptr;
};

int main() {
	FuncPtr obj(20);
	obj.set_mult();
	obj.print_func(5);
	obj.set_sum();
	obj.print_func(30);
	return 0;
}
