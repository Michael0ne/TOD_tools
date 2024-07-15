class database_timeCtrl_Stamp : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_timeCtrl_Stamp", "Node", false, false);
		
		//	TODO: add members and methods to database_timeCtrl_Stamp.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_timeCtrl_Stamp::Create();