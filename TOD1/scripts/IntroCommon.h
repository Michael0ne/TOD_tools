class IntroCommon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroCommon", "Node", false, false);
		
		//	TODO: add members and methods to IntroCommon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

IntroCommon::Create();