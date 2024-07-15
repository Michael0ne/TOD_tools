class database_Mission_loadscreen : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Mission_loadscreen", "Node", false, false);
		
		//	TODO: add members and methods to database_Mission_loadscreen.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Mission_loadscreen::Create();