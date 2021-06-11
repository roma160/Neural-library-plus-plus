#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <chrono>
#include "math/vec.h"
#include "neural_core/simple_network.h"
#include "neural_core/trainer/simple_trainer.h"
#include "neural_core/simple_data.h"

using namespace std;

///Tests location : ../../../testing/tests/

typedef chrono::steady_clock::time_point time_point;
time_point get_time_point()
{ return chrono::steady_clock::now(); }
size_t get_millis_delta(time_point begin, time_point end)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); }
size_t get_cur_millis()
{ return std::chrono::duration_cast<std::chrono::milliseconds>(
	chrono::steady_clock::now().time_since_epoch()).count(); }
size_t get_micros_delta(time_point begin, time_point end)
{ return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(); }

class test_copy
{
public:
	int number;

	test_copy(int n)
	{
		number = n;
		cout << number << " constructor called\n";
	}
	test_copy(test_copy& to_copy)
	{
		number = to_copy.number + 1;
		cout << "Copy constructor called on " << number << " from " << to_copy.number<<"\n";
	}
	~test_copy()
	{
		cout << number << " destructor called\n";
	}

	void operator=(test_copy& to_copy)
	{
		cout << "Copy operator called!\n";
	}

	static test_copy& func()
	{
		return test_copy(5);
	}
};

void xor_test()
{
	//SAME_RANDOM is enabled
	
	SimpleNetwork network(Sigmoid, { 2, 2, 1 }, RndNormalised);
	cout << network.Weights << "\n";
	SimpleData train_data("../../../testing/tests/xor/train_data.b", true);
	
	double a = 5, b = 0.2;
	//cin >> a >> b;
	SimpleTrainer trainer(&network, &train_data, a, b, 2);
	trainer.TrainNetwork(1000);

	for (int i = 0; i < 4; i++)
		cout << *train_data.GetInputData(i) << " - " <<
			network.ComputeNetworkAnswer(
				*train_data.GetInputData(i)) << "\n";
}

void mnist_test()
{
	//SAME_RANDOM is disabled
	
	SimpleNetwork network(Sigmoid, { 784, 128, 10 }, RndNormalised);
	SimpleBinStreamData train_data("./res.b");
	SimpleTrainer trainer(&network, &train_data, 1.1, 1, 20);
	trainer.TrainNetwork(5000, &cout, 100);

	for (int i = 0; i < 20; i++)
		cout << train_data.GetOutputData(i)->max_element_ind() << " - " <<
			network.ComputeNetworkAnswer(*train_data.GetInputData(i)) << "\n";

	//network.SaveToFile("D:/buff/mnist_test.b", true);
}

int main(){
	// To check memory leaks
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#endif

	/*vec<vec<vec<int>>> v(1, false);
	v[0] = vec<vec<int>>(2, false);
	v[0][0] = vec<int>(3, false);
	vec<vec<vec<vec<int>>>> shape_copy = vec<vec<vec<vec<int>>>>::create_from_shape(vec<size_t>{2, 2, 3, 4});
	cout << shape_copy;*/
	//time_point begin = get_time_point();
	//mnist_test();
	//xor_test();
	//cout << "\nWorking time : " << get_millis_delta(begin, get_time_point());
	//xor_test();

	/*vec<vec<int>> a = { {1, 2}, {3, 4} };
	cout << "a : " << a<<"\n";
	vec<vec<int>> b = a;
	b[0][0] = b[1][1];
	cout << "a : " << a << "\n";
	cout << "b : " << b << "\n";*/
	
	//Creating mnist network backup :
	SimpleNetwork network(Sigmoid, { 784, 128, 10 }, RndNormalised);
	SimpleBinStreamData train_data("../../../testing/tests/mnist/main_data.b");
	SimpleTrainer trainer(&network, &train_data, 1.1, 1, 20);
	trainer.TrainNetwork(60000, &cout, 100);
	
	SimpleBinStreamData test_data("../../../testing/tests/mnist/testing_data.b");

	for (int i = 0; i < 20; i++)
		cout << test_data.GetOutputData(i)->max_element_ind() << " - " <<
			network.ComputeNetworkAnswer(*test_data.GetInputData(i)) << "\n";
			
	cout<<"Now saving the network...\n";
	network.SaveToFile("../../../testing/tests/mnist/network_backup.b", true);
	cout<<"Now the network is must be saved...";
	return 0;
}