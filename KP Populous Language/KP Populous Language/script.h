#pragma once

#define SCRIPT_VERSION 12U
#define MAX_CODES 4096U
#define MAX_FIELDS 512U
#define MAX_VARS 64U

#include <cinttypes>
#include <exception>
#include <string>
#include <istream>
#include <ostream>

#include "config_and_consts.h"

#define CODES_ARRAY_SIZE (MAX_CODES * sizeof(ScriptCode))
#define FIELDS_ARRAY_SIZE (MAX_FIELDS * sizeof(ScriptField))
#define EMPTY_DATA_ARRAY_SIZE 264
#define SCRIPT_SIZE (CODES_ARRAY_SIZE + FIELDS_ARRAY_SIZE + EMPTY_DATA_ARRAY_SIZE)

class BadIndex : public std::exception
{
public:
	BadIndex(const char* const message);
};

struct Script;

class ScriptCodeAccessor
{
private:
	ScriptCode* const _codeData;

public:
	ScriptCode& operator[] (int index);
	const ScriptCode& operator[] (int index) const;

	friend struct Script;

private:
	ScriptCodeAccessor(ScriptCode* const data);
	ScriptCodeAccessor(const ScriptCode* const data);
};

class ScriptFieldAccessor
{
private:
	ScriptField* const _fieldData;

public:
	ScriptField& operator[] (int index);
	const ScriptField& operator[] (int index) const;

	friend struct Script;

private:
	ScriptFieldAccessor(ScriptField* const data);
	ScriptFieldAccessor(const ScriptField* const data);
};

struct Script
{
	union
	{
		byte_t data[SCRIPT_SIZE];
		struct
		{
			ScriptCode codeData[MAX_CODES];
			ScriptField fieldData[MAX_FIELDS];
			byte_t unusedData[EMPTY_DATA_ARRAY_SIZE];
		};
	};

	Script();

	ScriptCode& code(int index);
	const ScriptCode& code(int index) const;

	ScriptCodeAccessor codes();
	const ScriptCodeAccessor codes() const;

	void setVersion();
	uint16_t getVersion() const;

	void copyCodeDataFrom(Script& other);

	ScriptField& field(int index);
	const ScriptField& field(int index) const;

	ScriptFieldAccessor fields();
	const ScriptFieldAccessor fields() const;

	void clear();

	void read(std::istream& input);
	void write(std::ostream& output) const;

	void readFromFile(const std::string& file);
	void writeToFile(const std::string& file) const;

};
