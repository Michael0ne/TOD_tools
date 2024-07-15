class HUD_intermission : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_intermission", "Node", false, false);
		
		//	TODO: add members and methods to HUD_intermission.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_intermission::Create();