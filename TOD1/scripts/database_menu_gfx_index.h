class database_menu_gfx_index : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_menu_gfx_index", "Node", false, false);
		
		//	TODO: add members and methods to database_menu_gfx_index.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_menu_gfx_index::Create();