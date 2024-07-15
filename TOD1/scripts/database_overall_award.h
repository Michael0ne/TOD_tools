class database_overall_award : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_overall_award", "Node", false, false);
		
		//	TODO: add members and methods to database_overall_award.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_overall_award::Create();