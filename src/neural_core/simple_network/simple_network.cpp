#include <neural_core/simple_network/simple_network.h>
#include <stdexcept>
#include <sstream>

SimpleNetwork::SimpleNetwork(NeuronFunction function, std::initializer_list<int> structure)
{
#ifdef _DEBUG
	//Errors handling
	if (structure.size() < 2)
		throw new std::range_error(
			"Too small number of layers (at least 2"
			" needed for input and output layers)!");
#endif

	func = function.Function;
	dfunc = function.dFunction;
	new (&this->structure) vec<int>(structure);
	layers_num = structure.size();
	max_layer_size = this->structure.max_element();

#ifdef _DEBUG
	srand(0);
#endif

	new (&w) vec<vec<vec<double>>>(layers_num);
	for (int l = 0; l < layers_num - 1; l++) {
#ifdef _DEBUG
		//Errors handling
		if (this->structure[l] == 0)
			throw new std::range_error(
				"Invalid layer size (the layer must have at"
				" least 1 neuron)");
#endif
		new (&w[l]) vec<vec<double>>(this->structure[l]);
		for (int f = 0; f < this->structure[l]; f++)
		{
			new (&w[l][f]) vec<double>(this->structure[l + 1]);
			for (int t = 0; t < this->structure[l + 1]; t++)
				w[l][f][t] = (double)rand() / RAND_MAX;
		}
	}
}

SimplePropagationResult SimpleNetwork::ForwardPropagation(
	const vec<double>& input_data) const
{
#ifdef _DEBUG
	//Errors handling
	if (input_data.size() != structure[0]) {
		std::stringstream ss;
		ss << "Your input data size(" << input_data.size();
		ss << ") doesn\'t equal to the input layer size of the network(";
		ss << structure[0] << ")!";
		throw new std::out_of_range(ss.str());
	}
#endif

	//Creating result structure
	SimplePropagationResult ret{ (SimpleNetwork*) this };
	new (&ret.z) vec<vec<double>>(layers_num);
	new (&ret.a) vec<vec<double>>(layers_num);
	new (&ret.z[0]) vec<double>(input_data);
	new (&ret.a[0]) vec<double>(input_data);

	//The propagation
	int from_layer_size, to_layer_size = structure[0];
	for(int wl = 0; wl < layers_num - 1; wl++)
	{
		from_layer_size = to_layer_size;
		to_layer_size = structure[wl + 1];
		new (&ret.z[wl + 1]) vec<double>(to_layer_size);
		new (&ret.a[wl + 1]) vec<double>(to_layer_size);
		ret.z[wl + 1] = w[wl][0] * ret.a[wl][0];
		for (int from = 1; from < from_layer_size; from++)
			ret.z[wl + 1] += w[wl][from] * ret.a[wl][from];
		for (int to = 0; to < to_layer_size; to++)
			ret.a[wl + 1][to] = func(ret.z[wl + 1][to]);
	}
	return ret;
}

SimpleBackwardPropagationResult SimpleNetwork::BackwardPropagation(
	const SimplePropagationResult& res,
	const vec<double>& desired_output) const
{
#ifdef _DEBUG
	//Errors handling
	if (desired_output.size() != structure[layers_num - 1]) {
		std::stringstream ss;
		ss << "Your output data size(" << desired_output.size();
		ss << ") doesn\'t equal to the output layer size of the network(";
		ss << structure[layers_num - 1] << ")!";
		throw new std::out_of_range(ss.str());
	}
#endif
	
	//Creating result structure
	SimpleBackwardPropagationResult ret;
	vec<vec<double>> dC_da(layers_num);
	new (&ret.dC_dw) vec<vec<vec<double>>>(layers_num - 1);
	dC_da[layers_num - 1] = (desired_output - res.a[layers_num - 1]) * 2.0;
	ret.error = 0;
	for (int i = 0; i < structure[layers_num - 1]; i++)
		ret.error += dC_da[layers_num - 1][i];
	ret.error /= structure[layers_num - 1];

	//The propagation
	vec<double> buff_dC_dz(max_layer_size);
	int from_layer_size = structure[layers_num - 1], to_layer_size;
	for(int wl = layers_num - 2; wl >= 0; wl--)
	{
		to_layer_size = from_layer_size;
		from_layer_size = structure[wl];
		buff_dC_dz.resize(to_layer_size);
		for (int to = 0; to < to_layer_size; to++)
			buff_dC_dz[to] = dC_da[wl + 1][to] * dfunc(res.z[wl + 1][to]);
		ret.dC_dw[wl] = res.a[wl] & buff_dC_dz;
		new (&dC_da[wl]) vec<double>(from_layer_size);
		for (int from = 0; from < from_layer_size; from++)
			dC_da[wl][from] = (w[wl][from] * buff_dC_dz).sum();
	}
	return ret;
}
