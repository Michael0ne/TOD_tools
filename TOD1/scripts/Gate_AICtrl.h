class Gate_AICtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Gate_AICtrl", "Node", false, false);
		
		//	TODO: add members and methods to Gate_AICtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Gate_AICtrl::Create();