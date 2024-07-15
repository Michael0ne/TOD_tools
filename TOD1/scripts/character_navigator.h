class character_navigator : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_navigator", "Node", false, false);
		
		//	TODO: add members and methods to character_navigator.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_navigator::Create();