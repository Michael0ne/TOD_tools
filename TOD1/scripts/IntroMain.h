class IntroMain : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroMain", "Node", false, false);
		
		//	TODO: add members and methods to IntroMain.
		
		inst->CalculateSize();
		
		return inst;
	};
};

IntroMain::Create();