#pragma once
#include "EntityType.h"

class BuiltinType : public EntityType
{
	typedef void* (BuiltinType::* Getter)(void) const;
	typedef void (BuiltinType::* Setter)(void*);

	struct BuiltinMember
	{
		BaseType*	m_RetType;
		Getter		m_Getter;
		Setter		m_Setter;
		String		m_Proto;
		String		m_Str_2;
	};

	struct BuiltinHandler
	{
	protected:
		String		m_Prototype;
		void*		(*m_Handler)(void*);
		String		m_Name;

	public:
		inline BuiltinHandler(const char* _prot, void* (*_hndlr)(void*), const char* _name);
	};

private:
	std::vector<BuiltinHandler>	m_HandlersList;
	std::map<int, int>			m_MembersMap;
	int							field_94;

	BuiltinType();	//	@486D00

	void	RegisterMemberFunction(
		BaseType* returntype,
		const char* membername,
		Getter,
		Setter,
		const char* memberproto,
		const char* a6);	//	@486D90

	void	RegisterHandler(
		const char* handlesignature,
		void* a2,
		const char* handlermsg);	//	@486430

public:
	void	Sin(float*);	//	@487C10
	void	Cos(float*);	//	@487C30
	void	Tan(float*);	//	@487C50
	void	Asin(float*);	//	@487C70
	void	Acos(float*);	//	@487C90
	void	Atan(float*);	//	@487CB0
	void	Abs(int*);	//	@487CE0
	void	Fabs(float*);	//	@487CD0
	void	Sqrt(float*);	//	@487D20
	void	Floor(float*);	//	@487D00
	void	Ceil(float*);	//	@487D30
	void	Clamp(float*);	//	@487D50
	void	Testbits(int*);	//	@487D90
	void	Setbit(int*);	//	@487DB0
	void	Getbit(int*);	//	@487DD0
	void	Rand_seed(int*);	//	@484F70
	void	Rand_integer(int*);	//	@484F80
	void	Rand_number(float*);	//	@484FA0
	void	Get_facecoll_MaterialID(int*);	//	@485D30
	void	GetTime(float*);	//	@487DF0
	void	Print(int*);	//	@484FB0
	void	IsKeyDown(int*);	//	@485210
	void	IsKeyPressed(int*);	//	@485250
	void	IsKeyReleased(int*);	//	@485280
	void	DrawPoint(int*);	//	@4855F0
	void	DrawLine(int*);	//	@4855B0
	void	DrawLine2D(int*);	//	@4855C0
	void	DrawSphere(int*);	//	@485600
	void	ProfileBegin(int*);	//	@484F50
	void	ProfileEnd(int*);	//	@484F60
	void	NumberToInteger(void*);	//	@487E30
	void	IntegerToNumber(void*);	//	@487E20
	void	PrintStack(int*);	//	@4852B0
	void	GenericCall(int*);	//	@485300
	void	QuadTreeQuery(int*);	//	@486950
	void	AuxQuadTreeQuery(int*);	//	@4865A0
	void	SetSelectedSoundrenderer(int*);	//	@484FE0
	void	SfxMuteAll(int*);	//	@485070
	void	SfxIsMuteAll(int*);	//	@485090
	void	AllocateGlobalStreamedSound(int*);	//	@4850B0
	void	DeallocateGlobalStreamedSound(int*);	//	@4850E0
	void	PlayGlobalStreamedSound(int*);	//	@4850F0
	void	StopGlobalStreamedSound(int*);	//	@485110
	void	SetVolumePitchGlobalStreamedSound(int*);	//	@485130
	void	GetDefaultFxVolumeVar(float*);	//	@485150
	void	GetDefaultAmbienceVolumeVar(float*);	//	@485170
	void	GetDefaultMusicVolumeVar(float*);	//	@485190
	void	GetDefaultSpeaksVolumeVar(float*);	//	@4851B0
	void	SetVolModifierOnGroup(int*);	//	@485870
	void	CutsceneDisableAware(int*);	//	@4851D0
	void	IsDebugConsoleActive(int*);	//	@4851E0
	void	DebugConsoleTextBox(int*);	//	@4851F0
	void	DebugConsolePrint(int*);	//	@485200
	void	GlobalKillAllEmmiters(int*);	//	@484FD0
	void	GetVersionNumber(char**);	//	@4856F0
	void	GetConfigString(int*);	//	@485770
	void	GetConfigTruth(int*);	//	@485310
	void	GetSessionVariableString(int*);	//	@4857F0
	void	SetSessionVariableString(int*);	//	@485390
	void	GetSessionVariableTruth(char*);	//	@485350
	void	SetSessionVariableTruth(int*);	//	@4853B0
	void	SetCurrentCountryCode(char*);	//	@487E10
	void	GetCurrentCountryCode(char*);	//	@4882F0
	void	SetDiscErrorMessage(int*);	//	@485610
	void	SetLodFactor(float*);	//	@4852E0
	void	LoadScene(char**);	//	@485CD0
	void	GetSystemLanguage(int*);	//	@4855D0
	void	StartCleanupDashboard(int*);	//	@4853E0
	void	SetScreenResolution(int*);	//	@4853F0
	void	GetScreenResolution(float*);	//	@485420
	void	IsScreenResolutionAvailable(int*);	//	@485460
	void	IsWideScreen(int*);	//	@485640
	void	SetVirtualHudScreenSize(float*);	//	@485480
	void	GetVirtualHudScreenSize(float*);	//	@4854C0
	void	GetScreenTopInVirtualUnits(float*);	//	@485500
	void	GetScreenBottomInVirtualUnits(float*);	//	@485520
	void	GetScreenLeftInVirtualUnits(float*);	//	@485540
	void	GetScreenRightInVirtualUnits(float*);	//	@485560
	void	DisableCurrentLoadScreen(int*);	//	@485580
	void	GetEditorActive(bool*);	//	@4853D0
	void	DumptableCreate(int*);	//	@486200
	void	DumptableCreateFromFile(int*);	//	@4862C0
	void	DumptableAddIntegerColumn(int*);	//	@4858C0
	void	DumptableAddNumberColumn(int*);	//	@4858F0
	void	DumptableAddStringColumn(int*);	//	@485920
	void	DumptableSetNumRows(int*);	//	@485950
	void	DumptableSetIntegerValue(int*);	//	@485970
	void	DumptableSetNumberValue(float*);	//	@4859A0
	void	DumptableSetStringValue(int*);	//	@4859D0
	void	DumptableWriteToFile(int*);	//	@485A00
	void	DumptableClose(int*);	//	@485A90
	void	EditorReloadAllAssets(int*);	//	@485590
	void	EditorSelectNode(int*);	//	@4855A0
	void	GetRegion(int*);	//	@485620
	void	GetMessageId(int*);	//	@488040
	void	QuitGame(int*);	//	@485660
	void	GetCoverdemoPlayMode(int*);	//	@485670
	void	GetCoverdemoInactiveTimeoutSec(int*);	//	@485690
	void	GetCoverdemoGameplayTimeoutSec(int*);	//	@4856B0
	void	CoverdemoExit(int*);	//	@4856D0

	static void	Register();	//	@486F30

	static const Vector4f		ZeroVector;		//	@9B7084
	static const Vector4f		RightVector;	//	@9B70A4
	static const Vector4f		UpVector;		//	@9B70B4
	static const Vector4f		InVector;		//	@9B70C4
	static const Orientation	Orient;			//	@9B70D4
	static const Vector4f		LeftVector;		//	@9B70E4
	static const Vector4f		DownVector;		//	@9B70F4
	static const Vector4f		OutVector;		//	@9B7104
	static const ColorRGB		ColorBlack;		//	@9B7114
	static const ColorRGB		ColorRed;		//	@9B7124
	static const ColorRGB		ColorGreen;		//	@9B7134
	static const ColorRGB		ColorYellow;	//	@9B7144
	static const ColorRGB		ColorDarkBlue;	//	@9B7154
	static const ColorRGB		ColorPink;		//	@9B7164
	static const ColorRGB		ColorBlue;		//	@9B7174
	static const ColorRGB		ColorWhite;		//	@9B7184
};

extern BuiltinType* tBuiltin;	//	@A3B578

ASSERT_CLASS_SIZE(BuiltinType, 152);