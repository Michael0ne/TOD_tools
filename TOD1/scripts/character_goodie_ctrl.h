class character_goodie_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_goodie_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_goodie_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_goodie_ctrl::Create();