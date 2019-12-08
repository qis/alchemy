#include "IMemPool.h"

void Test_IMemPool(void)
{
	IMemPool <UInt32, 3>	pool;

	SKSE_DMESSAGE("main: pool test");
	gLog.Indent();

	SKSE_DMESSAGE("start");
	pool.Dump();

	UInt32	* data0, * data1, * data2;

	data0 = pool.Allocate();
	SKSE_DMESSAGE("alloc0 = %08X", data0);
	pool.Dump();

	data1 = pool.Allocate();
	SKSE_DMESSAGE("alloc1 = %08X", data1);
	pool.Dump();

	data2 = pool.Allocate();
	SKSE_DMESSAGE("alloc2 = %08X", data2);
	pool.Dump();

	SKSE_DMESSAGE("free0 %08X", data0);
	pool.Free(data0);
	pool.Dump();

	data0 = pool.Allocate();
	SKSE_DMESSAGE("alloc0 = %08X", data0);
	pool.Dump();

	SKSE_DMESSAGE("free2 %08X", data2);
	pool.Free(data2);
	pool.Dump();

	SKSE_DMESSAGE("done");
	pool.Dump();

	gLog.Outdent();
}
