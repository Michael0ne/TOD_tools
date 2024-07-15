class prop_move_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("prop_move_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to prop_move_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

prop_move_ctrl::Create();