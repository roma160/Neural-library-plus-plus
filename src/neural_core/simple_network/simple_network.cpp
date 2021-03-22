#include <neural_core/simple_network/simple_network.h>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

SimpleNetwork::SimpleNetwork() {}

SimpleNetwork::SimpleNetwork(NeuronFunction function, 
                             std::initializer_list<size_t> structure)
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
	new (&Structure) vec<size_t>(structure);
	input_layer_size = Structure[0];
	output_layer_size = Structure[layers_num - 1];
	max_layer_size = Structure.max_element();

#ifdef _DEBUG
	srand(0);
#endif

	new (&Weights) vec<vec<vec<double>>>(layers_num - 1, false);
	for (int wl = 0; wl < layers_num - 1; wl++) {
#ifdef _DEBUG
		//Errors handling
		if (Structure[wl] == 0) {
			std::cerr << "Invalid layer size (the layer must have at"
				" least 1 neuron)";
			throw new std::range_error(
				"Invalid layer size (the layer must have at"
				" least 1 neuron)");
		}
#endif
		new (&Weights[wl]) vec<vec<double>>(Structure[wl], false);
		for (int f = 0; f < Structure[wl]; f++)
		{
			new (&Weights[wl][f]) vec<double>(Structure[wl + 1], false);
			for (int t = 0; t < Structure[wl + 1]; t++)
				Weights[wl][f][t] = (double)rand() / RAND_MAX;
		}
	}

	new (&BasisWeights) vec<vec<double>>(layers_num - 1, false);
	for (int wl = 0; wl < layers_num - 1; wl++) {
		new (&BasisWeights[wl]) vec<double>(Structure[wl + 1], false);
		for (int f = 0; f < Structure[wl + 1]; f++)
			BasisWeights[wl][f] = (double)rand() / RAND_MAX;
	}
}

SimpleNetwork::SimpleNetwork(std::string& file_location, bool is_in_binary)
{
	std::ifstream file;
	if (is_in_binary)
		new (&file) std::ifstream(file_location,
			std::ios::binary | std::ios::in);
	else new (&file) std::ifstream(file_location,
		std::ios::in);
	if (!file)
	{
		std::stringstream ss;
		ss << "Was unable to open the file! ("
			<< file_location << ")";
		std::cerr << ss.str();
		throw new std::ios_base::failure(ss.str());
	}

	//Firstly reading number of the layers
	if (is_in_binary)
		file.read((char*)&layers_num, sizeof(size_t));
	else file >> layers_num;
	
	new (&Structure) vec<size_t>(layers_num, false);
	//Reading number of neurons in each layer
	if (is_in_binary)
		for (int l = 0; l < layers_num; l++)
			file.read((char*)&Structure[l], sizeof(size_t));
	else for (int l = 0; l < layers_num; l++)
		file >> Structure[l];
	input_layer_size = Structure[0];
	output_layer_size = Structure[layers_num - 1];
	max_layer_size = Structure.max_element();

	//Readinf neuron function name
	if (is_in_binary)
		file.read((char*)&neuron_function, sizeof(NeuronFunction));
	else file >> (int&) neuron_function;
	func = __NeuronFunctionsRealization::getFunction(neuron_function);
	dfunc = __NeuronFunctionsRealization::getDFunction(neuron_function);

	//Reading the weights data
	new (&Weights) vec<vec<vec<double>>>(layers_num - 1, false);
	new (&BasisWeights) vec<vec<double>>(layers_num - 1, false);
	if (is_in_binary) {
		for (int wl = 0; wl < layers_num - 1; wl++) {
			new (&Weights[wl]) vec<vec<double>>(Structure[wl], false);
			for (int f = 0; f < Structure[wl]; f++) {
				new (&Weights[wl][f]) vec<double>(Structure[wl + 1], false);
				for (int t = 0; t < Structure[wl + 1]; t++)
					file.read((char*)&Weights[wl][f][t], sizeof(double));
			}
		}
		for (int wl = 0; wl < layers_num - 1; wl++) {
			new (&BasisWeights[wl]) vec<double>(Structure[wl + 1], false);
			for (int f = 0; f < Structure[wl + 1]; f++)
				file.read((char*)&BasisWeights[wl][f], sizeof(double));
		}
	}
	else {
		for (int wl = 0; wl < layers_num - 1; wl++) {
			new (&Weights[wl]) vec<vec<double>>(Structure[wl], false);
			for (int f = 0; f < Structure[wl]; f++) {
				new (&Weights[wl][f]) vec<double>(Structure[wl + 1], false);
				for (int t = 0; t < Structure[wl + 1]; t++)
					file >> Weights[wl][f][t];
			}
		}
		for (int wl = 0; wl < layers_num - 1; wl++) {
			new (&BasisWeights[wl]) vec<double>(Structure[wl + 1], false);
			for (int f = 0; f < Structure[wl + 1]; f++)
				file >> BasisWeights[wl][f];
		}
	}
	file.close();
}

SimpleNetwork::SimpleNetwork(const char* file_location, bool is_in_binary) :
	SimpleNetwork(std::string(file_location), is_in_binary) { }

SimpleNetwork::SimpleNetwork(const SimpleNetwork& to_copy)
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

SimplePropagationResult SimpleNetwork::ForwardPropagation(
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
	SimplePropagationResult ret{ (SimpleNetwork*) this };
	new (&ret.z) vec<vec<double>>(layers_num, false);
	new (&ret.a) vec<vec<double>>(layers_num, false);
	new (&ret.z[0]) vec<double>(input_data);
	new (&ret.a[0]) vec<double>(input_data);

	//The propagation
	size_t from_layer_size, to_layer_size = input_layer_size;
	for(size_t wl = 0; wl < layers_num - 1; wl++)
	{
		from_layer_size = to_layer_size;
		to_layer_size = Structure[wl + 1];
		new (&ret.z[wl + 1]) vec<double>(to_layer_size, false);
		new (&ret.a[wl + 1]) vec<double>(to_layer_size, false);
		ret.z[wl + 1] = Weights[wl][0] * ret.a[wl][0];
		for (int from = 1; from < from_layer_size; from++)
			ret.z[wl + 1] += Weights[wl][from] * ret.a[wl][from];
		for (int to = 0; to < to_layer_size; to++)
			ret.a[wl + 1][to] = func(
				ret.z[wl + 1][to] + BasisWeights[wl][to]);
	}
	return ret;
}

SimpleBackwardPropagationResult SimpleNetwork::BackwardPropagation(
	const SimplePropagationResult& res,
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
	SimpleBackwardPropagationResult ret;
	vec<vec<double>> dC_da(layers_num, false);
	new (&ret.dC_dw) vec<vec<vec<double>>>(layers_num - 1, false);
	new (&ret.dC_dbw) vec<vec<double>>(layers_num - 1, false);
	dC_da[layers_num - 1] = (desired_output - res.a[layers_num - 1]) * 2.0;
	ret.error = 0;
	for (int i = 0; i < Structure[layers_num - 1]; i++)
		ret.error += pow(desired_output[i] - res.a[layers_num - 1][i], 2);
	ret.error /= Structure[layers_num - 1];

	//The propagation
	vec<double> buff_dC_dz(max_layer_size, false);
	int from_layer_size = Structure[layers_num - 1], to_layer_size;
	for(int wl = layers_num - 2; wl >= 0; wl--)
	{
		to_layer_size = from_layer_size;
		from_layer_size = Structure[wl];
		buff_dC_dz.resize(to_layer_size);
		for (int to = 0; to < to_layer_size; to++)
			buff_dC_dz[to] = dC_da[wl + 1][to] * dfunc(res.z[wl + 1][to]);
		ret.dC_dbw[wl] = buff_dC_dz;
		ret.dC_dw[wl] = res.a[wl] & buff_dC_dz;
		new (&dC_da[wl]) vec<double>(from_layer_size, false);
		for (int from = 0; from < from_layer_size; from++)
			dC_da[wl][from] = (Weights[wl][from] * buff_dC_dz).sum();
	}
	return ret;
}

void SimpleNetwork::SaveToFile(std::string& file_location, 
	bool write_in_binary) const
{
	std::ofstream file;
	if(write_in_binary)
		new (&file) std::ofstream(file_location,
			std::ios::binary | std::ios::out);
	else new (&file) std::ofstream(file_location, 
		std::ios::out);
	if (!file)
	{
		std::stringstream ss;
		ss << "Was unable to write the file! ("
			<< file_location << ")";
		std::cerr << ss.str();
		throw new std::ios_base::failure(ss.str());
	}

	if (write_in_binary) {
		//Firstly writing number of the layers
		file.write((char*)&layers_num, sizeof(size_t));
		//Writing number of neurons in each layer
		for (int l = 0; l < layers_num; l++)
			file.write((char*)&Structure[l], sizeof(size_t));
		//Neuron function name
		file.write((char*)&neuron_function, sizeof(NeuronFunction));
		//Writing the weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl]; f++)
				for (int t = 0; t < Structure[wl + 1]; t++)
					file.write((char*)&Weights[wl][f][t], sizeof(double));
		//Writing basis weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl + 1]; f++)
				file.write((char*)&BasisWeights[wl][f], sizeof(double));
	}
	else
	{
		//Firstly writing number of the layers
		file << layers_num << " ";
		//Writing number of neurons in each layer
		for (int l = 0; l < layers_num; l++)
			file << Structure[l] << " ";
		//Neuron function name
		file << neuron_function << " ";
		//Writing the weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl]; f++)
				for (int t = 0; t < Structure[wl + 1]; t++)
					file << Weights[wl][f][t] << " ";
		//Writing basis weights data
		for (int wl = 0; wl < layers_num - 1; wl++)
			for (int f = 0; f < Structure[wl + 1]; f++)
				file << BasisWeights[wl][f] << " ";
	}
	file.close();
}

void SimpleNetwork::SaveToFile(const char* file_location, bool write_in_binary) const
{ SaveToFile(std::string(file_location), write_in_binary); }
