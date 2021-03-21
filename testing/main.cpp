#include <iostream>
#include "math/vec.h"
#include "neural_core/simple_network/simple_network.h"

using namespace std;

int main(){
	SimpleNetwork network(
		NeuronFunctions::Sigmoid, 
		{ 2, 2, 1 });
	SimplePropagationResult res = 
		network.ForwardPropagation(vec<double>({ 1, 1 }));
	vec<double> output(1);
	output[0] = 0;
	SimpleBackwardPropagationResult b =
		network.BackwardPropagation(res, output);
	cout<<b.dC_dw;
    return 0;
}