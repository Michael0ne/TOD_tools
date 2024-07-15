class database_havoc_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_havoc_type", "Node", false, false);
		
		//	TODO: add members and methods to database_havoc_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_havoc_type::Create();