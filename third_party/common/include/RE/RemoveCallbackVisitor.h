#pragma once

#include "RE/FxDelegate.h"
#include "RE/FxDelegateHandler.h"


namespace RE
{
	class RemoveCallbackVisitor : public FxDelegateHandler::CallbackProcessor
	{
	public:
		inline static const void* RTTI = RTTI_RemoveCallbackVisitor;


		RemoveCallbackVisitor(FxDelegate::CallbackHash* a_hash);
		virtual ~RemoveCallbackVisitor() = default;															// 00

		// override (FxDelegateHandler::CallbackProcessor)
		virtual void Process(const GString& a_methodName, FxDelegateHandler::CallbackFn a_method) override;	// 01


		// members
		FxDelegate::CallbackHash* hash;	// 08
	};
	static_assert(sizeof(RemoveCallbackVisitor) == 0x10);
}
