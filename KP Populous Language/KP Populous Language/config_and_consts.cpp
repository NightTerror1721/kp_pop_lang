#include "config_and_consts.h"

void ScriptField::invalidate()
{
	type = FieldType::Invalid;
	value = 3;
}

bool ScriptField::isInvalid() const { return type == FieldType::Invalid; }
bool ScriptField::isConstant() const { return type == FieldType::Constant; }
bool ScriptField::isUser() const { return type == FieldType::User; }
bool ScriptField::isInternal() const { return type == FieldType::Internal; }
