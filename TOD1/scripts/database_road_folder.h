class database_road_folder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_road_folder", "Node", false, false);
		
		//	TODO: add members and methods to database_road_folder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_road_folder::Create();