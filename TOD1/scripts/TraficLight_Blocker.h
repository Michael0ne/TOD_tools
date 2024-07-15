class TraficLight_Blocker : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("TraficLight_Blocker", "Node", false, false);
		
		//	TODO: add members and methods to TraficLight_Blocker.
		
		inst->CalculateSize();
		
		return inst;
	};
};

TraficLight_Blocker::Create();