class database_decal : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_decal", "Node", false, false);
		
		//	TODO: add members and methods to database_decal.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_decal::Create();