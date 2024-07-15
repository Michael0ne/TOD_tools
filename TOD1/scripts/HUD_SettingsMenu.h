class HUD_SettingsMenu : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_SettingsMenu", "Node", false, false);
		
		//	TODO: add members and methods to HUD_SettingsMenu.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_SettingsMenu::Create();