#pragma once

#include "skse64/PapyrusEvents.h"

#include "RE/Actor.h"
#include "RE/BSFixedString.h"
#include "RE/NiSmartPointer.h"
#include "RE/TESCameraState.h"
#include "RE/TESForm.h"
#include "RE/TESObjectREFR.h"


namespace SKSE
{
	struct ModCallbackEvent
	{
		ModCallbackEvent(RE::BSFixedString a_eventName, RE::BSFixedString a_strArg, float a_numArg, RE::TESForm* a_sender) :
			eventName(a_eventName),
			strArg(a_strArg),
			numArg(a_numArg),
			sender(a_sender)
		{}


		RE::BSFixedString	eventName;
		RE::BSFixedString	strArg;
		float				numArg;
		RE::TESForm*		sender;
	};
	static_assert(offsetof(ModCallbackEvent, eventName) == offsetof(SKSEModCallbackEvent, eventName));
	static_assert(offsetof(ModCallbackEvent, strArg) == offsetof(SKSEModCallbackEvent, strArg));
	static_assert(offsetof(ModCallbackEvent, numArg) == offsetof(SKSEModCallbackEvent, numArg));
	static_assert(offsetof(ModCallbackEvent, sender) == offsetof(SKSEModCallbackEvent, sender));
	static_assert(sizeof(ModCallbackEvent) == sizeof(SKSEModCallbackEvent));


	struct CameraEvent
	{
		CameraEvent(RE::TESCameraState* a_oldState, RE::TESCameraState* a_newState) :
			oldState(a_oldState),
			newState(a_newState)
		{}


		RE::TESCameraState*	oldState;
		RE::TESCameraState*	newState;
	};
	static_assert(offsetof(CameraEvent, oldState) == offsetof(SKSECameraEvent, oldState));
	static_assert(offsetof(CameraEvent, newState) == offsetof(SKSECameraEvent, newState));
	static_assert(sizeof(CameraEvent) == sizeof(SKSECameraEvent));


	struct CrosshairRefEvent
	{
		explicit CrosshairRefEvent(RE::NiPointer<RE::TESObjectREFR> a_crosshairRef) :
			crosshairRef(a_crosshairRef)
		{}


		RE::NiPointer<RE::TESObjectREFR> crosshairRef;
	};
	static_assert(offsetof(CrosshairRefEvent, crosshairRef) == offsetof(SKSECrosshairRefEvent, crosshairRef));
	static_assert(sizeof(CrosshairRefEvent) == sizeof(SKSECrosshairRefEvent));


	struct ActionEvent
	{
		enum class Type : UInt32
		{
			kWeaponSwing = 0,
			kSpellCast = 1,
			kSpellFire = 2,
			kVoiceCast = 3,
			kVoiceFire = 4,
			kBowDraw = 5,
			kBowRelease = 6,
			kBeginDraw = 7,
			kEndDraw = 8,
			kBeginSheathe = 9,
			kEndSheathe = 10
		};


		enum class Slot : UInt32
		{
			kLeft = 0,
			kRight = 1,
			kVoice = 2
		};


		ActionEvent(Type a_type, RE::Actor* a_actor, RE::TESForm* a_sourceForm, Slot a_slot) :
			type(a_type),
			actor(a_actor),
			sourceForm(a_sourceForm),
			slot(a_slot)
		{}


		Type			type;
		RE::Actor*		actor;
		RE::TESForm*	sourceForm;
		Slot			slot;
	};
	static_assert(offsetof(ActionEvent, type) == offsetof(SKSEActionEvent, type));
	static_assert(offsetof(ActionEvent, actor) == offsetof(SKSEActionEvent, actor));
	static_assert(offsetof(ActionEvent, sourceForm) == offsetof(SKSEActionEvent, sourceForm));
	static_assert(offsetof(ActionEvent, slot) == offsetof(SKSEActionEvent, slot));
	static_assert(sizeof(ActionEvent) == sizeof(SKSEActionEvent));


	struct NiNodeUpdateEvent
	{
		explicit NiNodeUpdateEvent(RE::TESObjectREFR* a_reference) :
			reference(a_reference)
		{}


		RE::TESObjectREFR* reference;
	};
	static_assert(offsetof(NiNodeUpdateEvent, reference) == offsetof(SKSENiNodeUpdateEvent, reference));
	static_assert(sizeof(NiNodeUpdateEvent) == sizeof(SKSENiNodeUpdateEvent));
}
