class Trigger_Activate_Sound : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Sound", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Sound.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Sound::Create();