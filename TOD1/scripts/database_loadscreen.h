class database_loadscreen : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_loadscreen", "Node", false, false);
		
		//	TODO: add members and methods to database_loadscreen.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_loadscreen::Create();