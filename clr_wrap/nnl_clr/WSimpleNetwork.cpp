#include "pch.h"
#include "WSimpleNetwork.h"
#include "neural_core/simple_data.h"
#include "utils.h"

using namespace Runtime::InteropServices;

NNL::WSimpleNetwork::WSimpleNetwork(String^ fileLocation, bool isBinary)
{
	_simple_network = new SimpleNetwork(
		(const char*)(void*)Marshal::StringToHGlobalAnsi(fileLocation),
		isBinary
	);
}
NNL::WSimpleNetwork::~WSimpleNetwork()
{ delete _simple_network; }

unsigned long NNL::WSimpleNetwork::InputLayerSize::get()
{ return _simple_network->GetInputLayerSize(); }
unsigned long NNL::WSimpleNetwork::OutputLayerSize::get()
{ return _simple_network->GetOutputLayerSize(); }

cli::array<double>^ NNL::WSimpleNetwork::GetNetworkOutput(ITrainingData^ data, long testIndex)
{
	vec<double> buff = _simple_network->ComputeNetworkAnswer(
		*data->_simple_data->GetInputData(testIndex));
	return vec_to_array(&buff);
}
cli::array<double>^ NNL::WSimpleNetwork::GetNetworkOutput(cli::array<double>^ input_data)
{
	vec<double> buff = _simple_network->ComputeNetworkAnswer(array_to_vec(input_data));
	return vec_to_array(&buff);
}
