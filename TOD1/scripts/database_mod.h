class database_mod : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mod", "Node", false, false);
		
		//	TODO: add members and methods to database_mod.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mod::Create();