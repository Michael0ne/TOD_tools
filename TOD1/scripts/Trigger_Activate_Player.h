class Trigger_Activate_Player : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Player", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Player.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Player::Create();