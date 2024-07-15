class database_structure_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_structure_type", "Node", false, false);
		
		//	TODO: add members and methods to database_structure_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_structure_type::Create();