#pragma once

#include "neural_core/simple_data.h"

using namespace System;

namespace NNL
{
	public ref class ITrainingData
	{
	internal:
		I_SimpleData* _simple_data;

	public:
		cli::array<double>^ GetInputData(long index);
		cli::array<double>^ GetOutputData(long index);
	};
	
	/// <summary>
	/// Class that loads training data into the RAM.
	/// Performs quite fast, but requires a lot of memory.
	/// </summary>
	public ref class TrainingData : ITrainingData
	{
	public:
		TrainingData(String^ fileLocation, bool isBinary);
		~TrainingData();
	};

	/// <summary>
	/// Class that opens stream to the training data, and implements
	/// interface of reading tests from it.
	/// </summary>
	public ref class StreamTrainingData : ITrainingData
	{
	public:
		StreamTrainingData(String^ fileLocation, bool isBinary);
		~StreamTrainingData();
	};
}
