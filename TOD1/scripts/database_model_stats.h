class database_model_stats : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_model_stats", "Node", false, false);
		
		//	TODO: add members and methods to database_model_stats.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_model_stats::Create();