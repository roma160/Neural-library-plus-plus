#pragma once

#include "neural_core/simple_network.h"
#include "TrainingData.h"

using namespace System;

namespace NNL
{
	public ref class WSimpleNetwork
	{
	private:
		SimpleNetwork* _simple_network;

	public:
		/// <summary>
		/// Loads network weight from the file.
		/// </summary>
		WSimpleNetwork(String^ fileLocation, bool isBinary);
		~WSimpleNetwork();
		
		property unsigned long InputLayerSize
		{ unsigned long get(); }
		property unsigned long OutputLayerSize
		{ unsigned long get(); }

		cli::array<double>^ GetNetworkOutput(ITrainingData^ data, long testIndex);
		cli::array<double>^ GetNetworkOutput(cli::array<double>^ input_data);
	};
}
