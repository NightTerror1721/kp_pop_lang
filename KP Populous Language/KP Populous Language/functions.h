#pragma once

#include <map>

#include "datatypes.h"
#include "script.h"

class Callable
{
private:
	std::string _name;
	std::vector<std::pair<std::string, DataType>> _parameters;
	bool _return;

public:
	Callable(const std::string& name, bool returnAny, std::vector<std::pair<std::string, DataType>> parameters);

	const std::string name() const;

	size_t getParameterCount() const;
	const std::vector<std::pair<std::string, DataType>>& parameters() const;

	bool returnAny() const;

	virtual void build(ScriptCodeBuilder& builder) const = 0;
};


/*class Getter : public Callable
{
private:
	const ScriptCode _code;


public:
	Getter(const std::string& name, ScriptCode code);

	void build(ScriptCodeBuilder& builder) const override;
};



class Function
{
private:
	std::string _name;
};

namespace
{
	class _FunctionManager
	{
	private:
		std::map<std::string, Callable*> _funcs;

	public:
		_FunctionManager();
		~_FunctionManager();

		bool has(const std::string& name) const;

		const Callable* get(const std::string& name) const;

		void registerGetter(const std::string& name, AttributeInternal attribute);
		void registerGetter(const std::string& name, ReadOnlyInternal internal);
	};
}*/

namespace function
{
	
}
