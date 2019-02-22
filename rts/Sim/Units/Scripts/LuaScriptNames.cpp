/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "LuaScriptNames.h"
#include "Sim/Misc/GlobalConstants.h"

// script function-indices never change, so this is fine wrt. reloading
static std::array<std::string, LUAFN_Last> scriptNames;
static spring::unordered_map<std::string, int> scriptMap;


const std::array<std::string, LUAFN_Last>& CLuaUnitScriptNames::GetScriptNames()
{
	if (!scriptNames[LUAFN_Destroy].empty())
		return scriptNames;

	scriptNames[LUAFN_Destroy]       = "Destroy";
	scriptNames[LUAFN_StartMoving]   = "StartMoving";
	scriptNames[LUAFN_StopMoving]    = "StopMoving";
	scriptNames[LUAFN_StartSkidding] = "StartSkidding";
	scriptNames[LUAFN_StopSkidding]  = "StopSkidding";
	scriptNames[LUAFN_ChangeHeading] = "ChangeHeading";
	scriptNames[LUAFN_Activate]      = "Activate";
	scriptNames[LUAFN_Killed]        = "Killed";
	scriptNames[LUAFN_Deactivate]    = "Deactivate";
	scriptNames[LUAFN_WindChanged]   = "WindChanged";
	scriptNames[LUAFN_ExtractionRateChanged] = "ExtractionRateChanged";
	scriptNames[LUAFN_RockUnit]      = "RockUnit";
	scriptNames[LUAFN_MoveRate]      = "MoveRate";
	scriptNames[LUAFN_SetSFXOccupy]  = "setSFXoccupy";
	scriptNames[LUAFN_HitByWeapon]   = "HitByWeapon";

	scriptNames[LUAFN_QueryLandingPads]     = "QueryLandingPads";
	scriptNames[LUAFN_Falling]              = "Falling";
	scriptNames[LUAFN_Landed]               = "Landed";
	scriptNames[LUAFN_BeginTransport]       = "BeginTransport";
	scriptNames[LUAFN_QueryTransport]       = "QueryTransport";
	scriptNames[LUAFN_TransportPickup]      = "TransportPickup";
	scriptNames[LUAFN_StartUnload]          = "StartUnload";
	scriptNames[LUAFN_EndTransport]         = "EndTransport";
	scriptNames[LUAFN_TransportDrop]        = "TransportDrop";
	scriptNames[LUAFN_StartBuilding]        = "StartBuilding";
	scriptNames[LUAFN_StopBuilding]         = "StopBuilding";
	scriptNames[LUAFN_QueryNanoPiece]       = "QueryNanoPiece";
	scriptNames[LUAFN_QueryBuildInfo]       = "QueryBuildInfo";

	scriptNames[LUAFN_MoveFinished] = "MoveFinished";
	scriptNames[LUAFN_TurnFinished] = "TurnFinished";

	// Also add the weapon aiming stuff
	scriptNames[LUAFN_QueryWeapon]   = "QueryWeapon";
	scriptNames[LUAFN_AimWeapon]     = "AimWeapon";
	scriptNames[LUAFN_AimShield]     = "AimShield"; // TODO: maybe not the best name?
	scriptNames[LUAFN_AimFromWeapon] = "AimFromWeapon";
	scriptNames[LUAFN_FireWeapon]    = "FireWeapon";
	scriptNames[LUAFN_EndBurst]      = "EndBurst";
	scriptNames[LUAFN_Shot]          = "Shot";
	scriptNames[LUAFN_BlockShot]     = "BlockShot";
	scriptNames[LUAFN_TargetWeight]  = "TargetWeight";

	return scriptNames;
}


const spring::unordered_map<std::string, int>& CLuaUnitScriptNames::GetScriptMap()
{
	if (!scriptMap.empty())
		return scriptMap;

	const auto& n = GetScriptNames();

	scriptMap.reserve(n.size());

	for (size_t i = 0; i < n.size(); ++i) {
		scriptMap.insert(n[i], i);
	}

	return scriptMap;
}


int CLuaUnitScriptNames::GetScriptNumber(const std::string& fname)
{
	const auto& scriptMap = GetScriptMap();
	const auto it = scriptMap.find(fname);

	if (it != scriptMap.end())
		return it->second;

	return -1;
}

const std::string& CLuaUnitScriptNames::GetScriptName(int num)
{
	const static std::string empty;
	const auto& n = GetScriptNames();

	if (num >= 0 && num < int(n.size()))
		return n[num];

	return empty;
}

