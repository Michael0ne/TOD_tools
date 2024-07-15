class target_dummy_physic_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_dummy_physic_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to target_dummy_physic_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_dummy_physic_ctrl::Create();