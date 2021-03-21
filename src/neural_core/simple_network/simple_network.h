#ifndef NEURALNETWORKTRY_SIMPLENETWORK_H
#define NEURALNETWORKTRY_SIMPLENETWORK_H

#include "math/vec.h"
#include "math/neuron_functions.h"

struct SimplePropagationResult;
struct SimpleBackwardPropagationResult;

class SimpleNetwork : stringable
{
private:
	vec<vec<vec<double>>> w;
	vec<int> structure;
	int max_layer_size;
	int layers_num;

	double (*func)(double);
	double (*dfunc)(double);
	
public:
	SimpleNetwork(NeuronFunction function,
		std::initializer_list<int> structure);
	SimpleNetwork(NeuronFunction function,
		std::initializer_list<int> structure,
		const vec<vec<vec<double>>>& weights);
	SimpleNetwork(const SimpleNetwork& to_copy);

	SimplePropagationResult ForwardPropagation(
		const vec<double>& input_data) const;

	SimpleBackwardPropagationResult BackwardPropagation(
		const SimplePropagationResult& res, 
		const vec<double>& desired_output) const;
};

struct SimplePropagationResult
{
	/// <summary>
	/// Network, which result the structure stores
	/// </summary>
	SimpleNetwork* network;

	/// <summary>
	/// Input, that each neuron gets \n
	///		z[layer_num][neuron_index] = sum(...)
	/// </summary>
	vec<vec<double>> z;

	/// <summary>
	/// Output, that each neuron sends \n
	///		a[layer_num][neuron_index] =
	///		neuron_func(a)
	/// </summary>
	vec<vec<double>> a;
};

struct SimpleBackwardPropagationResult
{
	/// <summary>
	/// Network, which result the structure stores
	/// </summary>
	SimpleNetwork* network;

	/// <summary>
	/// dC_dw for each weight
	/// </summary>
	vec<vec<vec<double>>> dC_dw;
	
	double error;
};

#endif //NEURALNETWORKTRY_SIMPLENETWORK_H