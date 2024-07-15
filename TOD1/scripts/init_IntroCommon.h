class init_IntroCommon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("init_IntroCommon", "Node", false, false);
		
		//	TODO: add members and methods to init_IntroCommon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

init_IntroCommon::Create();