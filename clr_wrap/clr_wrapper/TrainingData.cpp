#include "pch.h"
#include "TrainingData.h"
#include "utils.h"

using namespace Runtime::InteropServices;

array<double>^ NNL::ITrainingData::GetInputData(long index)
{ return vec_to_array(_simple_data->GetInputData(index)); }
array<double>^ NNL::ITrainingData::GetOutputData(long index)
{ return vec_to_array(_simple_data->GetOutputData(index));  }

NNL::TrainingData::TrainingData(String^ fileLocation, bool isBinary)
{
	_simple_data = new SimpleData(
		(const char*)(void*) Marshal::StringToHGlobalAnsi(fileLocation),
		isBinary
	);
}
NNL::TrainingData::~TrainingData()
{ delete (SimpleData*) _simple_data; }

NNL::StreamTrainingData::StreamTrainingData(String^ fileLocation, bool isBinary)
{
	_simple_data = new SimpleBinStreamData(
		(const char*)(void*)Marshal::StringToHGlobalAnsi(fileLocation),
		isBinary
	);
}
NNL::StreamTrainingData::~StreamTrainingData()
{ delete (SimpleBinStreamData*) _simple_data; }
