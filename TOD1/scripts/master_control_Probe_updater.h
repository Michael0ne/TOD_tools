class master_control_Probe_updater : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_Probe_updater", "Node", false, false);
		
		//	TODO: add members and methods to master_control_Probe_updater.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_Probe_updater::Create();