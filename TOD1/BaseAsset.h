#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>

//	NOTE: this class is actually inherited from another class, but parent doesn't seem to do anything important, so skipping it now.
#pragma pack(4)
class Asset
{
public:
	enum PlatformId
	{
		PC = 0,
		PS2 = 1,
		XBOX = 2
	};

	enum ResourceBlockTypeNumber
	{
		NONE = 0,
		MAP = 1,
		SUBMAP = 1,
		MISSION = 2,
		CUTSCENE = 3,
		PLAYERDATA = 4
	};

	enum BlockTypeNumber
	{
		UNKNOWN = -1,
		NONE = 0,
		MAP = 1,
		SUBMAP = 2,
		MISSION = 3,
		CUTSCENE = 4,
		PLAYERDATA = 5,
		MAIN = 6
	};

public:
	const char*     m_ResourcePath;
	int             m_GlobalResourceId;	//	NOTE: this is an index for Blocks global 'ResourceTypeList'.
	int             field_C;
	UINT64          m_ResourceTimestamp;
	int             m_Flags;

public:
	virtual         ~Asset();	//	@851F90 scalar, actual dtor @8516C0
	virtual Asset*  GetInstancePtr() const = 0;
	virtual void    SetUnkFlag(unsigned char, int, int);
	virtual int     GetUnkFlag() const;
	virtual void    stub5(int) {};
	virtual void    GetResourcesDir(String& outDir, PlatformId platformId);
	virtual void    ApplyAssetData(int*);
	virtual char    SetResourcePlaceholder();
	virtual int     stub9();	//	NOTE: retrieves some unknown flag.
	virtual void    GetResourceName(String& outName, int);
	virtual void    LoadResource(const char* resPath);
	virtual void    DestroyResource();
	virtual int     stub13();

	Asset(bool);	//	@851D00

	const char*     AddResToOpenListAndReturnName();	//	@851720
	void            _851800(String& outstr, const char* inpath, bool a3, bool a4) const;	//	@851800
	void            _8513E0(unsigned char);	//	@8513E0
	void            ApplyLoadedResource(ResourceHolder&);
	void            EncodeCountryCode(const char* const countrycode);	//	@851480
	const char* const GetResourceCountryCode() const;	//	@851CC0

	static void     Destroy(Asset* res);	//	@851FC0

	static unsigned int	LastOpenResourceIndex;	//	@A3BE14
};

static unsigned int TotalResourcesCreated = NULL;	//	@A3BE10
static std::vector<String>	OpenResourcesList;	//	@A10F00

ASSERT_CLASS_SIZE(Asset, 28);