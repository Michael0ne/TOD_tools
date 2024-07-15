class character_physic_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_physic_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_physic_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_physic_ctrl::Create();