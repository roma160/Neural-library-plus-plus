#ifndef NEURALNETWORKTRY_SIMPLETRAINER_H
#define NEURALNETWORKTRY_SIMPLETRAINER_H

#include "simple_network.h"

class SimpleTrainer
{
private:
	SimpleNetwork* Network;
	
	class SimpleTrainData;
	SimpleTrainData *TrainData;
	
public:
	/// <summary>
	/// Cooficients, that determine the speed of learning
	/// </summary>
	double C, Cb;
	/// <summary>
	/// Number of training iterations per one network check 
	/// </summary>
	int Iterations;
	
	class SimpleTrainData
	{
	private:
		size_t InputLayerSize;
		size_t OutputLayerSize;
		size_t DataSize;
		vec<vec<double>> InputData;
		vec<vec<double>> OutputData;

	public:
		SimpleTrainData(
			size_t input_layer_size,
			size_t output_layer_size);
		SimpleTrainData(
			size_t input_layer_size,
			size_t output_layer_size,
			size_t data_size,
			double** input_data,
			double** output_data);
		SimpleTrainData(std::string file_location,
			bool is_in_binary);
		SimpleTrainData(const char* file_location,
			bool is_in_binary);

		vec<double>& GetInputData(size_t index) const;
		vec<double>& GetOutputData(size_t index) const;

		void AddData(
			vec<double> input_data,
			vec<double> output_data);
		void AddData(
			const double input_data[],
			const double output_data[]);

		size_t GetSize() const;
		size_t GetInputLayerSize() const;
		size_t GetOutputLayerSize() const;

		void SaveData(std::string file_location, 
			bool write_in_binary);
		void SaveData(const char* file_location,
			bool write_in_binary);
	};
	
	SimpleTrainer(SimpleNetwork* network, SimpleTrainData* train_data);
	
	/// <param name="c">Speed of the weights changing</param>
	/// <param name="cb">Speed of the basis weights changing</param>
	/// <param name="iters">Number of training iterations per one network check</param>
	SimpleTrainer(SimpleNetwork* network,  SimpleTrainData* train_data,
		double c, double cb, int iters);

	double ComputeError(int test_index);
	double ComputeAverageError();

	/// <summary>
	/// Do one network check
	/// </summary>
	double TrainNetwork();

	/// <param name="checks">Number of network checks</param>
	void TrainNetwork(int checks);

	/// <summary>
	/// Does several network checks.
	///	Outputs log to the stream.
	/// </summary>
	/// <param name="checks">Number of network checks</param>
	/// <param name="stream">The stream to write the logs</param>
	void TrainNetwork(int checks, std::ostream* stream);
};

#endif //NEURALNETWORKTRY_SIMPLETRAINER_H