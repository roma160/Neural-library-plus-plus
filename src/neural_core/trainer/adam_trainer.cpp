#include <neural_core/trainer/adam_trainer.h>

AdamTrainer::AdamTrainer(SimpleNetwork* network, I_SimpleData* train_data,
	double beta1, double beta2, double alpha,
	int t_stop1, int t_stop2) : NetworkTrainer(network, train_data)
{
	Beta1 = beta1;
	Beta2 = beta2;
	Alpha = alpha;
	T_stop1 = t_stop1;
	T_stop2 = t_stop2;
}

void AdamTrainer::TrainNetwork(int checks)
{
	
}
