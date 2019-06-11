#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include <map>

#include "config_and_consts.h"

namespace
{
	class _NativeDataType
	{
	private:
		uint8_t _id;
		std::string _name;

		bool _integerType;
		std::map<std::string, ScriptCode> _avByName;
		std::map<ScriptCode, std::string> _avByValue;

		ScriptCode _defvalue;

		_NativeDataType(const uint8_t id, const std::string& name);
		_NativeDataType(const uint8_t id, const std::string& name, const std::vector<std::pair<std::string, ScriptCode>>& availableValues, const std::string& defaultName, ScriptCode defaultValue);

	public:
		_NativeDataType(const _NativeDataType&) = delete;
		_NativeDataType(_NativeDataType&&) = delete;

		_NativeDataType& operator= (const _NativeDataType&) = delete;
		_NativeDataType& operator= (_NativeDataType&&) = delete;

		const std::string& name() const;

		std::vector<std::string> availableValues() const;

		bool isValidIdentifier(const std::string& identifier) const;
		bool isValidValue(ScriptCode value) const;

		std::string getValueIdentifier(ScriptCode value) const;
		ScriptCode getIdentifierValue(const std::string& identifier) const;

		bool operator== (const _NativeDataType& dt) const;
		bool operator!= (const _NativeDataType& dt) const;


	public:
		static const _NativeDataType Integer;
		static const _NativeDataType State;
		static const _NativeDataType Team;
		static const _NativeDataType Spell;
		static const _NativeDataType Follower;
		static const _NativeDataType Building;
	};
}
