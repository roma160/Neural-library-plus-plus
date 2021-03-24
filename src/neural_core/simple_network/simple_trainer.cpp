#include "simple_trainer.h"
#include "neural_core/data_saver.h"

SimpleTrainer::SimpleTrainData::SimpleTrainData(
	size_t input_layer_size, size_t output_layer_size)
{
	InputLayerSize = input_layer_size;
	OutputLayerSize = output_layer_size;
	DataSize = 0;
	new (&InputData) vec<vec<double>>(0, false);
	new (&OutputData) vec<vec<double>>(0, false);
}

SimpleTrainer::SimpleTrainData::SimpleTrainData(
	size_t input_layer_size, size_t output_layer_size, size_t data_size,
	double** input_data, double** output_data)
{
	InputLayerSize = input_layer_size;
	OutputLayerSize = output_layer_size;
	DataSize = data_size;
	new (&InputData) vec<vec<double>>(DataSize, false);
	new (&OutputData) vec<vec<double>>(DataSize, false);
	for(size_t i = 0; i < DataSize; i++)
	{
		new (&InputData[i]) vec<double>(InputLayerSize, input_data[i]);
		new (&OutputData[i]) vec<double>(OutputLayerSize, output_data[i]);
	}
}

SimpleTrainer::SimpleTrainData::SimpleTrainData(
	std::string file_location, bool is_in_binary)
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
	
	if(is_in_binary)
	{
		DataRead::ReadBinary(file, InputLayerSize);
		DataRead::ReadBinary(file, OutputLayerSize);
		DataRead::ReadBinary(file, DataSize);
		new (&InputData) vec<vec<double>>(DataSize, false);
		new (&OutputData) vec<vec<double>>(DataSize, false);
		for(size_t i = 0; i < DataSize; i++)
		{
			new (&InputData[i]) vec<double>(InputLayerSize, false);
			DataRead::ReadBinary(file, InputData[i]);
			new (&OutputData[i]) vec<double>(OutputLayerSize, false);
			DataRead::ReadBinary(file, OutputData[i]);
		}
	}
	else
	{
		DataRead::Read(file, InputLayerSize);
		DataRead::Read(file, OutputLayerSize);
		DataRead::Read(file, DataSize);
		new (&InputData) vec<vec<double>>(DataSize, false);
		new (&OutputData) vec<vec<double>>(DataSize, false);
		for (size_t i = 0; i < DataSize; i++)
		{
			new (&InputData[i]) vec<double>(InputLayerSize, false);
			DataRead::Read(file, InputData[i]);
			new (&OutputData[i]) vec<double>(OutputLayerSize, false);
			DataRead::Read(file, OutputData[i]);
		}
	}
	file.close();
}

SimpleTrainer::SimpleTrainData::SimpleTrainData(
	const char* file_location, bool is_in_binary) :
	SimpleTrainData(std::string(file_location), is_in_binary) { }

vec<double>& SimpleTrainer::SimpleTrainData::GetInputData(size_t index) const
{
#ifdef _DEBUG
	if (index >= DataSize) {
		std::stringstream ss;
		ss << "You\'ve tried to access " << index << "th ";
		ss << "element of train data with size of " << DataSize;
		std::cerr << ss.str();
		throw std::out_of_range(ss.str());
	}
#endif
	return InputData[index];
}

vec<double>& SimpleTrainer::SimpleTrainData::GetOutputData(size_t index) const
{
#ifdef _DEBUG
	if (index >= DataSize) {
		std::stringstream ss;
		ss << "You\'ve tried to access " << index << "th ";
		ss << "element of train data with size of " << DataSize;
		std::cerr << ss.str();
		throw std::out_of_range(ss.str());
	}
#endif
	return OutputData[index];
}

void SimpleTrainer::SimpleTrainData::AddData(
	vec<double> input_data, vec<double> output_data)
{
#ifdef _DEBUG
	if (InputLayerSize != input_data.size() ||
		OutputLayerSize != output_data.size()) {
		std::cerr << "Sizes of the pushed and current data don`t match!";
		throw std::out_of_range(
			"Sizes of the pushed and current data don`t match!");
	}
#endif
	InputData.push_back(input_data);
	OutputData.push_back(output_data);
	DataSize++;
}

void SimpleTrainer::SimpleTrainData::AddData(
	const double input_data[], const double output_data[])
{
	InputData.push_back(vec<double>(InputLayerSize, (double*)input_data));
	OutputData.push_back(vec<double>(OutputLayerSize, (double*)output_data));
	DataSize++;
}

size_t SimpleTrainer::SimpleTrainData::GetSize() const
	{ return DataSize; }

size_t SimpleTrainer::SimpleTrainData::GetInputLayerSize() const
	{ return InputLayerSize; }

size_t SimpleTrainer::SimpleTrainData::GetOutputLayerSize() const
	{ return OutputLayerSize; }

void SimpleTrainer::SimpleTrainData::SaveData(
	std::string file_location, bool write_in_binary)
{
	std::ofstream file;
	if (write_in_binary)
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
	
	if(write_in_binary)
	{
		DataSave::SaveBinary(file, InputLayerSize);
		DataSave::SaveBinary(file, OutputLayerSize);
		DataSave::SaveBinary(file, DataSize);
		for(size_t i = 0; i < DataSize; i++)
		{
			DataSave::SaveBinary(file, InputData[i], false);
			DataSave::SaveBinary(file, OutputData[i], false);
		}
	}
	else
	{
		DataSave::Save(file, InputLayerSize);
		DataSave::Save(file, OutputLayerSize);
		DataSave::Save(file, DataSize);
		for (size_t i = 0; i < DataSize; i++)
		{
			DataSave::Save(file, InputData[i], false);
			DataSave::Save(file, OutputData[i], false);
		}
	}
	file.close();
}

void SimpleTrainer::SimpleTrainData::SaveData(
	const char* file_location, bool write_in_binary)
	{ SaveData(std::string(file_location), write_in_binary); }

SimpleTrainer::SimpleTrainer(SimpleNetwork* network, SimpleTrainData* train_data) :
	SimpleTrainer(network, train_data, 1, 1, 100) { };

SimpleTrainer::SimpleTrainer(SimpleNetwork* network, SimpleTrainData* train_data,
	double c, double cb, int iters)
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
	buff = Network->ForwardPropagation(TrainData->GetInputData(test_index));
	output_data = &TrainData->GetOutputData(test_index);
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
		buff = Network->ForwardPropagation(TrainData->GetInputData(i));
		output_data = &TrainData->GetOutputData(i);
		for (size_t j = 0; j < output_size; j++)
			ret += pow(buff.a[output_layer_index][j] - (*output_data)[j], 2);
	}
	return ret / (TrainDataSize * output_size);
}

double SimpleTrainer::TrainNetwork()
{
	double error_av = 0;
	size_t TrainDataSize = TrainData->GetSize(), test;
	for (int i = 0; i < Iterations; i++)
	{
		test = rand() % TrainDataSize;
		SimpleBackwardPropagationResult backward =
			Network->BackwardPropagation(
				Network->ForwardPropagation(
					TrainData->GetInputData(test)),
				TrainData->GetOutputData(test));
		error_av += backward.error;
		Network->Weights += backward.dC_dw * C;
		Network->BasisWeights += backward.dC_dbw * Cb;
	}
	return error_av / Iterations;
}

void SimpleTrainer::TrainNetwork(int checks)
{
	size_t TrainDataSize = TrainData->GetSize(), test;
	for(int check = 0; check < checks; check++)
	{
		double error_av = 0;
		for (int i = 0; i < Iterations; i++)
		{
			test = rand() % TrainDataSize;
			SimpleBackwardPropagationResult backward =
				Network->BackwardPropagation(
					Network->ForwardPropagation(
						TrainData->GetInputData(test)),
					TrainData->GetOutputData(test));
			error_av += backward.error;
			Network->Weights += backward.dC_dw * C;
			Network->BasisWeights += backward.dC_dbw * Cb;
		}
	}
}

void SimpleTrainer::TrainNetwork(int checks, std::ostream* stream)
{
	size_t TrainDataSize = TrainData->GetSize(), test;
	for (int check = 0; check < checks; check++)
	{
		double error_av = 0;
		for (int i = 0; i < Iterations; i++)
		{
			test = rand() % TrainDataSize;
			SimpleBackwardPropagationResult backward =
				Network->BackwardPropagation(
					Network->ForwardPropagation(
						TrainData->GetInputData(test)),
					TrainData->GetOutputData(test));
			error_av += backward.error;
			Network->Weights += backward.dC_dw * C;
			Network->BasisWeights += backward.dC_dbw * Cb;
		}
		*stream << "Iteration : " << check + 1 <<
			" - Error : " << error_av / Iterations << "\n";
	}
}
