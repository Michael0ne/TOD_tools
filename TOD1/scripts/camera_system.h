class camera_system : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("camera_system", "Node", false, false);
		
		//	TODO: add members and methods to camera_system.
		
		inst->CalculateSize();
		
		return inst;
	};
};

camera_system::Create();