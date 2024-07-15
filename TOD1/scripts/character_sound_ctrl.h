class character_sound_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_sound_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_sound_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_sound_ctrl::Create();