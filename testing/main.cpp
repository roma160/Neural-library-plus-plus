#include <iostream>
#include "math/vec.h"
#include "neural_core/simple_network/simple_network.h"

using namespace std;

SimpleNetwork network(
	Sigmoid, { 2, 2, 1 });
double input_data[4][2] = {
	{0, 0}, {1, 0},
	{0, 1}, {1, 1}
};
double output_data[4][1] = {
	{0}, {1}, {1}, {0}
};
double c = 1;
int iters_num = 100;

double train_iter()
{
	double error_av = 0;
	for(int i = 0; i < iters_num; i++)
	{
		int test = rand() % 4;
		SimplePropagationResult forward = network.ForwardPropagation(
			{ 2, input_data[test] });
		SimpleBackwardPropagationResult backward =
			network.BackwardPropagation(forward, 
				{ 1, output_data[test] });
		error_av += backward.error;
		network.Weights += backward.dC_dw * c;
	}
	return error_av / iters_num;
}

int main(){
	/*vec<vec<vec<double>>> best_weights;
	double min_err = DBL_MAX, buff;
	for (int i = 0; i < 100; i++)
	{
		buff = train_iter();
		if(min_err > buff)
		{
			min_err = buff;
			best_weights = network.Weights;
			cout << "New min error : " << min_err << "\n";
		}
	}

	network.Weights = best_weights;
	for(int i = 0; i < 4; i++)
	{
		vec<double> input = { 2, input_data[i] };
		cout << input << " - ";
		SimplePropagationResult res = network.ForwardPropagation(input);
		cout << res.a[2] << "\n";
	}
	cout << best_weights;*/
	//network.SaveToFile("D:\\buff\\test", true);

	SimpleNetwork test_read("D:\\buff\\test", true);
	cout << test_read.Weights<<"\n";
	for (int i = 0; i < 4; i++)
	{
		vec<double> input = { 2, input_data[i] };
		cout << input << " - ";
		SimplePropagationResult res = test_read.ForwardPropagation(input);
		cout << res.a[2] << "\n";
	}
	
    return 0;
}