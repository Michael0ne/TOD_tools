class character_trigger_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_trigger_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_trigger_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_trigger_ctrl::Create();