class target_system_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_system_root", "Node", false, false);
		
		//	TODO: add members and methods to target_system_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_system_root::Create();