//
// Created by 1 on 20.03.2021.
//

#ifndef NEURALNETWORKTRY_STRINGABLE_H
#define NEURALNETWORKTRY_STRINGABLE_H

#include <string>

class stringable{
public:
    virtual std::string to_string() const;
};

std::ostream& operator<< (std::ostream& stream, const stringable& obj);

#endif //NEURALNETWORKTRY_STRINGABLE_H