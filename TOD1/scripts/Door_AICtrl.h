class Door_AICtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Door_AICtrl", "Node", false, false);
		
		//	TODO: add members and methods to Door_AICtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Door_AICtrl::Create();