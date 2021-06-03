#ifndef NEURALNETWORKTRY_DATASAVER_H
#define NEURALNETWORKTRY_DATASAVER_H

#include "math/vec.h"

using namespace std;

namespace NNL {
    namespace DataSave {
        template<typename T>
        void Save(std::ofstream &stream, const T &data);

        template<typename T>
        void Save(std::ofstream &stream,
                  const vec<T> &data, bool write_size);

        template<typename T>
        void SaveBinary(std::ofstream &stream, const T &data);

        template<typename T>
        void SaveBinary(std::ofstream &stream,
                        const vec<T> &data, bool write_size);
    }

    namespace DataRead {
        template<typename T>
        void Read(std::ifstream &stream, T &data);

        /// <summary>
        /// Reads the size of vec<T> data,
        /// and initializes it.
        /// </summary>
        template<typename T>
        void ReadWithSize(std::ifstream &stream, vec<T> *data);

        /// <summary>
        /// Writes data to initialized vec<T> data,
        /// which already sized properly
        /// </summary>
        template<typename T>
        void Read(std::ifstream &stream, vec<T> &data);

        template<typename T>
        void ReadBinary(std::ifstream &stream, T &data);

        /// <summary>
        /// Reads the size of vec<T> data,
        /// and initializes it.
        /// </summary>
        template<typename T>
        void ReadBinaryWithSize(std::ifstream &stream, vec<T> *data);

        /// <summary>
        /// Writes data to initialized vec<T> data,
        /// which already sized properly
        /// </summary>
        template<typename T>
        void ReadBinary(std::ifstream &stream, vec<T> &data);
    }

#include "data_saver.tpp"

}

#endif //NEURALNETWORKTRY_DATASAVER_H