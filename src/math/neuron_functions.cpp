#include "neuron_functions.h"
#include <cmath>

double __NeuronFunctionsRealization::Sigmoid(double x)
	{ return 1.0 / (1.0 + exp(-x)); }
double __NeuronFunctionsRealization::dSigmoid(double x)
{
	double buff = Sigmoid(x);
	return buff * (1 - buff);
}

double(* __NeuronFunctionsRealization::getFunction(
	NeuronFunction neuron_function))(double)
{
	switch (neuron_function)
	{
		case NeuronFunction::Sigmoid:
			return Sigmoid;
		default:
			return nullptr;
	}
}

double(* __NeuronFunctionsRealization::getDFunction(
	NeuronFunction neuron_function))(double)
{
	switch (neuron_function)
	{
		case NeuronFunction::Sigmoid:
			return dSigmoid;
		default:
			return nullptr;
	}
}
