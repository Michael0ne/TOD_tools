class camera_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("camera_root", "Node", false, false);
		
		//	TODO: add members and methods to camera_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

camera_root::Create();