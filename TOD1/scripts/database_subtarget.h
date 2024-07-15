class database_subtarget : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_subtarget", "Node", false, false);
		
		//	TODO: add members and methods to database_subtarget.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_subtarget::Create();