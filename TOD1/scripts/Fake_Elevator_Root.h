class Fake_Elevator_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Fake_Elevator_Root", "Node", false, false);
		
		//	TODO: add members and methods to Fake_Elevator_Root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Fake_Elevator_Root::Create();