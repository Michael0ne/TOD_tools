class Destroyable_Object_Healthbar_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Destroyable_Object_Healthbar_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Destroyable_Object_Healthbar_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Destroyable_Object_Healthbar_Ctrl::Create();