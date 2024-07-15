class Trig_disabler : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trig_disabler", "Node", false, false);
		
		//	TODO: add members and methods to Trig_disabler.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trig_disabler::Create();