#include "simple_data.h"
#include <fstream>
#include <utility>
#include "neural_core/data_saver.h"

size_t I_SimpleData::GetSize() const { return DataSize; }

size_t I_SimpleData::GetInputLayerSize() const { return InputLayerSize; }

size_t I_SimpleData::GetOutputLayerSize() const { return OutputLayerSize; }

SimpleData::SimpleData(
	size_t input_layer_size, size_t output_layer_size)
{
	InputLayerSize = input_layer_size;
	OutputLayerSize = output_layer_size;
	DataSize = 0;
	new (&InputData) vec<vec<double>>(0, false);
	new (&OutputData) vec<vec<double>>(0, false);
}

SimpleData::SimpleData(
	size_t input_layer_size, size_t output_layer_size, size_t data_size,
	double** input_data, double** output_data)
{
	InputLayerSize = input_layer_size;
	OutputLayerSize = output_layer_size;
	DataSize = data_size;
	new (&InputData) vec<vec<double>>(DataSize, false);
	new (&OutputData) vec<vec<double>>(DataSize, false);
	for (size_t i = 0; i < DataSize; i++)
	{
		new (&InputData[i]) vec<double>(InputLayerSize, input_data[i]);
		new (&OutputData[i]) vec<double>(OutputLayerSize, output_data[i]);
	}
}

SimpleData::SimpleData(
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

	if (is_in_binary)
	{
		DataRead::ReadBinary(file, InputLayerSize);
		DataRead::ReadBinary(file, OutputLayerSize);
		DataRead::ReadBinary(file, DataSize);
		new (&InputData) vec<vec<double>>(DataSize, false);
		new (&OutputData) vec<vec<double>>(DataSize, false);
		for (size_t i = 0; i < DataSize; i++)
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

SimpleData::SimpleData(
	const char* file_location, bool is_in_binary) :
	SimpleData(std::string(file_location), is_in_binary) { }

vec<double>* SimpleData::GetInputData(size_t index)
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
	return &InputData[index];
}

vec<double>* SimpleData::GetOutputData(size_t index)
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
	return &OutputData[index];
}

void SimpleData::AddData(
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

void SimpleData::AddData(
	const double input_data[], const double output_data[])
{
	InputData.push_back(vec<double>(InputLayerSize, (double*)input_data));
	OutputData.push_back(vec<double>(OutputLayerSize, (double*)output_data));
	DataSize++;
}

void SimpleData::SaveData(
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

	if (write_in_binary)
	{
		DataSave::SaveBinary(file, InputLayerSize);
		DataSave::SaveBinary(file, OutputLayerSize);
		DataSave::SaveBinary(file, DataSize);
		for (size_t i = 0; i < DataSize; i++)
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

void SimpleData::SaveData(
	const char* file_location, bool write_in_binary)
{ SaveData(std::string(file_location), write_in_binary); }

void SimpleBinStreamData::add_to_buffer(size_t index)
{
	if (++BufferIndex >= BufferSize) BufferIndex = 0;
	Stream.seekg(index * ChunkSize + DataStart);
	Stream.read((char*) Buffer[BufferIndex].Input.p(), InputDataChunkSize);
	Stream.read((char*) Buffer[BufferIndex].Output.p(), OutputDataChunkSize);
	Buffer[BufferIndex].Index = index;
}

SimpleBinStreamData::SimpleBinStreamData(std::string file_name, int threads_num)
{
	FileName = std::move(file_name);
	new (&Stream) std::ifstream(FileName,
		std::ios::binary | std::ios::in);
	if (!Stream)
	{
		std::stringstream ss;
		ss << "Was unable to open the file! ("
			<< FileName << ")";
		std::cerr << ss.str();
		throw new std::ios_base::failure(ss.str());
	}

	DataRead::ReadBinary(Stream, InputLayerSize);
	DataRead::ReadBinary(Stream, OutputLayerSize);
	DataRead::ReadBinary(Stream, DataSize);

	DataStart = 3 * sizeof(size_t);
	InputDataChunkSize = InputLayerSize * sizeof(double);
	OutputDataChunkSize = OutputLayerSize * sizeof(double);
	ChunkSize = InputDataChunkSize + OutputDataChunkSize;

	BufferSize = threads_num;
	BufferIndex = -1;
	Buffer = new BufferMember[BufferSize];
	for (int i = 0; i < BufferSize; i++)
	{
		new (&Buffer[i].Input) vec<double>(InputLayerSize, false);
		new (&Buffer[i].Output) vec<double>(OutputLayerSize, false);
	}
}

SimpleBinStreamData::SimpleBinStreamData(const char* file_name, int threads_num) :
	SimpleBinStreamData(std::string(file_name), threads_num) { }

SimpleBinStreamData::~SimpleBinStreamData()
{
	delete[] Buffer;
}

SimpleBinStreamData::SimpleBinStreamData(std::string file_name) :
	SimpleBinStreamData(file_name, 1) { }

SimpleBinStreamData::SimpleBinStreamData(const char* file_name) :
	SimpleBinStreamData(std::string(file_name), 1) { }

vec<double>* SimpleBinStreamData::GetInputData(size_t index)
{
	for (int i = 0; i < BufferSize; i++)
		if (Buffer[i].Index == index)
			return &Buffer[i].Input;
	add_to_buffer(index);
	return &Buffer[BufferIndex].Input;
}

vec<double>* SimpleBinStreamData::GetOutputData(size_t index)
{
	for (int i = 0; i < BufferSize; i++)
		if (Buffer[i].Index == index)
			return &Buffer[i].Output;
	add_to_buffer(index);
	return &Buffer[BufferIndex].Output;
}