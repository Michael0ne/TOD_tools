class database_combo_chain : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_combo_chain", "Node", false, false);
		
		//	TODO: add members and methods to database_combo_chain.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_combo_chain::Create();