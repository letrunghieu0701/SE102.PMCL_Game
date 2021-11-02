#include <iostream>

using namespace std;

void print(int *x)
{
	cout << "Dia chi cua bien trong ham: " << &x << endl;
}

void main()
{
	int a = 72;
	cout << "Dia chi cua bien a: " << &a << endl;
	
	print(&a);
}