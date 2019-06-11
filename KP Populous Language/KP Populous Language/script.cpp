#include "script.h"
#include <cstring>
#include <fstream>

BadIndex::BadIndex(const char* const message) :
	exception{ message }
{}


ScriptCode& ScriptCodeAccessor::operator[] (int index)
{
	if (index < 0 || index >= MAX_CODES)
		throw BadIndex{ "Bad Index location in Script Codes" };
	return _codeData[index];
}

const ScriptCode& ScriptCodeAccessor::operator[] (int index) const
{
	if (index < 0 || index >= MAX_CODES)
		throw BadIndex{ "Bad Index location in Script Codes" };
	return _codeData[index];
}

ScriptCodeAccessor::ScriptCodeAccessor(ScriptCode* const data) :
	_codeData{ data }
{}
ScriptCodeAccessor::ScriptCodeAccessor(const ScriptCode* const data) :
	_codeData{ const_cast<ScriptCode* const>(data) }
{}


ScriptField& ScriptFieldAccessor::operator[] (int index)
{
	if (index < 0 || index >= MAX_FIELDS)
		throw BadIndex{ "Bad Index location in Script Fields" };
	return _fieldData[index];
}

const ScriptField& ScriptFieldAccessor::operator[] (int index) const
{
	if (index < 0 || index >= MAX_FIELDS)
		throw BadIndex{ "Bad Index location in Script Fields" };
	return _fieldData[index];
}

ScriptFieldAccessor::ScriptFieldAccessor(ScriptField* const data) :
	_fieldData{ data }
{}
ScriptFieldAccessor::ScriptFieldAccessor(const ScriptField* const data) :
	_fieldData{ const_cast<ScriptField* const>(data) }
{}



Script::Script() :
	data{}
{
	clear();
}

ScriptCode& Script::code(int index)
{
	if (index < 0 || index >= MAX_CODES)
		throw BadIndex{ "Bad Index location in Script Codes" };
	return codeData[index];
}
const ScriptCode& Script::code(int index) const
{
	if (index < 0 || index >= MAX_CODES)
		throw BadIndex{ "Bad Index location in Script Codes" };
	return codeData[index];
}

void Script::setVersion()
{
	data[0] = SCRIPT_VERSION;
	data[1] = 0U;
}

uint16_t Script::getVersion() const { return *codeData; }

void Script::copyCodeDataFrom(Script& other)
{
	std::memcpy(codeData, other.codeData, sizeof(codeData));
	setVersion();
}

ScriptField& Script::field(int index)
{
	if (index < 0 || index >= MAX_FIELDS)
		throw BadIndex{ "Bad Index location in Script Fields" };
	return fieldData[index];
}

const ScriptField& Script::field(int index) const
{
	if (index < 0 || index >= MAX_FIELDS)
		throw BadIndex{ "Bad Index location in Script Fields" };
	return fieldData[index];
}

void Script::clear()
{
	std::memset(codeData, 0, sizeof(codeData));
	wide_memset(fieldData, ScriptField::invalid(), MAX_FIELDS);
}

ScriptCodeAccessor Script::codes() { return { codeData }; }
const ScriptCodeAccessor Script::codes() const { return { codeData }; }

ScriptFieldAccessor Script::fields() { return { fieldData }; }
const ScriptFieldAccessor Script::fields() const { return { fieldData }; }


void Script::read(std::istream& input)
{
	clear();

	if (input && !input.eof())
		input.read(reinterpret_cast<char*>(codeData), sizeof(codeData));
	if (input && !input.eof())
		input.read(reinterpret_cast<char*>(fieldData), sizeof(fieldData));
}

void Script::write(std::ostream& output) const
{
	if (output)
		output.write(reinterpret_cast<const char*>(data), sizeof(*this));
}


void Script::readFromFile(const std::string& file)
{
	std::fstream f{ file, std::ios::in | std::ios::binary };
	read(f);
	f.close();
}

void Script::writeToFile(const std::string& file) const
{
	std::fstream f{ file, std::ios::out | std::ios::binary };
	write(f);
	if (f)
		f.flush();
	f.close();
}
