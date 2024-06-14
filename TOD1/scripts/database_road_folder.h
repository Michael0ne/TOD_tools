class Database_road_folder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Database_road_folder", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("TrafficMaxDensity:integer"), "2", NULL);
		inst->AddMember(GetScriptIdByFullName("TrafficMaxSpeed:number"), "15.000000", NULL);
		inst->AddMember(GetScriptIdByFullName("TrafficCurrentDensity:integer"), "0", NULL);
		inst->AddMember(GetScriptIdByFullName("TrafficAllowLargeVehicles:truth"), "truth", NULL);

		inst->AddMethod(487, _650830, _6508A0);

		inst->AddLocal(_650830, nullptr);

		inst->CalculateSize();
		return inst;
	};

	static void _650830(ScriptThread* pThread)
	{
		return;
	}

	static void _6508A0(ScriptThread* pThread, void* pArgs)
	{
		return;
	}
};

Database_road_folder::Create();