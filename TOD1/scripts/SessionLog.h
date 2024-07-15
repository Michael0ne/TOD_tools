class SessionLog : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("SessionLog", "Node", false, false);
		
		//	TODO: add members and methods to SessionLog.
		
		inst->CalculateSize();
		
		return inst;
	};
};

SessionLog::Create();