class Trigger_Activate_Trig_Noaction_Reactivate : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Trig_Noaction_Reactivate", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Trig_Noaction_Reactivate.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Trig_Noaction_Reactivate::Create();