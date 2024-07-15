class database_action_animslot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_action_animslot", "Node", false, false);
		
		//	TODO: add members and methods to database_action_animslot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_action_animslot::Create();