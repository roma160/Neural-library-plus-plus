#include <neural_core/simple_network.h>
#include <neural_core/data_saver.h>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
using namespace NNL;

void SimpleNetwork::fill_weights(SimpleNetworkFillingType filling_type)
{
	size_t from_layer_size, to_layer_size = Structure[0];
	switch (filling_type)
	{
	case Zeros:
		Weights = vec<vec<vec<double>>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			from_layer_size = to_layer_size;
			to_layer_size = Structure[wl + 1];

			Weights[wl] = vec<vec<double>>(from_layer_size, false);
			for (size_t f = 0; f < from_layer_size; f++)
				Weights[wl][f] = vec<double>(to_layer_size, true);
		}

		BasisWeights = vec<vec<double>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++)
			BasisWeights[wl] = vec<double>(Structure[wl + 1], true);
		return;

	case RndNonNormalized:
		Weights = vec<vec<vec<double>>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			from_layer_size = to_layer_size;
			to_layer_size = Structure[wl + 1];

			Weights[wl] = vec<vec<double>>(from_layer_size, false);
			for (size_t f = 0; f < from_layer_size; f++)
			{
				Weights[wl][f] = vec<double>(to_layer_size, false);
				for (size_t t = 0; t < to_layer_size; t++)
					Weights[wl][f][t] = r_norm_udbl();
			}
		}

		BasisWeights = vec<vec<double>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			to_layer_size = Structure[wl + 1];
			BasisWeights[wl] = vec<double>(to_layer_size, false);
			for (size_t f = 0; f < to_layer_size; f++)
				BasisWeights[wl][f] = r_norm_udbl();
		}
		return;

	case RndNormalised:
		Weights = vec<vec<vec<double>>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			from_layer_size = to_layer_size;
			to_layer_size = Structure[wl + 1];

			Weights[wl] = vec<vec<double>>(from_layer_size, false);
			for (size_t f = 0; f < from_layer_size; f++)
			{
				Weights[wl][f] = vec<double>(to_layer_size, false);
				for (size_t t = 0; t < to_layer_size; t++)
					Weights[wl][f][t] = r_norm_udbl() / from_layer_size;
			}
		}

		BasisWeights = vec<vec<double>>(layers_num - 1, false);
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			to_layer_size = Structure[wl + 1];
			BasisWeights[wl] = vec<double>(to_layer_size, false);
			for (size_t f = 0; f < to_layer_size; f++)
				BasisWeights[wl][f] = r_norm_udbl() / 2;
		}
	}
}

size_t NNL::SimpleNetwork::GetInputLayerSize() const { return input_layer_size; }

size_t NNL::SimpleNetwork::GetOutputLayerSize() const { return output_layer_size; }

NNL::SimpleNetwork::SimpleNetwork() : RandomElement() { }

NNL::SimpleNetwork::SimpleNetwork(NeuronFunction function,
	initializer_list<size_t> structure,
	SimpleNetworkFillingType filling_type) : SimpleNetwork()
{
	layers_num = structure.size();
#ifdef _DEBUG
	//Errors handling
	if (layers_num < 2) {
		std::cerr << "Too small number of layers (at least 2"
			" needed for input and output layers)!";
		throw new std::range_error(
			"Too small number of layers (at least 2"
			" needed for input and output layers)!");
	}
#endif

	neuron_function = function;
	func = __NeuronFunctionsRealization::getFunction(function);
	dfunc = __NeuronFunctionsRealization::getDFunction(function);
	Structure = vec<size_t>(structure);
	input_layer_size = Structure[0];
	output_layer_size = Structure[layers_num - 1];
	max_layer_size = Structure.max_element();
#ifdef _DEBUG
	//Errors handling
	for(size_t l = 0; l < layers_num; l++)
		if(!Structure[l])
		{
			std::cerr << "Invalid layer size (the layer must have at"
				" least 1 neuron)";
			throw new std::range_error(
				"Invalid layer size (the layer must have at"
				" least 1 neuron)");
		}
#endif

	fill_weights(filling_type);
}

SimpleNetwork::SimpleNetwork(std::string& file_location, bool is_in_binary) :
	SimpleNetwork()
{
	std::ifstream file;
	if (is_in_binary)
		new (&file) std::ifstream(file_location, std::ios::binary | std::ios::in);
	else new (&file) std::ifstream(file_location, std::ios::in);
	if (!file)
	{
		std::stringstream ss;
		ss << "Was unable to open the file! ("
			<< file_location << ")";
		std::cerr << ss.str();
		throw new std::ios_base::failure(ss.str());
	}

	//Reading structure
	if(is_in_binary)
		DataRead::ReadBinaryWithSize(file, &Structure);
	else DataRead::ReadWithSize(file, &Structure);
	layers_num = Structure.size();
	input_layer_size = Structure[0];
	output_layer_size = Structure[layers_num - 1];
	max_layer_size = Structure.max_element();

	//Readinf neuron function name
	if (is_in_binary)
		DataRead::ReadBinary(file, (size_t&) neuron_function);
	else DataRead::Read(file, (size_t&) neuron_function);
	func = __NeuronFunctionsRealization::getFunction(neuron_function);
	dfunc = __NeuronFunctionsRealization::getDFunction(neuron_function);

	//Reading the weights data
	Weights = vec<vec<vec<double>>>(layers_num - 1, false);
	BasisWeights = vec<vec<double>>(layers_num - 1, false);
	if (is_in_binary) {
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			Weights[wl] = vec<vec<double>>(Structure[wl], false);
			for (size_t f = 0; f < Structure[wl]; f++) {
				Weights[wl][f] = vec<double>(Structure[wl + 1], false);
				DataRead::ReadBinary(file, Weights[wl][f]);
			}
		}
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			BasisWeights[wl] = vec<double>(Structure[wl + 1], false);
			DataRead::ReadBinary(file, BasisWeights[wl]);
		}
	}
	else {
		for (size_t wl = 0; wl < layers_num - 1; wl++) {
			Weights[wl] = vec<vec<double>>(Structure[wl], false);
			for (size_t f = 0; f < Structure[wl]; f++) {
				Weights[wl][f] = vec<double>(Structure[wl + 1], false);
				DataRead::Read(file, Weights[wl][f]);
			}
		}
		for (int wl = 0; wl < layers_num - 1; wl++) {
			BasisWeights[wl] = vec<double>(Structure[wl + 1], false);
			DataRead::Read(file, BasisWeights[wl]);
		}
	}
	file.close();
}

SimpleNetwork::SimpleNetwork(const char* file_location, bool is_in_binary) :
	SimpleNetwork((std::string&) file_location, is_in_binary) { }

SimpleNetwork::SimpleNetwork(const SimpleNetwork& to_copy) :
	SimpleNetwork()
{
	max_layer_size = to_copy.max_layer_size;
	layers_num = to_copy.layers_num;
	input_layer_size = to_copy.input_layer_size;
	output_layer_size = to_copy.output_layer_size;

	neuron_function = to_copy.neuron_function;
	func = to_copy.func;
	dfunc = to_copy.dfunc;

	Weights = to_copy.Weights;
	BasisWeights = to_copy.BasisWeights;
	Structure = to_copy.Structure;
}

SimpleForwardPropagationResult SimpleNetwork::ForwardPropagation(
	const vec<double>& input_data) const
{
#ifdef _DEBUG
	//Errors handling
	if (input_data.size() != input_layer_size) {
		std::stringstream ss;
		ss << "Your input data size(" << input_data.size();
		ss << ") doesn\'t equal to the input layer size of the network(";
		ss << input_layer_size << ")!";
		std::cerr << ss.str();
		throw new std::out_of_range(ss.str());
	}
#endif

	//Creating result Structure
	SimpleForwardPropagationResult ret{ (SimpleNetwork*) this };
	ret.z = vec<vec<double>>(layers_num, false);
	ret.a = vec<vec<double>>(layers_num, false);
	ret.z[0] = vec<double>(input_data);
	ret.a[0] = vec<double>(input_data);

	//The propagation
	size_t from_layer_size, to_layer_size = input_layer_size;
	for(size_t wl = 0; wl < layers_num - 1; wl++)
	{
		from_layer_size = to_layer_size;
		to_layer_size = Structure[wl + 1];
		ret.z[wl + 1] = vec<double>(to_layer_size, false);
		ret.a[wl + 1] = vec<double>(to_layer_size, false);
		ret.z[wl + 1] = Weights[wl][0] * ret.a[wl][0];
		for (size_t from = 1; from < from_layer_size; from++)
			ret.z[wl + 1] += Weights[wl][from] * ret.a[wl][from];
		for (size_t to = 0; to < to_layer_size; to++)
			ret.a[wl + 1][to] = func(
				ret.z[wl + 1][to] + BasisWeights[wl][to]);
	}
	return ret;
}

vec<double> SimpleNetwork::ComputeNetworkAnswer(
	const vec<double>& input_data) const
{
	return ForwardPropagation(input_data).a[layers_num - 1];
}

SimpleBackwardPropagationResult SimpleNetwork::BackwardPropagation(
	const SimpleForwardPropagationResult& res,
	const vec<double>& desired_output) const
{
#ifdef _DEBUG
	//Errors handling
	if (desired_output.size() != output_layer_size) {
		std::stringstream ss;
		ss << "Your output data size(" << desired_output.size();
		ss << ") doesn\'t equal to the output layer size of the network(";
		ss << output_layer_size << ")!";
		std::cerr << ss.str();
		throw new std::out_of_range(ss.str());
	}
#endif
	
	//Creating result Structure
	SimpleBackwardPropagationResult ret { (SimpleNetwork*) this };
	vec<vec<double>> dC_da(layers_num, false);
	ret.dC_dw = vec<vec<vec<double>>>(layers_num - 1, false);
	ret.dC_dbw = vec<vec<double>>(layers_num - 1, false);
	dC_da[layers_num - 1] = (desired_output - res.a[layers_num - 1]) * 2.0;
	ret.error = 0;
	for (size_t i = 0; i < output_layer_size; i++)
		ret.error += pow(desired_output[i] - res.a[layers_num - 1][i], 2);
	ret.error /= output_layer_size;

	//The propagation
	vec<double> buff_dC_dz(max_layer_size, false);
	size_t from_layer_size = output_layer_size, to_layer_size;
	size_t wl = layers_num - 1;
	do{
		wl--;
		to_layer_size = from_layer_size;
		from_layer_size = Structure[wl];
		buff_dC_dz.resize(to_layer_size);
		for (size_t to = 0; to < to_layer_size; to++)
			buff_dC_dz[to] = dC_da[wl + 1][to] * dfunc(res.z[wl + 1][to]);
		ret.dC_dbw[wl] = buff_dC_dz;
		ret.dC_dw[wl] = res.a[wl] & buff_dC_dz;
		dC_da[wl] = vec<double>(from_layer_size, false);
		for (size_t from = 0; from < from_layer_size; from++)
			dC_da[wl][from] = (Weights[wl][from] * buff_dC_dz).sum();
	} while (wl != 0);
	return ret;
}

void SimpleNetwork::SaveToFile(std::string& file_location, 
	bool write_in_binary) const
{
	std::ofstream file;
	if(write_in_binary)
		new (&file) std::ofstream(file_location, std::ios::binary | std::ios::out);
	else new (&file) std::ofstream(file_location, std::ios::out);
	if (!file)
	{
		std::stringstream ss;
		ss << "Was unable to write the file! ("
			<< file_location << ")";
		std::cerr << ss.str();
		throw new std::ios_base::failure(ss.str());
	}

	if (write_in_binary) {
		//Saving structure
		DataSave::SaveBinary(file, Structure, true);
		//Neuron function name
		DataSave::SaveBinary(file, (size_t) neuron_function);
		//Writing the weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl]; f++)
				DataSave::SaveBinary(file, Weights[wl][f], false);
		//Writing basis weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			DataSave::SaveBinary(file, BasisWeights[wl], false);
	}
	else
	{
		//Saving structure
		DataSave::Save(file, Structure, true);
		//Neuron function name
		DataSave::Save(file, neuron_function);
		//Writing the weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl]; f++)
				DataSave::Save(file, Weights[wl][f], false);
		//Writing basis weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			DataSave::Save(file, BasisWeights[wl], false);
	}
	file.close();
}

void SimpleNetwork::SaveToFile(const char* file_location, bool write_in_binary) const
{ SaveToFile((std::string&) file_location, write_in_binary); }
