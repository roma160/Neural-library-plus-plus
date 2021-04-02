#ifndef NEURALNETWORKTRY_SIMPLETRAINER_H
#define NEURALNETWORKTRY_SIMPLETRAINER_H

#include "simple_network.h"
#include "simple_data.h"

class SimpleTrainer : RandomElement<>
{
private:
	SimpleNetwork* Network;
	I_SimpleData *TrainData;
	
public:
	/// <summary>
	/// Cooficients, that determine the speed of learning
	/// </summary>
	double C, Cb;
	/// <summary>
	/// Number of training iterations per one network check 
	/// </summary>
	int Iterations;
	
	SimpleTrainer(SimpleNetwork* network, I_SimpleData* train_data);
	
	/// <param name="c">Speed of the weights changing</param>
	/// <param name="cb">Speed of the basis weights changing</param>
	/// <param name="iters">Number of training iterations per one network check</param>
	SimpleTrainer(SimpleNetwork* network,  I_SimpleData* train_data,
		double c, double cb, int iters);

	double ComputeError(int test_index);
	double ComputeAverageError();

	/// <summary>
	/// Do one network check
	/// </summary>
	double TrainNetwork();

	/// <summary>
	/// Does several network checks.
	/// </summary>
	/// <param name="checks">Number of network checks</param>
	void TrainNetwork(int checks);

	/// <summary>
	/// Does several network checks.
	///	Outputs log to the stream.
	/// </summary>
	/// <param name="checks">Number of network checks</param>
	/// <param name="stream">The stream to write the logs</param>
	/// <param name="logging_rate">Number of checks per log</param>
	void TrainNetwork(int checks, std::ostream* stream, int logging_rate = 10);
};

#endif //NEURALNETWORKTRY_SIMPLETRAINER_H