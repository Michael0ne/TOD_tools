class global_weapon_list : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("global_weapon_list", "Node", false, false);
		
		//	TODO: add members and methods to global_weapon_list.
		
		inst->CalculateSize();
		
		return inst;
	};
};

global_weapon_list::Create();