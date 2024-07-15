class character_deathpose : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_deathpose", "Node", false, false);
		
		//	TODO: add members and methods to character_deathpose.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_deathpose::Create();