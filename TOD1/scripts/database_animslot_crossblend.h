class database_animslot_crossblend : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_animslot_crossblend", "Node", false, false);
		
		//	TODO: add members and methods to database_animslot_crossblend.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_animslot_crossblend::Create();