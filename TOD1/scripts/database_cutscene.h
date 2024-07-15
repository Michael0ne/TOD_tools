class database_cutscene : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_cutscene", "Node", false, false);
		
		//	TODO: add members and methods to database_cutscene.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_cutscene::Create();