#pragma once

#include <string>
#include <regex>
#include <exception>

#include "datatypes.h"
#include "functions.h"

enum class CodeFragmentType
{
	Identifier,
	LiteralInteger,
	TypeConstant,
	Stopchar,
	Arguments,
	Operator,
	Operation,
	FunctionCall,
	Scope,
	CommandArguments,
	Command
};

class CodeFragment
{
public:
	virtual ~CodeFragment();

	virtual CodeFragmentType getCodeFragmentType() const = 0;

	virtual bool isStatement() const = 0;

	virtual std::string toString() const = 0;

	virtual bool operator== (const CodeFragment& cf) const = 0;
	inline bool operator!= (const CodeFragment& cf) const { return !operator==(cf); }

	bool is(const CodeFragmentType type);
	bool is(const CodeFragmentType type1, const CodeFragmentType type2);
	bool is(const CodeFragmentType type1, const CodeFragmentType type2, const CodeFragmentType type3);
};

class Statement : public CodeFragment
{
public:
	virtual ~Statement();

	bool isStatement() const override;
};


class InvalidIdentifier : public std::exception {};


class Identifier : public Statement
{
private:
	std::string _identifier;

public:
	Identifier(const std::string& identifier);

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Identifier& cf) const;
	bool operator!= (const Identifier& cf) const;

public:
	static const std::regex PATTERN;

public:
	static bool isValid(const std::string& identifier);
};


class LiteralInteger : public Statement
{
private:
	field_value_t _value;

public:
	LiteralInteger(const field_value_t value);

	field_value_t getValue() const;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const LiteralInteger& other) const;
	bool operator!= (const LiteralInteger& other) const;

	static LiteralInteger parse(const std::string& str);
	static bool isValid(const std::string& str);

	static const std::regex PATTERN;
};


class TypeConstant : public Statement
{
private:
	DataType _type;
	ScriptCode _value;

public:
	TypeConstant(ScriptCode code);

	DataType getType() const;
	ScriptCode getValue() const;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const TypeConstant& other) const;
	bool operator!= (const TypeConstant& other) const;

	static bool isValid(const std::string& str);
	static bool isValid(ScriptCode code);
};



class Stopchar : public CodeFragment
{
private:
	char _symbol;

public:
	bool isStatement() const override;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Stopchar& other) const;
	bool operator!= (const Stopchar& other) const;

private:
	Stopchar(const char symbol);

public:
	static const Stopchar Semicolon;
	static const Stopchar Comma;
	static const Stopchar TwoPoints;
};



class _ArgumentsList
{
private:
	std::vector<Statement*> _args;

public:
	_ArgumentsList();
	virtual ~_ArgumentsList();

	bool empty() const;
	size_t size() const;

	template<class _ArgTy>
	void addArgument(const _ArgTy& arg)
	{
		Statement* ptr = new _ArgTy{ arg };
		_args.push_back(ptr);
	}

	const Statement& operator[] (const size_t idx) const;

	std::string toString() const;

	bool operator== (const _ArgumentsList& other) const;
	bool operator!= (const _ArgumentsList& other) const;
};



class Arguments : public Statement, public _ArgumentsList
{
public:
	Arguments();

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Arguments& other) const;
	bool operator!= (const Arguments& other) const;
};



enum class OperatorType
{
	Unary,
	Binary,
	Ternary,
	Assignment
};

class Operator : public CodeFragment
{
private:
	uint8_t _id;
	std::string _symbol;
	OperatorType _type;
	unsigned int _priority;
	bool _rightToLeft;
	bool _conditional;

	Operator(const std::string& symbol, const OperatorType type, unsigned int priority, bool rightToLeft, bool conditional);

public:
	unsigned int getPriority() const;
	bool hasRightToLeft() const;
	bool isConditional() const;

	bool isUnary() const;
	bool isBinary() const;
	bool isTernary() const;
	bool isAssignment() const;

	int comparePriority(const Operator& other) const;

	bool isStatement() const override;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Operator& other) const;
	bool operator!= (const Operator& other) const;

public:
	static const Operator SufixIncrement;
	static const Operator SufixDecrement;

	static const Operator PrefixIncrement;
	static const Operator PrefixDecrement;
	static const Operator UnaryMinus;
	static const Operator BinaryNot;

	static const Operator Multiplication;
	static const Operator Division;

	static const Operator Addition;
	static const Operator Subtraction;

	static const Operator GreaterThan;
	static const Operator SmallerThan;
	static const Operator GreaterEqualsThan;
	static const Operator SmallerEqualsThan;

	static const Operator EqualsTo;
	static const Operator NotEqualsTo;

	static const Operator BinaryAnd;
	static const Operator BinaryOr;

	static const Operator TernaryConditional;

	static const Operator Assignment;
	static const Operator AssignmentAddition;
	static const Operator AssignmentSubtraction;
	static const Operator AssignmentMultiplication;
	static const Operator AssignmentDivision;
};




class BadOperation : public std::exception {};

class Operation : public Statement
{
private:
	Operator _operator;
	std::vector<Statement*> _operands;

public:
	~Operation();

	unsigned int getOperandCount() const;

	const Operator& getOperator() const;

	const Statement& getOperand(const size_t idx) const;

	bool isUnary() const;
	bool isBinary() const;
	bool isTernary() const;
	bool isAssignment() const;

	template<class _StatementTy>
	const _StatementTy& getCastedOperand(const size_t idx) const { return *dynamic_cast<_StatementTy*>(_operands[idx]); }

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Operation& other) const;
	bool operator!= (const Operation& other) const;

private:
	Operation(const Operator& op, Statement* const op1, Statement* const op2 = nullptr, Statement* const op3 = nullptr);

public:
	template<class _OpTy>
	static Operation unary(const Operator& op, const _OpTy& operand)
	{
		return Operation{ op, dynamic_cast<Statement*>(new _OpTy{ operand }) };
	}

	template<class _OpLeftTy, class _OpRightTy>
	static Operation binary(const Operator& op, const _OpLeftTy& op_left, const _OpRightTy& op_right)
	{
		return Operation{ op,
			dynamic_cast<Statement*>(new _OpLeftTy{ op_left }),
			dynamic_cast<Statement*>(new _OpRightTy{ op_right })
		};
	}

	template<class _CondTy, class _OpTrueCondTy, class _OpFalseCondTy>
	static Operation ternary(const Operator& op, const Statement& condition, const Statement& true_cond_op, const Statement& false_cond_op)
	{
		return Operation{ op,
			dynamic_cast<Statement*>(new _CondTy{ condition }),
			dynamic_cast<Statement*>(new _OpTrueCondTy{ true_cond_op }),
			dynamic_cast<Statement*>(new _OpFalseCondTy{ false_cond_op })
		};
	}

	template<class _OpSourceTy>
	static Operation assignment(const Operator& op, const Identifier& dest, const _OpSourceTy& source)
	{
		return Operation{ op,
			reinterpret_cast<Statement*>(new Identifier{ dest }),
			dynamic_cast<Statement*>(new _OpSourceTy{ source })
		};
	}
};




class FunctionCall : public Statement
{
private:
	const Callable* _function;
	Arguments _args;

public:
	FunctionCall(const Callable* function, const Arguments& args);

	const Callable* getFunction() const;

	const Arguments& getArguments() const;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const FunctionCall& other) const;
	bool operator!= (const FunctionCall& other) const;
};



class Command : public CodeFragment
{
private:
	uint8_t _id;
	std::string _name;

public:
	bool isStatement() const override;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Command& other) const;
	bool operator!= (const Command& other) const;

private:
	Command(const std::string& name);

public:
	static const Command Var;
	static const Command Const;
	static const Command Define;
	static const Command Import;
	static const Command If;
	static const Command Else;
	static const Command Every;
};



class CommandArguments : public CodeFragment, public _ArgumentsList
{
public:
	CommandArguments();

	bool isStatement() const override;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const CommandArguments& other) const;
	bool operator!= (const CommandArguments& other) const;
};




class Instruction;

class Scope : public Statement
{
	using iterator = std::vector<Instruction*>::iterator;
	using const_iterator = std::vector<Instruction*>::const_iterator;

private:
	std::vector<Instruction*> _instructions;

public:
	Scope();
	~Scope();

	bool empty() const;
	size_t size() const;

	template<class _InstTy>
	void add(const _InstTy& inst)
	{
		Instruction* ptr = new _InstTy{ arg };
		_instructions.push_back(ptr);
	}

	Instruction& operator[] (const size_t idx);
	const Instruction& operator[] (const size_t idx) const;

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	CodeFragmentType getCodeFragmentType() const override;

	std::string toString() const override;

	bool operator== (const CodeFragment& cf) const override;
	bool operator== (const Scope& other) const;
	bool operator!= (const Scope& other) const;
};
