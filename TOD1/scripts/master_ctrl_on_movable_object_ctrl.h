class master_ctrl_on_movable_object_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_ctrl_on_movable_object_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_ctrl_on_movable_object_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_ctrl_on_movable_object_ctrl::Create();