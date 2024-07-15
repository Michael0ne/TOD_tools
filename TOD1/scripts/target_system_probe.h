class target_system_probe : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_system_probe", "Node", false, false);
		
		//	TODO: add members and methods to target_system_probe.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_system_probe::Create();