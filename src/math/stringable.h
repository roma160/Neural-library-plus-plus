//
// Created by 1 on 20.03.2021.
//

#ifndef NEURALNETWORKTRY_STRINGABLE_H
#define NEURALNETWORKTRY_STRINGABLE_H

#include <string>
#include <typeinfo>

class stringable{
public:
    virtual std::string to_string() const{
        std::string ret(typeid(this).name());
        ret += " doesn\'t have to_string() overrided in it.";
        return ret;
    }
};

std::ostream& operator << (std::ostream& stream, const stringable& obj) {
    stream<<obj.to_string();
    return stream;
}

#endif //NEURALNETWORKTRY_STRINGABLE_H
