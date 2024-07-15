class weapon_shell_move_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("weapon_shell_move_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to weapon_shell_move_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

weapon_shell_move_ctrl::Create();