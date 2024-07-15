class database_physic_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_physic_type", "Node", false, false);
		
		//	TODO: add members and methods to database_physic_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_physic_type::Create();