class character_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to character_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_placeholder::Create();