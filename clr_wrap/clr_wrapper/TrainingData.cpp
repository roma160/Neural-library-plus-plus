#include "pch.h"
#include "TrainingData.h"

using namespace Runtime::InteropServices;

NNL::TrainingData::TrainingData(String^ fileLocation, bool isBinary)
{
	_simple_data = new SimpleData(
		(const char*)(void*) Marshal::StringToHGlobalAuto(fileLocation),
		isBinary
	);
}
NNL::TrainingData::~TrainingData()
{ delete _simple_data; }

NNL::StreamTrainingData::StreamTrainingData(String^ fileLocation, bool isBinary)
{
	_simple_data = new SimpleBinStreamData(
		(const char*)(void*)Marshal::StringToHGlobalAuto(fileLocation),
		isBinary
	);
}
NNL::StreamTrainingData::~StreamTrainingData()
{ delete _simple_data; }
