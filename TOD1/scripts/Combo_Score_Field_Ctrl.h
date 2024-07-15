class Combo_Score_Field_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Combo_Score_Field_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Combo_Score_Field_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Combo_Score_Field_Ctrl::Create();