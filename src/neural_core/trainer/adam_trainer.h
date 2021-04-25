#ifndef NEURALNETWORKTRY_ADAMTRAINER_H
#define NEURALNETWORKTRY_ADAMTRAINER_H

#include "network_trainer.h"

/// <summary>
/// https://habr.com/ru/post/318970/
/// </summary>
class AdamTrainer : NetworkTrainer
{
	/// <summary> Really small number </summary>
	const double eta = 1e-8;
	
public:
	/// <summary> Training speed coefficient </summary>
	double Alpha;
	/// <summary> How much prev gradient is used </summary>
	double Beta1;
	/// <summary> How much more we edit unique weights </summary>
	double Beta2;

	/// <summary> How many times we artificially increase m </summary>
	int T_stop1;
	/// <summary> How many times we artificially increase v </summary>
	int T_stop2;

	/// <summary> Gradient average </summary>
	vec<vec<vec<double>>> m;
	/// <summary> Usage average </summary>
	vec<vec<vec<double>>> v;

	AdamTrainer(SimpleNetwork* network, I_SimpleData* train_data,
		double beta1 = .9, double beta2 = .999, double alpha = .001,
		int t_stop1 = 10, int t_stop2 = 1000);

	double ComputeError(int test_index) override;
	double ComputeAverageError() override;

	double TrainNetwork() override;

	void TrainNetwork(int checks) override;

	void TrainNetwork(int checks, std::ostream* stream, int logging_rate = 10) override;
};

#endif //NEURALNETWORKTRY_ADAMTRAINER_H
