class HUD_combo_total_score_bar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_combo_total_score_bar", "Node", false, false);
		
		//	TODO: add members and methods to HUD_combo_total_score_bar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_combo_total_score_bar::Create();