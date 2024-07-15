class Trigger_Activate_Overall_Score : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Overall_Score", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Overall_Score.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Overall_Score::Create();