#ifndef NEURALNETWORKTRY_SIMPLENETWORK_H
#define NEURALNETWORKTRY_SIMPLENETWORK_H

#include "math/vec.h"
#include "math/neuron_functions.h"
#include "math/random_element.h"

struct SimpleForwardPropagationResult;
struct SimpleBackwardPropagationResult;
enum SimpleNetworkFillingType : int;

class SimpleNetwork : stringable, RandomElement<>
{
private:
	size_t max_layer_size;
	size_t layers_num;
	size_t input_layer_size;
	size_t output_layer_size;

	NeuronFunction neuron_function;
	double (*func)(double);
	double (*dfunc)(double);

	void fill_weights(SimpleNetworkFillingType filling_type);
	
public:
	vec<vec<vec<double>>> Weights;
	/// <summary>
	/// BasisWeights[weight_layer][to_neuron]
	/// </summary>
	vec<vec<double>> BasisWeights;
	vec<size_t> Structure;

	size_t GetInputLayerSize() const;
	size_t GetOutputLayerSize() const;

	SimpleNetwork();
	SimpleNetwork(NeuronFunction function,
	    std::initializer_list<size_t> structure,
		SimpleNetworkFillingType filling_type);
	SimpleNetwork(NeuronFunction function,
		std::initializer_list<size_t> structure,
		const vec<vec<vec<double>>>& weights,
		const vec<vec<double>>& basis_weights);
	SimpleNetwork(std::string& file_location, bool is_in_binary);
	SimpleNetwork(const char* file_location, bool is_in_binary);
	SimpleNetwork(const SimpleNetwork& to_copy);

	SimpleForwardPropagationResult ForwardPropagation(
		const vec<double>& input_data) const;
	vec<double> ComputeNetworkAnswer(
		const vec<double>& input_data) const;
	
	SimpleBackwardPropagationResult BackwardPropagation(
		const SimpleForwardPropagationResult& res, 
		const vec<double>& desired_output) const;

	void SaveToFile(std::string& file_location, 
		bool write_in_binary) const;
	void SaveToFile(const char* file_location,
		bool write_in_binary) const;
};

struct SimpleForwardPropagationResult
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

	/// <summary>
	/// dC_dbw for each basis weight
	/// </summary>
	vec<vec<double>> dC_dbw;
	
	double error;
};

enum SimpleNetworkFillingType
{
	/// <summary>
	/// Sets all weights to value of zero
	/// </summary>
	Zeros,
	/// <summary>
	/// Sets each weight value to random double from -1 to 1
	/// </summary>
	RndNonNormalized,
	/// <summary>
	/// Sets each weight value to random double from -1 to 1, divided by previous layer size.
	/// </summary>
	RndNormalised
};

#endif //NEURALNETWORKTRY_SIMPLENETWORK_H