class character_animation_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_animation_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_animation_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_animation_ctrl::Create();