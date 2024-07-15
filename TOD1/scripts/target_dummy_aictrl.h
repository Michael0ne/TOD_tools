class target_dummy_aictrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_dummy_aictrl", "Node", false, false);
		
		//	TODO: add members and methods to target_dummy_aictrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_dummy_aictrl::Create();