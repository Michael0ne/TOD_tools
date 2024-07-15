class character_bone_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_bone_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_bone_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_bone_ctrl::Create();