class database_combo_award : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_combo_award", "Node", false, false);
		
		//	TODO: add members and methods to database_combo_award.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_combo_award::Create();