class weapon_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("weapon_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to weapon_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

weapon_placeholder::Create();