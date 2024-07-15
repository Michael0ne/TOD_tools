class database_animslot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_animslot", "Node", false, false);
		
		//	TODO: add members and methods to database_animslot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_animslot::Create();