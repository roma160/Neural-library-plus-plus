#pragma once

#include "neural_core/simple_data.h"

using namespace System;

namespace NNL
{
	ref class WSimpleNetwork;
	
	/// <summary>
	/// Class that loads training data into the RAM.
	/// Performs quite fast, but requires a lot of memory.
	/// </summary>
	ref class TrainingData
	{
	internal:
		SimpleData* _simple_data;

	public:
		TrainingData(String^ fileLocation, bool isBinary);
		~TrainingData();
	};

	/// <summary>
	/// Class that opens stream to the training data, and implements
	/// interface of reading tests from it.
	/// </summary>
	ref class StreamTrainingData
	{
	internal:
		SimpleBinStreamData* _simple_data;

	public:
		StreamTrainingData(String^ fileLocation, bool isBinary);
		~StreamTrainingData();
	};
}
