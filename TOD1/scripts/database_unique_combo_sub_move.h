class database_unique_combo_sub_move : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_unique_combo_sub_move", "Node", false, false);
		
		//	TODO: add members and methods to database_unique_combo_sub_move.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_unique_combo_sub_move::Create();