class tutorial_menu_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("tutorial_menu_root", "Node", false, false);
		
		//	TODO: add members and methods to tutorial_menu_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

tutorial_menu_root::Create();