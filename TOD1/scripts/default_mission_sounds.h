class default_mission_sounds : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_mission_sounds", "Node", false, false);
		
		//	TODO: add members and methods to default_mission_sounds.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_mission_sounds::Create();