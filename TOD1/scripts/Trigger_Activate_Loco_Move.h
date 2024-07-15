class Trigger_Activate_Loco_Move : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Loco_Move", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Loco_Move.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Loco_Move::Create();