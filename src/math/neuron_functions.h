#ifndef NEURALNETWORKTRY_NEURONFUNCS_H
#define NEURALNETWORKTRY_NEURONFUNCS_H

enum NeuronFunction : int
{
	Sigmoid
};

namespace __NeuronFunctionsRealization
{
	double Sigmoid(double x);
	double dSigmoid(double x);

	double (*getFunction(NeuronFunction neuron_function))(double);
	double (*getDFunction(NeuronFunction neuron_function))(double);
}

#endif //NEURALNETWORKTRY_NEURONFUNCS_H