class database_combo_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_combo_type", "Node", false, false);
		
		//	TODO: add members and methods to database_combo_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_combo_type::Create();