class Trigger_Activate_PlayerModel : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_PlayerModel", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_PlayerModel.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_PlayerModel::Create();