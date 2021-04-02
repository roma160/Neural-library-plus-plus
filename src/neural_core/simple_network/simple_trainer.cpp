#include "simple_trainer.h"
#include "neural_core/data_saver.h"

SimpleTrainer::SimpleTrainer(SimpleNetwork* network, I_SimpleData* train_data) :
	SimpleTrainer(network, train_data, 1, 1, 100) { }

SimpleTrainer::SimpleTrainer(SimpleNetwork* network, I_SimpleData* train_data,
	double c, double cb, int iters) : RandomElement()
{
#ifdef _DEBUG
	if(train_data->GetInputLayerSize() != network->GetInputLayerSize())
	{
		std::cerr << "Sizes of the network and data input layer, don`t match!";
		throw std::out_of_range(
			"Sizes of the network and data input layer, don`t match!");
	}
	if(train_data->GetOutputLayerSize() != network->GetOutputLayerSize())
	{
		std::cerr << "Sizes of the network and data output layer, don`t match!";
		throw std::out_of_range(
			"Sizes of the network and data output layer, don`t match!");
	}
#endif
	Network = network;
	TrainData = train_data;
	C = c;
	Cb = cb;
	Iterations = iters;
}

double SimpleTrainer::ComputeError(int test_index)
{
	size_t TrainDataSize = TrainData->GetSize();
#ifdef _DEBUG
	if (test_index >= TrainDataSize) {
		std::stringstream ss;
		ss << "You\'ve tried to access " << test_index << "th ";
		ss << "element of train data with size of " << TrainDataSize;
		std::cerr << ss.str();
		throw std::out_of_range(ss.str());
	}
#endif
	size_t output_size = Network->GetOutputLayerSize(),
		output_layer_index = Network->Structure.size() - 1;
	SimpleForwardPropagationResult buff;
	vec<double>* output_data;
	double ret = 0;
	buff = Network->ForwardPropagation(*TrainData->GetInputData(test_index));
	output_data = TrainData->GetOutputData(test_index);
	for (size_t j = 0; j < output_size; j++)
		ret += pow(buff.a[output_layer_index][j] - (*output_data)[j], 2);
	return ret / output_size;
}

double SimpleTrainer::ComputeAverageError()
{
	size_t TrainDataSize = TrainData->GetSize(),
		output_size = Network->GetOutputLayerSize(),
		output_layer_index = Network->Structure.size() - 1;
	double ret = 0;
	SimpleForwardPropagationResult buff;
	vec<double>* output_data;
	for(size_t i = 0; i < TrainDataSize; i++)
	{
		buff = Network->ForwardPropagation(*TrainData->GetInputData(i));
		output_data = TrainData->GetOutputData(i);
		for (size_t j = 0; j < output_size; j++)
			ret += pow(buff.a[output_layer_index][j] - (*output_data)[j], 2);
	}
	return ret / (TrainDataSize * output_size);
}

double SimpleTrainer::TrainNetwork()
{
	const size_t TrainDataSize = TrainData->GetSize();
	size_t test = rnd(TrainDataSize);
	vec<double>* input_data = TrainData->GetInputData(test),
		* output_data = TrainData->GetOutputData(test);
	SimpleBackwardPropagationResult backward =
		Network->BackwardPropagation(
			Network->ForwardPropagation(*input_data),
			*output_data);

	double average_err = backward.error;
	vec<vec<vec<double>>> dC_dw(backward.dC_dw);
	vec<vec<double>> dC_dbw(backward.dC_dbw);
	
	for (int i = 1; i < Iterations; i++)
	{
		test = rnd(TrainDataSize);
		input_data = TrainData->GetInputData(test);
		output_data = TrainData->GetOutputData(test);
		backward = Network->BackwardPropagation(
				Network->ForwardPropagation(*input_data),
				*output_data);
		
		average_err += backward.error;
		dC_dw += backward.dC_dw;
		dC_dbw += backward.dC_dbw;
	}
	Network->Weights += dC_dw * (C / Iterations);
	Network->BasisWeights += dC_dbw * (Cb / Iterations);
	return average_err / Iterations;
}

void SimpleTrainer::TrainNetwork(int checks)
{
	const size_t TrainDataSize = TrainData->GetSize();
	size_t test;
	vec<double>* input_data, * output_data;
	SimpleBackwardPropagationResult backward;
	vec<vec<vec<double>>> dC_dw;
	vec<vec<double>> dC_dbw;
	
	for(int check = 0; check < checks; check++)
	{
		test = rnd(TrainDataSize);
		input_data = TrainData->GetInputData(test),
			output_data = TrainData->GetOutputData(test);
		backward = Network->BackwardPropagation(
				Network->ForwardPropagation(*input_data),
				*output_data);
		dC_dw = backward.dC_dw;
		dC_dbw = backward.dC_dbw;

		for (int i = 1; i < Iterations; i++)
		{
			test = rnd(TrainDataSize);
			input_data = TrainData->GetInputData(test);
			output_data = TrainData->GetOutputData(test);
			backward = Network->BackwardPropagation(
				Network->ForwardPropagation(*input_data),
				*output_data);

			dC_dw += backward.dC_dw;
			dC_dbw += backward.dC_dbw;
		}
		Network->Weights += dC_dw * (C / Iterations);
		Network->BasisWeights += dC_dbw * (Cb / Iterations);
	}
}

void SimpleTrainer::TrainNetwork(int checks, std::ostream* stream, int logging_rate)
{
	const size_t TrainDataSize = TrainData->GetSize();
	size_t test;
	vec<double>* input_data, * output_data;
	SimpleBackwardPropagationResult backward;
	vec<vec<vec<double>>> dC_dw;
	vec<vec<double>> dC_dbw;
	double average_err;

	for (int check = 0; check < checks; check++)
	{
		test = rnd(TrainDataSize);
		input_data = TrainData->GetInputData(test),
			output_data = TrainData->GetOutputData(test);
		backward = Network->BackwardPropagation(
			Network->ForwardPropagation(*input_data),
			*output_data);
		average_err = backward.error;

		//Memory leak fix :
		dC_dw = backward.dC_dw;
		dC_dbw = backward.dC_dbw;

		for (int i = 1; i < Iterations; i++)
		{
			test = rnd(TrainDataSize);
			input_data = TrainData->GetInputData(test);
			output_data = TrainData->GetOutputData(test);
			backward = Network->BackwardPropagation(
				Network->ForwardPropagation(*input_data),
				*output_data);

			dC_dw += backward.dC_dw;
			dC_dbw += backward.dC_dbw;
			average_err += backward.error;
		}
		Network->Weights += dC_dw * (C / Iterations);
		Network->BasisWeights += dC_dbw * (Cb / Iterations);

		if(!(check % logging_rate))
			*stream << "Iteration : " << check + 1 <<
				" - Error : " << average_err / Iterations << "\n";
	}
}
