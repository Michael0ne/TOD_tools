class IntroInputCtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroInputCtrl", "Node", false, false);
		
		//	TODO: add members and methods to IntroInputCtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

IntroInputCtrl::Create();