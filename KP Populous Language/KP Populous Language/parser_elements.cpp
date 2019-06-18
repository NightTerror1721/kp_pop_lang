#include "parser_elements.h"

#include <sstream>

CodeFragment::~CodeFragment() {}

bool CodeFragment::is(const CodeFragmentType type) { return getCodeFragmentType() == type; }

bool CodeFragment::is(const CodeFragmentType type1, const CodeFragmentType type2)
{
	const CodeFragmentType type = getCodeFragmentType();
	return type == type1 || type == type2;
}

bool CodeFragment::is(const CodeFragmentType type1, const CodeFragmentType type2, const CodeFragmentType type3)
{
	const CodeFragmentType type = getCodeFragmentType();
	return type == type1 || type == type2 || type == type3;
}



Statement::~Statement() {}

bool Statement::isStatement() const { return true; }



Identifier::Identifier(const std::string& identifier) :
	Statement{},
	_identifier{ identifier }
{
	if (!isValid(identifier))
		throw InvalidIdentifier{};
}

CodeFragmentType Identifier::getCodeFragmentType() const { return CodeFragmentType::Identifier; }

std::string Identifier::toString() const { return _identifier; }

bool Identifier::operator== (const CodeFragment& cf) const
{
	try
	{
		const Identifier& other = dynamic_cast<const Identifier&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}

bool Identifier::operator== (const Identifier& cf) const { return _identifier == cf._identifier; }
bool Identifier::operator!= (const Identifier& cf) const { return _identifier != cf._identifier; }


const std::regex Identifier::PATTERN{"[_a-zA-Z][_a-zA-Z0-9]*"};


bool Identifier::isValid(const std::string& identifier) { return std::regex_search(identifier, PATTERN); }


LiteralInteger::LiteralInteger(const field_value_t value) :
	Statement{},
	_value{ value }
{}

field_value_t LiteralInteger::getValue() const { return _value; }

CodeFragmentType LiteralInteger::getCodeFragmentType() const { return CodeFragmentType::LiteralInteger; }

std::string LiteralInteger::toString() const { return std::to_string(_value); }

bool LiteralInteger::operator== (const CodeFragment& cf) const
{
	try
	{
		const LiteralInteger& other = dynamic_cast<const LiteralInteger&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}

bool LiteralInteger::operator== (const LiteralInteger& other) const { return _value == other._value; }
bool LiteralInteger::operator!= (const LiteralInteger& other) const { return _value != other._value; }

enum IntegerBase { Octal = 8, Decimal = 10, Hexadecimal = 16 };

IntegerBase find_integer_base(const std::string& str)
{
	if (str.size() <= 1 || str[0] != '0')
		return IntegerBase::Decimal;
	const char& c = str[1];
	return c == 'x' || c == 'X' ? IntegerBase::Hexadecimal : IntegerBase::Octal;
}

LiteralInteger LiteralInteger::parse(const std::string& str)
{
	return std::stol(str, nullptr, find_integer_base(str));
}

bool LiteralInteger::isValid(const std::string& str) { return std::regex_search(str, PATTERN); }

const std::regex LiteralInteger::PATTERN{ "(0|0[xX])?[0-9]+" };



TypeConstant::TypeConstant(ScriptCode code) :
	Statement{},
	_value{ code },
	_type{ DataType::findTypeFromValue(code) }
{}

DataType TypeConstant::getType() const { return _type; }
ScriptCode TypeConstant::getValue() const { return _value; }

CodeFragmentType TypeConstant::getCodeFragmentType() const { return CodeFragmentType::TypeConstant; }

std::string TypeConstant::toString() const { return _type ? _type.getValueIdentifier(_value) : std::to_string(_value); }

bool TypeConstant::operator== (const CodeFragment& cf) const
{
	try
	{
		const TypeConstant& other = dynamic_cast<const TypeConstant&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool TypeConstant::operator== (const TypeConstant& other) const { return _value == other._value; }
bool TypeConstant::operator!= (const TypeConstant& other) const { return _value != other._value; }

bool TypeConstant::isValid(const std::string& str) { return DataType::findTypeFromValueName(str); }
bool TypeConstant::isValid(ScriptCode code) { return DataType::findTypeFromValue(code); }



Stopchar::Stopchar(const char symbol) :
	CodeFragment{},
	_symbol{ symbol }
{}

bool Stopchar::isStatement() const { return false; }

CodeFragmentType Stopchar::getCodeFragmentType() const { return CodeFragmentType::Stopchar; }

std::string Stopchar::toString() const { return std::string{ _symbol }; }

bool Stopchar::operator== (const CodeFragment& cf) const
{
	try
	{
		const Stopchar& other = dynamic_cast<const Stopchar&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Stopchar::operator== (const Stopchar& other) const { return _symbol == other._symbol; }
bool Stopchar::operator!= (const Stopchar& other) const { return _symbol != other._symbol; }

const Stopchar Stopchar::Semicolon{ ';' };
const Stopchar Stopchar::Comma{ ',' };
const Stopchar Stopchar::TwoPoints{ ':' };



_ArgumentsList::_ArgumentsList() :
	_args{}
{}

_ArgumentsList::~_ArgumentsList()
{
	free_ptr_vector(_args);
}

bool _ArgumentsList::empty() const { return _args.empty(); }
size_t _ArgumentsList::size() const { return _args.size(); }

const Statement& _ArgumentsList::operator[] (const size_t idx) const { return *_args[idx]; }

std::string _ArgumentsList::toString() const
{
	if (_args.empty())
		return "()";

	std::stringstream ss{};
	ss << "(";

	Statement *const *s = _args.data(), *const *end = &_args[_args.size() - 1];
	for (; s <= end; ++s)
	{
		if (s == end)
			ss << (*s)->toString() << ")";
		else ss << (*s)->toString() << " ,";
	}

	return ss.str();
}

bool _ArgumentsList::operator== (const _ArgumentsList& other) const { return _args == other._args; }
bool _ArgumentsList::operator!= (const _ArgumentsList& other) const { return _args != other._args; }




Arguments::Arguments() :
	Statement{},
	_ArgumentsList{}
{}

CodeFragmentType Arguments::getCodeFragmentType() const { return CodeFragmentType::Arguments; }

std::string Arguments::toString() const { return _ArgumentsList::toString(); }

bool Arguments::operator== (const CodeFragment& cf) const
{
	try
	{
		const Arguments& other = dynamic_cast<const Arguments&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Arguments::operator== (const Arguments& other) const { return _ArgumentsList::operator==(other); }
bool Arguments::operator!= (const Arguments& other) const { return _ArgumentsList::operator!=(other); }




Operator::Operator(const std::string& symbol, const OperatorType type, unsigned int priority, bool rightToLeft, bool conditional) :
	_id{},
	_symbol{ symbol },
	_type{ type },
	_priority{ priority },
	_rightToLeft{ rightToLeft },
	_conditional{ conditional }
{
	static uint8_t id_gen = 0;
	_id = id_gen++;
}


unsigned int Operator::getPriority() const { return _priority; }
bool Operator::hasRightToLeft() const { return _rightToLeft; }
bool Operator::isConditional() const { return _conditional; }

bool Operator::isUnary() const { return _type == OperatorType::Unary; }
bool Operator::isBinary() const { return _type == OperatorType::Binary; }
bool Operator::isTernary() const { return _type == OperatorType::Ternary; }
bool Operator::isAssignment() const { return _type == OperatorType::Assignment; }

int Operator::comparePriority(const Operator& other) const
{
	if (_priority == other._priority)
		return _rightToLeft || other._rightToLeft ? -1 : 0;
	return _priority < other._priority ? 1 : -1;
}

bool Operator::isStatement() const { return false; }

CodeFragmentType Operator::getCodeFragmentType() const { return CodeFragmentType::Operator; }

std::string Operator::toString() const { return _symbol; }

bool Operator::operator== (const CodeFragment& cf) const
{
	try
	{
		const Operator& other = dynamic_cast<const Operator&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Operator::operator== (const Operator& other) const { return _id == other._id; }
bool Operator::operator!= (const Operator& other) const { return _id != other._id; }


const Operator Operator::SufixIncrement{ "++", OperatorType::Unary, 0, false, false };
const Operator Operator::SufixDecrement{ "--", OperatorType::Unary, 0, false, false };

const Operator Operator::PrefixIncrement{ "++", OperatorType::Unary, 1, true, false };
const Operator Operator::PrefixDecrement{ "--", OperatorType::Unary, 1, true, false };
const Operator Operator::UnaryMinus{ "-", OperatorType::Unary, 1, true, false };
const Operator Operator::BinaryNot{ "!", OperatorType::Unary, 1, true, false };

const Operator Operator::Multiplication{ "*", OperatorType::Binary, 2, false, false };
const Operator Operator::Division{ "/", OperatorType::Binary, 2, false, false };

const Operator Operator::Addition{ "+", OperatorType::Binary, 3, false, false };
const Operator Operator::Subtraction{ "-", OperatorType::Binary, 3, false, false };

const Operator Operator::GreaterThan{ ">", OperatorType::Binary, 4, false, true };
const Operator Operator::SmallerThan{ "<", OperatorType::Binary, 4, false, true };
const Operator Operator::GreaterEqualsThan{ ">=", OperatorType::Binary, 4, false, true };
const Operator Operator::SmallerEqualsThan{ "<=", OperatorType::Binary, 4, false, true };

const Operator Operator::EqualsTo{ "==", OperatorType::Binary, 5, false, true };
const Operator Operator::NotEqualsTo{ "!=", OperatorType::Binary, 5, false, true };

const Operator Operator::BinaryAnd{ "&&", OperatorType::Binary, 6, false, false };
const Operator Operator::BinaryOr{ "||", OperatorType::Binary, 6, false, false };

const Operator Operator::TernaryConditional{ "?=", OperatorType::Ternary, 7, false, false };

const Operator Operator::Assignment{ "=", OperatorType::Assignment, 8, true, false };
const Operator Operator::AssignmentAddition{ "+=", OperatorType::Assignment, 8, true, false };
const Operator Operator::AssignmentSubtraction{ "-=", OperatorType::Assignment, 8, true, false };
const Operator Operator::AssignmentMultiplication{ "*=", OperatorType::Assignment, 8, true, false };
const Operator Operator::AssignmentDivision{ "/=", OperatorType::Assignment, 8, true, false };







Operation::Operation(const Operator& op, Statement* const op1, Statement* const op2, Statement* const op3) :
	_operator{ op },
	_operands{}
{
	_operands.push_back(op1);
	if (op2) _operands.push_back(op2);
	if (op3) _operands.push_back(op3);
}

Operation::~Operation() { free_ptr_vector(_operands); }

unsigned int Operation::getOperandCount() const { return _operands.size(); }

const Operator& Operation::getOperator() const { return _operator; }

const Statement& Operation::getOperand(const size_t idx) const { return *_operands[idx]; }

bool Operation::isUnary() const { return _operator.isUnary(); }
bool Operation::isBinary() const { return _operator.isBinary(); }
bool Operation::isTernary() const { return _operator.isTernary(); }
bool Operation::isAssignment() const { return _operator.isAssignment(); }

CodeFragmentType Operation::getCodeFragmentType() const { return CodeFragmentType::Operation; }

std::string Operation::toString() const
{
	if (_operator.isUnary())
	{
		return _operator == Operator::SufixIncrement || _operator == Operator::SufixDecrement
			? _operands[0]->toString() + _operator.toString()
			: _operator.toString() + _operands[0]->toString();
	}
	if (_operator.isTernary())
		return _operands[0]->toString() + " ? " + _operands[1]->toString() + " : " + _operands[2]->toString();

	return _operands[0]->toString() + " " + " " + _operator.toString() + _operands[1]->toString();
}

bool Operation::operator== (const CodeFragment& cf) const
{
	try
	{
		const Operation& other = dynamic_cast<const Operation&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Operation::operator== (const Operation& other) const { return _operator == other._operator && _operands == other._operands; }
bool Operation::operator!= (const Operation& other) const { return _operator != other._operator || _operands != other._operands; }




FunctionCall::FunctionCall(const Callable* function, const Arguments& args) :
	_function{ function },
	_args{ args }
{}

const Callable* FunctionCall::getFunction() const { return _function; }

const Arguments& FunctionCall::getArguments() const { return _args; }

CodeFragmentType FunctionCall::getCodeFragmentType() const { return CodeFragmentType::FunctionCall; }

std::string FunctionCall::toString() const
{
	return _function->name() + _args.toString();
}

bool FunctionCall::operator== (const CodeFragment& cf) const
{
	try
	{
		const FunctionCall& other = dynamic_cast<const FunctionCall&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool FunctionCall::operator== (const FunctionCall& other) const { return _function == other._function && _args == other._args; }
bool FunctionCall::operator!= (const FunctionCall& other) const { return _function != other._function || _args != other._args; }




Command::Command(const std::string& name) :
	_id{},
	_name{ name }
{
	static uint8_t ids = 0;
	_id = ids++;
}

bool Command::isStatement() const { return false; }

CodeFragmentType Command::getCodeFragmentType() const { return CodeFragmentType::Command; }

std::string Command::toString() const { return _name; }

bool Command::operator== (const CodeFragment& cf) const
{
	try
	{
		const Command& other = dynamic_cast<const Command&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Command::operator== (const Command& other) const { return _id == other._id; }
bool Command::operator!= (const Command& other) const { return _id != other._id; }


const Command Command::Var{ "var" };
const Command Command::Const{ "const" };
const Command Command::Import{ "import" };
const Command Command::If{ "var" };
const Command Command::Else{ "var" };
const Command Command::Every{ "var" };





CommandArguments::CommandArguments() :
	CodeFragment{},
	_ArgumentsList{}
{}

bool CommandArguments::isStatement() const { return false; }

CodeFragmentType CommandArguments::getCodeFragmentType() const { return CodeFragmentType::CommandArguments; }

std::string CommandArguments::toString() const { return _ArgumentsList::toString(); }

bool CommandArguments::operator== (const CodeFragment& cf) const
{
	try
	{
		const CommandArguments& other = dynamic_cast<const CommandArguments&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool CommandArguments::operator== (const CommandArguments& other) const { return _ArgumentsList::operator==(other); }
bool CommandArguments::operator!= (const CommandArguments& other) const { return _ArgumentsList::operator!=(other); }





Scope::Scope() :
	Statement{},
	_instructions{}
{}

Scope::~Scope()
{
	free_ptr_vector(_instructions);
}

bool Scope::empty() const { return _instructions.empty(); }
size_t Scope::size() const { return _instructions.size(); }

Instruction& Scope::operator[] (const size_t idx) { return *_instructions[idx]; }
const Instruction& Scope::operator[] (const size_t idx) const { return *_instructions[idx]; }

Scope::iterator Scope::begin() { return _instructions.begin(); }
Scope::const_iterator Scope::begin() const { return _instructions.begin(); }
Scope::iterator Scope::end() { return _instructions.end(); }
Scope::const_iterator Scope::end() const { return _instructions.end(); }

CodeFragmentType Scope::getCodeFragmentType() const { return CodeFragmentType::Scope; }

std::string Scope::toString() const override;

bool Scope::operator== (const CodeFragment& cf) const
{
	try
	{
		const Scope& other = dynamic_cast<const Scope&>(cf);
		return *this == other;
	}
	catch (...) { return false; }
}
bool Scope::operator== (const Scope& other) const { return _instructions == other._instructions; }
bool Scope::operator!= (const Scope& other) const { return _instructions == other._instructions; }
