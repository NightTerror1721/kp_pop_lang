#include "functions.h"

Callable::Callable(const std::string& name, bool returnAny, std::vector<std::pair<std::string, DataType>> parameters) :
	_name{ name },
	_return{ returnAny },
	_parameters{ parameters }
{}

const std::string Callable::name() const { return _name; }

size_t Callable::getParameterCount() const { return _parameters.size(); }
const std::vector<std::pair<std::string, DataType>>& Callable::parameters() const { return _parameters; }

bool Callable::returnAny() const { return _return; }
