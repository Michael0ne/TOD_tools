class target_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to target_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_ctrl::Create();