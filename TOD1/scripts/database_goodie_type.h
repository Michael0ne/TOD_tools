class database_goodie_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_goodie_type", "Node", false, false);
		
		//	TODO: add members and methods to database_goodie_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_goodie_type::Create();