class HUD_reward_power_up : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_reward_power_up", "Node", false, false);
		
		//	TODO: add members and methods to HUD_reward_power_up.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_reward_power_up::Create();