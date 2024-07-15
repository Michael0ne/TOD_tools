class Combo_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Combo_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Combo_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Combo_Ctrl::Create();