class character_deathpose_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_deathpose_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to character_deathpose_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_deathpose_placeholder::Create();