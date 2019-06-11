#include "datatypes.h"

#include <algorithm>

#include "config_and_consts.h"

_NativeDataType::_NativeDataType(const uint8_t id, const std::string& name) :
	_id{ id },
	_name{ name },
	_integerType{ true },
	_avByName{},
	_avByValue{},
	_defvalue{ 0 }
{}

_NativeDataType::_NativeDataType(
	const uint8_t id,
	const std::string& name,
	const std::vector<std::pair<std::string, ScriptCode>>& availableValues,
	const std::string& defaultName,
	ScriptCode defaultValue) :
	_id{ id },
	_name{ name },
	_integerType{ false },
	_avByName{},
	_avByValue{},
	_defvalue{ defaultValue }
{
	for (const auto& aval : availableValues)
	{
		if (!aval.first.empty())
			_avByName[aval.first] = aval.second;
		_avByValue[aval.second] = defaultName;
	}
}

const std::string& _NativeDataType::name() const { return _name; }

std::vector<std::string> _NativeDataType::availableValues() const
{
	std::vector<std::string> vec{ _avByName.size() };
	for (const auto& v : _avByName)
		vec.push_back(v.first);
	return std::move(vec);
}

bool _NativeDataType::isValidIdentifier(const std::string& identifier) const
{
	return _avByName.find(identifier) != _avByName.end();
}

bool _NativeDataType::isValidValue(ScriptCode value) const
{
	return _avByValue.find(value) != _avByValue.end();
}

std::string _NativeDataType::getValueIdentifier(ScriptCode value) const
{
	const auto& it = _avByValue.find(value);
	return it != _avByValue.end() ? it->second : "";
}

ScriptCode _NativeDataType::getIdentifierValue(const std::string& identifier) const
{
	const auto& it = _avByName.find(identifier);
	return it != _avByName.end() ? it->second : 0;
}

bool _NativeDataType::operator== (const _NativeDataType& dt) const { return _id == dt._id; }
bool _NativeDataType::operator!= (const _NativeDataType& dt) const { return _id != dt._id; }






const _NativeDataType _NativeDataType::Integer{ 0, "Integer" };

const _NativeDataType _NativeDataType::State{ 1, "State", {
	{ "on", InstructionToken::On },
	{ "off", InstructionToken::Off }
}, "off", InstructionToken::Off };

const _NativeDataType _NativeDataType::Team{ 2, "Team", {
	{ "Blue", CommandValueToken::Blue },
	{ "Red", CommandValueToken::Red },
	{ "Yellow", CommandValueToken::Yellow },
	{ "Green", CommandValueToken::Green }
}, "Blue", CommandValueToken::Blue };

const _NativeDataType _NativeDataType::Spell{ 3, "Spell", {
	{ "", ReadOnlyInternal::Burn },
	{ "Blast", ReadOnlyInternal::Blast },
	{ "Lightning", ReadOnlyInternal::LightningBolt },
	{ "", ReadOnlyInternal::Whirlwind },
	{ "Swarm", ReadOnlyInternal::InsectPlague },
	{ "Invisibility", ReadOnlyInternal::Invisibility },
	{ "Hypnotism", ReadOnlyInternal::Hypnotism },
	{ "Firestorm", ReadOnlyInternal::Firestorm },
	{ "GhostArmy", ReadOnlyInternal::GhostArmy },
	{ "Erosion", ReadOnlyInternal::Erosion },
	{ "Swamp", ReadOnlyInternal::Swamp },
	{ "LandBridge", ReadOnlyInternal::LandBridge },
	{ "AngelOfDead", ReadOnlyInternal::AngelOfDead },
	{ "Earthquake", ReadOnlyInternal::Earthquake },
	{ "Flatten", ReadOnlyInternal::Flatten },
	{ "Volcano", ReadOnlyInternal::Volcano },
	{ "Armageddon", ReadOnlyInternal::WrathOfGod },
	{ "Shield", ReadOnlyInternal::Shield },
	{ "Convert", ReadOnlyInternal::Convert },
	{ "Teleport", ReadOnlyInternal::Teleport },
	{ "Bloodlust", ReadOnlyInternal::Bloodlust },
	{ "UndefinedSpell", ReadOnlyInternal::NoSpecificSpell }
}, "Blast", ReadOnlyInternal::Blast };

const _NativeDataType _NativeDataType::Follower{ 4, "Follower", {
	{ "Brave", ReadOnlyInternal::Brave },
	{ "Warrior", ReadOnlyInternal::Warrior },
	{ "Religious", ReadOnlyInternal::Religious },
	{ "Spy", ReadOnlyInternal::Spy },
	{ "Firewarrior", ReadOnlyInternal::Firewarrior },
	{ "Shaman", ReadOnlyInternal::Shaman },
	{ "UndefinedFollower", ReadOnlyInternal::NoSpecificPerson }
}, "Brave", ReadOnlyInternal::Brave };

const _NativeDataType _NativeDataType::Building{ 5, "Building", {
	{ "SmallHut", ReadOnlyInternal::SmallHut },
	{ "MediumHut", ReadOnlyInternal::MediumHut },
	{ "LargeHut", ReadOnlyInternal::LargeHut },
	{ "DrumTower", ReadOnlyInternal::DrumTower },
	{ "Temple", ReadOnlyInternal::Temple },
	{ "SpyTrain", ReadOnlyInternal::SpyTrain },
	{ "WarriorTrain", ReadOnlyInternal::WarriorTrain },
	{ "FirewarriorTrain", ReadOnlyInternal::FirewarriorTrain },
	{ "", ReadOnlyInternal::Reconversion },
	{ "", ReadOnlyInternal::WallPiece },
	{ "", ReadOnlyInternal::Gate },
	{ "BoatHut", ReadOnlyInternal::BoatHut },
	{ "", ReadOnlyInternal::BoatHut2 },
	{ "AirshipHut", ReadOnlyInternal::AirshipHut },
	{ "", ReadOnlyInternal::AirshipHut2 },
	{ "UndefinedBuilding", ReadOnlyInternal::NoSpecificBuilding }
}, "SmallHut", ReadOnlyInternal::SmallHut };

