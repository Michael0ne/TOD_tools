class database_animslot_actioninfo_for_motions : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_animslot_actioninfo_for_motions", "Node", false, false);
		
		//	TODO: add members and methods to database_animslot_actioninfo_for_motions.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_animslot_actioninfo_for_motions::Create();