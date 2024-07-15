class Trigger_Activate_Radar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Radar", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Radar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Radar::Create();