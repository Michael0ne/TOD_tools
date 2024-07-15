class target_trig_forwarder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_trig_forwarder", "Node", false, false);
		
		//	TODO: add members and methods to target_trig_forwarder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_trig_forwarder::Create();