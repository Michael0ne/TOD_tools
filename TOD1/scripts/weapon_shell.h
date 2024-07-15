class weapon_shell : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("weapon_shell", "Node", false, false);
		
		//	TODO: add members and methods to weapon_shell.
		
		inst->CalculateSize();
		
		return inst;
	};
};

weapon_shell::Create();