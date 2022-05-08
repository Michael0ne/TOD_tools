class master_game_pulse_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_game_pulse_ctrl", "Node", false, false);

		inst->CalculateSize();

		return inst;
	};
};

master_game_pulse_ctrl::Create();