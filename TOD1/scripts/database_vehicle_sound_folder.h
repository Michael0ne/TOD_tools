class database_vehicle_sound_folder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_vehicle_sound_folder", "Node", false, false);
		
		//	TODO: add members and methods to database_vehicle_sound_folder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_vehicle_sound_folder::Create();