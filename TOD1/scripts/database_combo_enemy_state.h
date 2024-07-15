class database_combo_enemy_state : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_combo_enemy_state", "Node", false, false);
		
		//	TODO: add members and methods to database_combo_enemy_state.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_combo_enemy_state::Create();