class Trigger_Activate_Fog : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Fog", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Fog.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Fog::Create();