#ifndef NEURALNETWORKTRY_SIMPLENETWORK_H
#define NEURALNETWORKTRY_SIMPLENETWORK_H

#include "math/vec.h"
#include "math/neuron_functions.h"

struct SimplePropagationResult;
struct SimpleBackwardPropagationResult;

class SimpleNetwork : stringable
{
private:
	size_t max_layer_size;
	size_t layers_num;
	size_t input_layer_size;
	size_t output_layer_size;

	NeuronFunction neuron_function;
	double (*func)(double);
	double (*dfunc)(double);
	
public:
	vec<vec<vec<double>>> Weights;
	vec<size_t> Structure;
	
	SimpleNetwork(NeuronFunction function,
		std::initializer_list<size_t> structure);
	SimpleNetwork(NeuronFunction function,
		std::initializer_list<size_t> structure,
		const vec<vec<vec<double>>>& weights);
	explicit SimpleNetwork(std::string& file_location, bool is_in_binary);
	explicit SimpleNetwork(const char* file_location, bool is_in_binary);
	SimpleNetwork(const SimpleNetwork& to_copy);

	SimplePropagationResult ForwardPropagation(
		const vec<double>& input_data) const;
	SimpleBackwardPropagationResult BackwardPropagation(
		const SimplePropagationResult& res, 
		const vec<double>& desired_output) const;

	void SaveToFile(std::string& file_location, 
		bool write_in_binary) const;
	void SaveToFile(const char* file_location,
		bool write_in_binary) const;
};

struct SimplePropagationResult
{
	/// <summary>
	/// Network, which result the Structure stores
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
	/// Network, which result the Structure stores
	/// </summary>
	SimpleNetwork* network;

	/// <summary>
	/// dC_dw for each weight
	/// </summary>
	vec<vec<vec<double>>> dC_dw;
	
	double error;
};

#endif //NEURALNETWORKTRY_SIMPLENETWORK_H