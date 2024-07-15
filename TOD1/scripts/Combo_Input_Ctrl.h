class Combo_Input_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Combo_Input_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Combo_Input_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Combo_Input_Ctrl::Create();