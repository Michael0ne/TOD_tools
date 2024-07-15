class database_dual_skill_index : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_dual_skill_index", "Node", false, false);
		
		//	TODO: add members and methods to database_dual_skill_index.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_dual_skill_index::Create();