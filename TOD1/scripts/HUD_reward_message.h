class HUD_reward_message : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_reward_message", "Node", false, false);
		
		//	TODO: add members and methods to HUD_reward_message.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_reward_message::Create();