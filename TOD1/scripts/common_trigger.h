class common_trigger : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("common_trigger", "Node", false, false);
		
		//	TODO: add members and methods to common_trigger.
		
		inst->CalculateSize();
		
		return inst;
	};
};

common_trigger::Create();