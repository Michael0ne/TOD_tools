class database_taxipoint : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_taxipoint", "Node", false, false);
		
		//	TODO: add members and methods to database_taxipoint.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_taxipoint::Create();