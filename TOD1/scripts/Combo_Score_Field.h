class Combo_Score_Field : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Combo_Score_Field", "Node", false, false);
		
		//	TODO: add members and methods to Combo_Score_Field.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Combo_Score_Field::Create();