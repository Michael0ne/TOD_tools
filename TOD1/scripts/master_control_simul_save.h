class master_control_simul_save : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_simul_save", "Node", false, false);
		
		//	TODO: add members and methods to master_control_simul_save.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_simul_save::Create();