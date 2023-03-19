//SJCLib
#include <SJCLib.h>

namespace SJCMMLib
{
	struct SIndexEntry
	{
		uint32 offset;
		uint32 size;
		uint64 timestamp;
		bool isKeyFrame;
	};

	class CIndex
	{
	private:
		//Variables
		SJCLib::CMap<uint32, SJCLib::CArray<SIndexEntry>> indices;
	public:
		//Functions
		void Add(uint32 streamIndex, uint32 offset, uint32 size, uint64 timestamp, bool isKeyFrame);
		int32 Find(uint32 streamIndex, uint64 timestamp, bool backwards = false) const;
		const SJCLib::CArray<SIndexEntry> &GetStreamIndices(uint32 streamIndex) const;
		void Release();
	};
}