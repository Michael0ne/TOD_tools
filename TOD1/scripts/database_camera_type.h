class database_camera_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_camera_type", "Node", false, false);
		
		//	TODO: add members and methods to database_camera_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_camera_type::Create();