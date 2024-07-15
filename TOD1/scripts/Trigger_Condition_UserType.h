class Trigger_Condition_UserType : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_UserType", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_UserType.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_UserType::Create();