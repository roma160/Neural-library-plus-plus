#ifndef NEURALNETWORKTRY_SIMPLETRAINER_H
#define NEURALNETWORKTRY_SIMPLETRAINER_H

#include "network_trainer.h"

class SimpleTrainer : NetworkTrainer
{	
public:
	/// <summary>
	/// Coefficients, that determine the speed of learning
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
	SimpleTrainer(SimpleNetwork* network, I_SimpleData* train_data,
		double c, double cb, int iters);

	double ComputeError(int test_index) override;
	double ComputeAverageError() override;

	double TrainNetwork() override;

	void TrainNetwork(int checks) override;

	void TrainNetwork(int checks, std::ostream* stream, int logging_rate = 10) override;
};

#endif //NEURALNETWORKTRY_SIMPLETRAINER_H