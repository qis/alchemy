#pragma once

#include "RE/BSTEvent.h"
#include "RE/GFxValue.h"
#include "RE/IMenu.h"


namespace RE
{
	class MenuOpenCloseEvent;


	class KinectMenu :
		public IMenu,							// 00
		public BSTEventSink<MenuOpenCloseEvent>	// 30
	{
	public:
		inline static const void* RTTI = RTTI_KinectMenu;


		virtual ~KinectMenu();																										// 00

		// override (IMenu)
		virtual Result	ProcessMessage(UIMessage* a_message) override;																// 04

		// override (BSTEventSink<MenuOpenCloseEvent>)
		virtual	EventResult	ReceiveEvent(MenuOpenCloseEvent* a_event, BSTEventSource<MenuOpenCloseEvent>* a_eventSource) override;	// 01


		// members
		GFxValue root;	// 38 - "Menu_mc"
	};
	static_assert(sizeof(KinectMenu) == 0x50);
}
