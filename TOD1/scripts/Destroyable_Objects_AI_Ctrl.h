class Destroyable_Objects_AI_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Destroyable_Objects_AI_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Destroyable_Objects_AI_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Destroyable_Objects_AI_Ctrl::Create();