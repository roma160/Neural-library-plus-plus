#include "neuron_functions.h"

double __NeuronFunctionsRealization::Sigmoid(double x)
	{ return 1.0 / (1.0 + exp(-x)); }
double __NeuronFunctionsRealization::dSigmoid(double x)
{
	double buff = Sigmoid(x);
	return buff * (1 - buff);
}
