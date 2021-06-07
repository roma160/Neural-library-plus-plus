#pragma once

#include "neural_core/simple_network.h"
#include "TrainingData.h"

using namespace System;

namespace NNL
{
	ref class WSimpleNetwork
	{
	private:
		SimpleNetwork* _simple_network;
		cli::array<double>^ get_network_output(I_SimpleData* data, long test_index);

	public:
		/// <summary>
		/// Loads network weight from the file.
		/// </summary>
		WSimpleNetwork(String^ fileLocation, bool isBinary);
		~WSimpleNetwork();

		property unsigned long OutputLayerSize
		{ unsigned long get(); }

		cli::array<double>^ GetNetworkOutput(TrainingData^ data, long testIndex);
		cli::array<double>^ GetNetworkOutput(StreamTrainingData^ data, long testIndex);
	};
}
