class database_aim_bracket_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_aim_bracket_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_aim_bracket_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_aim_bracket_ID::Create();