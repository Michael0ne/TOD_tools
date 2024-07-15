class Character_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Character_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Character_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Character_ctrl::Create();