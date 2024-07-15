class IntroStringDB : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroStringDB", "Node", false, false);
		
		//	TODO: add members and methods to IntroStringDB.
		
		inst->CalculateSize();
		
		return inst;
	};
};

IntroStringDB::Create();