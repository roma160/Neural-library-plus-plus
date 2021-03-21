#include "stringable.h"
#include <typeinfo>
#include <string>
#include <ostream>

std::string stringable::to_string() const
{
    std::string ret(typeid(this).name());
    ret += " doesn\'t have to_string() overrided in it.";
    return ret;
}

std::ostream& operator<<(std::ostream& stream, const stringable& obj)
{
    stream << obj.to_string();
    return stream;
}
