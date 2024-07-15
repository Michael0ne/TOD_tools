class Trigger_Root_Menu : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Root_Menu", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Root_Menu.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Root_Menu::Create();