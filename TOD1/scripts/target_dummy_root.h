class target_dummy_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_dummy_root", "Node", false, false);
		
		//	TODO: add members and methods to target_dummy_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_dummy_root::Create();