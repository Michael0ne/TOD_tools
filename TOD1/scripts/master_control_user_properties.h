class master_control_user_properties : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_user_properties", "Node", false, false);
		
		//	TODO: add members and methods to master_control_user_properties.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_user_properties::Create();