class character_sync_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_sync_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_sync_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_sync_ctrl::Create();