#pragma once


namespace RE
{
	struct FileHash
	{
		UInt32	fileNameHash;	// 0
		char	fileExt[4];		// 4
		UInt32	dirHash;		// 8
	};
	static_assert(sizeof(FileHash) == 0xC);
}
