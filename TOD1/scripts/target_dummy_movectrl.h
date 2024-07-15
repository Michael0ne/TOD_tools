class target_dummy_movectrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_dummy_movectrl", "Node", false, false);
		
		//	TODO: add members and methods to target_dummy_movectrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_dummy_movectrl::Create();