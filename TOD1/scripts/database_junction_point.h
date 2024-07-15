class database_junction_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_junction_point", "Node", false, false);
		
		//	TODO: add members and methods to database_junction_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_junction_point::Create();