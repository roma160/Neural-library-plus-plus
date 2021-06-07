#include "pch.h"
#include "WSimpleNetwork.h"
#include "neural_core/simple_data.h"

using namespace Runtime::InteropServices;

NNL::WSimpleNetwork::WSimpleNetwork(String^ fileLocation, bool isBinary)
{
	_simple_network = new SimpleNetwork(
		(const char*)(void*) Marshal::StringToHGlobalAuto(fileLocation),
		isBinary
	);
}

NNL::WSimpleNetwork::~WSimpleNetwork()
{ delete _simple_network; }

unsigned long NNL::WSimpleNetwork::OutputLayerSize::get()
{ return _simple_network->GetOutputLayerSize(); }

cli::array<double>^ NNL::WSimpleNetwork::get_network_output(I_SimpleData* data, long test_index)
{
	vec<double> buff = _simple_network->ComputeNetworkAnswer(*data->GetInputData(test_index));
	size_t length = buff.size();
	cli::array<double>^ ret = gcnew cli::array<double>(length);
	Marshal::Copy(IntPtr(buff.p()), ret, 0, length);
	return ret;
}
cli::array<double>^ NNL::WSimpleNetwork::GetNetworkOutput(TrainingData^ data, long testIndex)
{ return get_network_output(data->_simple_data, testIndex); }
cli::array<double>^ NNL::WSimpleNetwork::GetNetworkOutput(StreamTrainingData^ data, long testIndex)
{ return get_network_output(data->_simple_data, testIndex); }
