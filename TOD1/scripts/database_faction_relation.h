class database_faction_relation : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_faction_relation", "Node", false, false);
		
		//	TODO: add members and methods to database_faction_relation.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_faction_relation::Create();