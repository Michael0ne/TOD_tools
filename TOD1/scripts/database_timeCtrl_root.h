class database_timeCtrl_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_timeCtrl_root", "Node", false, false);
		
		//	TODO: add members and methods to database_timeCtrl_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_timeCtrl_root::Create();