class Trigger_Condition_Mission_More : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Mission_More", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Mission_More.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Mission_More::Create();