#ifndef NEURALNETWORKTRY_SIMPLEDATA_H
#define NEURALNETWORKTRY_SIMPLEDATA_H

#include <string>
#include <fstream>
#include "math/vec.h"

class I_SimpleData
{
protected:
	size_t InputLayerSize;
	size_t OutputLayerSize;
	size_t DataSize;

public:
	virtual vec<double>* GetInputData(size_t index) = 0;
	virtual vec<double>* GetOutputData(size_t index) = 0;

	size_t GetSize() const;
	size_t GetInputLayerSize() const;
	size_t GetOutputLayerSize() const;
};

class SimpleData : public I_SimpleData
{
private:
	vec<vec<double>> InputData;
	vec<vec<double>> OutputData;

public:
	SimpleData(
		size_t input_layer_size,
		size_t output_layer_size);
	SimpleData(
		size_t input_layer_size,
		size_t output_layer_size,
		size_t data_size,
		double** input_data,
		double** output_data);
	SimpleData(std::string file_location,
		bool is_in_binary);
	SimpleData(const char* file_location,
		bool is_in_binary);

	vec<double>* GetInputData(size_t index) override;
	vec<double>* GetOutputData(size_t index) override;

	void AddData(
		vec<double> input_data,
		vec<double> output_data);
	void AddData(
		const double input_data[],
		const double output_data[]);

	void SaveData(std::string file_location,
		bool write_in_binary);
	void SaveData(const char* file_location,
		bool write_in_binary);
};

class SimpleBinStreamData : public I_SimpleData
{
private:
	std::string FileName;
	std::ifstream Stream;
	size_t DataStart;
	size_t ChunkSize;
	size_t InputDataChunkSize;
	size_t OutputDataChunkSize;

	int BufferSize, BufferIndex;
	struct BufferMember
	{
		size_t Index;
		vec<double> Input;
		vec<double> Output;
	} *Buffer;
	void add_to_buffer(size_t index);

public:
	/// <summary>
	/// Constructor for data, that you want to use in multi-thread application 
	/// </summary>
	/// <param name="file_name">Location with name of the file with the data</param>
	/// <param name="threads_num">Number of threads you want to use</param>
	SimpleBinStreamData(std::string file_name, int threads_num);
	/// <summary>
	/// Constructor for data, that you want to use in multi-thread application 
	/// </summary>
	/// <param name="file_name">Location with name of the file with the data</param>
	/// <param name="threads_num">Number of threads you want to use</param>
	SimpleBinStreamData(const char* file_name, int threads_num);
	~SimpleBinStreamData();
	
	SimpleBinStreamData(std::string file_name);
	SimpleBinStreamData(const char* file_name);

	vec<double>* GetInputData(size_t index) override;
	vec<double>* GetOutputData(size_t index) override;

	void Close();
	void Reopen();

	std::string GetFileName();
};

#endif //NEURALNETWORKTRY_SIMPLEDATA_H