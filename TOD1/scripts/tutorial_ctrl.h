class tutorial_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("tutorial_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to tutorial_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

tutorial_ctrl::Create();