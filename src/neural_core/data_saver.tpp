#include "data_saver.h"
#include <fstream>

template<typename T>
void DataSave::Save(std::ofstream& stream, const T& data)
	{ stream<<data<<" "; }

template <typename T>
void DataSave::Save(std::ofstream& stream, 
	const vec<T>& data, bool write_size)
{
	size_t size = data.size();
	if (write_size)
		stream << size << " ";
	for (size_t i = 0; i < size; i++)
		stream << data[i] << " ";
}

template <typename T>
void DataSave::SaveBinary(std::ofstream& stream, const T& data)
	{ stream.write((char*)&data, sizeof(T)); }

template <typename T>
void DataSave::SaveBinary(std::ofstream& stream,
	const vec<T>& data, bool write_size)
{
	size_t size = data.size();
	if (write_size)
		stream.write((char*)&size, sizeof(size_t));
	stream.write((char*) data.p(), sizeof(T) * size);
}


template<typename T>
void DataRead::Read(std::ifstream& stream, T& data)
	{ stream >> data; }

template <typename T>
void DataRead::ReadWithSize(std::ifstream& stream, vec<T>* data)
{
	size_t size;
	stream >> size;
	new (data) vec<T>(size, false);
	for (size_t i = 0; i < size; i++)
		stream >> (*data)[i];
}

template <typename T>
void DataRead::Read(std::ifstream& stream, vec<T>& data)
{
	size_t size = data.size();
	for (size_t i = 0; i < size; i++)
		stream >> data[i];
}

template <typename T>
void DataRead::ReadBinary(std::ifstream& stream, T& data)
	{ stream.read((char*)&data, sizeof(T)); }

template <typename T>
void DataRead::ReadBinaryWithSize(
	std::ifstream& stream, vec<T>* data)
{
	size_t size;
	stream.read((char*)&size, sizeof(size_t));
	new (data) vec<T>(size, false);
	stream.read((char*) data->p(), sizeof(T) * size);
}

template <typename T>
void DataRead::ReadBinary(
	std::ifstream& stream, vec<T>& data)
{
	stream.read((char*) data.p(), 
		sizeof(T) * data.size());
}
