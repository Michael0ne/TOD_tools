class damage_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("damage_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to damage_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

damage_ctrl::Create();