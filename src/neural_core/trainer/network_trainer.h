#ifndef NEURALNETWORKTRY_NETWORKTRAINER_H
#define NEURALNETWORKTRY_NETWORKTRAINER_H

#include "math/random_element.h"
#include "neural_core/simple_data.h"
#include "neural_core/simple_network.h"

class NetworkTrainer : protected RandomElement<>
{
protected:
	SimpleNetwork* Network;
	I_SimpleData* TrainData;

public:
	NetworkTrainer(SimpleNetwork* network, I_SimpleData* train_data)
	{
		Network = network;
		TrainData = train_data;
	}

	virtual double ComputeError(int test_index) = 0;
	virtual double ComputeAverageError() = 0;
	
	/// <summary>
	/// Do one network check
	/// </summary>
	virtual double TrainNetwork() = 0;

	/// <summary>
	/// Does several network checks.
	/// </summary>
	/// <param name="checks">Number of network checks</param>
	virtual void TrainNetwork(int checks) = 0;

	/// <summary>
	/// Does several network checks.
	///	Outputs log to the stream.
	/// </summary>
	/// <param name="checks">Number of network checks</param>
	/// <param name="stream">The stream to write the logs</param>
	/// <param name="logging_rate">Number of checks per log</param>
	virtual void TrainNetwork(int checks, std::ostream* stream, int logging_rate = 10) = 0;
};

#endif //NEURALNETWORKTRY_NETWORKTRAINER_H
