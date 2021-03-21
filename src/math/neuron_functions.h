#ifndef NEURALNETWORKTRY_NEURONFUNCS_H
#define NEURALNETWORKTRY_NEURONFUNCS_H

#include <cmath>

namespace __NeuronFunctionsRealization
{
	double Sigmoid(double x);
	double dSigmoid(double x);
}

struct NeuronFunction
{
	double (*Function)(double);
	double (*dFunction)(double);
};

namespace NeuronFunctions
{
	const NeuronFunction Sigmoid{
		__NeuronFunctionsRealization::Sigmoid,
		__NeuronFunctionsRealization::dSigmoid
	};
}

#endif //NEURALNETWORKTRY_NEURONFUNCS_H