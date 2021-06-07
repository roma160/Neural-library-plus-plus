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

unsigned long NNL::WSimpleNetwork::OutputLayerSize::get()
{ return _simple_network->GetOutputLayerSize(); }

cli::array<double>^ NNL::WSimpleNetwork::GetNetworkOutput(ITrainingData^ data, long testIndex)
{
	vec<double> buff = _simple_network->ComputeNetworkAnswer(*data->_simple_data->GetInputData(testIndex));
	return vec_to_array(&buff);
}
