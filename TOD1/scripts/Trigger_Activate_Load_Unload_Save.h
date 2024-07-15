class Trigger_Activate_Load_Unload_Save : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Load_Unload_Save", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Load_Unload_Save.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Load_Unload_Save::Create();