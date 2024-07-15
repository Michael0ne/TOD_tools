class character_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_root", "Node", false, false);
		
		//	TODO: add members and methods to character_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_root::Create();