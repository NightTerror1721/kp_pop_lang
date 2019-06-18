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




ScriptCodeBuilder::ScriptCodeBuilder() :
	_front{ nullptr },
	_back{ nullptr },
	_size{ 0 }
{}

ScriptCodeBuilder::~ScriptCodeBuilder()
{
	clear();
}

void ScriptCodeBuilder::clear()
{
	Node* node = _front;
	while (node)
	{
		Node* const next = node->next;
		delete node;
		node = next;
	}
	_front = nullptr;
	_back = nullptr;
	_size = 0;
}

CodeLocation ScriptCodeBuilder::push_back(const ScriptCode code)
{
	if (_size >= MAX_CODES)
		throw FullCodeData{};

	if (!_front)
	{
		_front = new Node{ this, code, nullptr, nullptr };
		_back = _front;
		++_size;
		return _front;
	}

	Node* const node{ new Node{ this, code, nullptr, _back } };
	_back->next = node;
	_back = node;
	++_size;
	return node;
}
CodeLocation ScriptCodeBuilder::push_front(const ScriptCode code)
{
	if (_size >= MAX_CODES)
		throw FullCodeData{};

	if (!_front)
	{
		_front = new Node{ this, code, nullptr, nullptr };
		_back = _front;
		++_size;
		return _front;
	}

	Node* const node{ new Node{ this, code, _front, nullptr } };
	_front->prev = node;
	_front = node;
	++_size;
	return node;
}

ScriptCode& ScriptCodeBuilder::front() { return _front->code; }
const ScriptCode& ScriptCodeBuilder::front() const { return _front->code; }

ScriptCode& ScriptCodeBuilder::back() { return _back->code; }
const ScriptCode& ScriptCodeBuilder::back() const { return _back->code; }

CodeLocation ScriptCodeBuilder::insert_before(const CodeLocation location, const ScriptCode code)
{
	if (location->builder != this)
		throw BadIndex{ "Invalid Builder" };
	if (_size >= MAX_CODES)
		throw FullCodeData{};
	
	Node* const base = const_cast<Node*>(location);
	if (!base->prev)
		return push_front(code);

	Node* node{ new Node{ this, code, base, base->prev } };
	base->prev->next = node;
	base->prev = node;
	++_size;
	return node;
}
CodeLocation ScriptCodeBuilder::insert_after(const CodeLocation location, const ScriptCode code)
{
	if (location->builder != this)
		throw BadIndex{ "Invalid Builder" };
	if (_size >= MAX_CODES)
		throw FullCodeData{};

	Node* const base = const_cast<Node*>(location);
	if (!base->next)
		return push_back(code);

	Node* node{ new Node{ this, code, base->next, base } };
	base->next->prev = node;
	base->next = node;
	++_size;
	return node;
}

uint16_t ScriptCodeBuilder::size() const { return _size; }
bool ScriptCodeBuilder::empty() const { return _size <= 0; }

void ScriptCodeBuilder::build(Script& script) const
{
	auto accessor = script.codes();
	uint16_t count = 0;
	for (Node* node = _front; node && count < MAX_CODES; node = node->next)
		accessor[count++] = node->code;
}

