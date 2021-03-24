#include <iostream>
#include "math/vec.h"
#include "neural_core/simple_network/simple_network.h"
#include "neural_core/simple_network/simple_trainer.h"

using namespace std;

///Tests location : ../../../testing/tests/

int main(){
	//SimpleNetwork network("../../../testing/tests/xor/network_structure.b", true);
	SimpleNetwork network(Sigmoid, {2, 2, 1});
	SimpleTrainer::SimpleTrainData train_data(
		"../../../testing/tests/xor/train_data.b", true);
	SimpleTrainer trainer(&network, &train_data, 10, 0.05, 100);
	trainer.TrainNetwork(100);
	size_t DataSize = train_data.GetSize();
	size_t LayersNumber = network.Structure.size();
	
	for (int i = 0; i < DataSize; i++)
	{
		vec<double>* input = &train_data.GetInputData(i);
		cout << *input << " - ";
		SimpleForwardPropagationResult res =
			network.ForwardPropagation(*input);
		cout << res.a[LayersNumber - 1] << "\n";
	}
	
    return 0;
}