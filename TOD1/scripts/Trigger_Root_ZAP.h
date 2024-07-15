class Trigger_Root_ZAP : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Root_ZAP", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Root_ZAP.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Root_ZAP::Create();