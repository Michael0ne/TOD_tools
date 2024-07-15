class character_action_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_action_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_action_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_action_ctrl::Create();