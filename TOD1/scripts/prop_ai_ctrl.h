class prop_ai_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("prop_ai_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to prop_ai_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

prop_ai_ctrl::Create();