#pragma once

#include "RE/ActorSpeedChannel.h"
#include "RE/BSFixedString.h"
#include "RE/BSIntrusiveRefCounted.h"
#include "RE/BSLock.h"
#include "RE/BSTAnimationGraphDataChannel.h"
#include "RE/BSTArray.h"
#include "RE/BSTEvent.h"
#include "RE/BSTSmartPointer.h"


namespace RE
{
	struct BSAnimationGraphEvent;
	class BShkbAnimationGraph;


	BSSmartPointer(BSAnimationGraphManager);


	class BSAnimationGraphManager :
		public BSTEventSink<BSAnimationGraphEvent>,	// 00
		public BSIntrusiveRefCounted				// 08
	{
	public:
		inline static const void* RTTI = RTTI_BSAnimationGraphManager;


		struct AnimationVariable
		{
			union Value
			{
				bool	b;
				SInt32	i;
				float	f;
			};
			static_assert(sizeof(Value) == 0x4);


			BSFixedString	name;	// 00
			Value*			value;	// 08
		};
		static_assert(sizeof(AnimationVariable) == 0x10);


		virtual ~BSAnimationGraphManager();																									// 00

		// override (BSTEventSink<BSAnimationGraphEvent>)
		virtual	EventResult	ReceiveEvent(BSAnimationGraphEvent* a_event, BSTEventSource<BSAnimationGraphEvent>* a_eventSource) override;	// 01


		// members
		UInt32																	unk0C;				// 0C
		BSTArray<BSTAnimationGraphDataChannel<Actor, float, ActorSpeedChannel>>	unk10;				// 10
		UInt64																	unk28;				// 28
		void*																	unk30;				// 30
		UInt64																	unk38;				// 38
		BSTSmallArray<BShkbAnimationGraph*>										animationGraphs;	// 40
		UInt64																	unk58;				// 58
		void*																	unk60;				// 60
		UInt64																	unk68;				// 68
		BSTArray<AnimationVariable>												animationVariables;	// 70
		void*																	unk88;				// 88
		BShkbAnimationGraph*													unk90;				// 90 - The active animation graph?
		UInt64																	unk98;				// 98
		mutable BSUniqueLock													unkA0;				// A0
		UInt64																	unkA8;				// A8
	};
	static_assert(offsetof(BSAnimationGraphManager, animationGraphs) == 0x40);
	static_assert(sizeof(BSAnimationGraphManager) == 0xB0);
}
