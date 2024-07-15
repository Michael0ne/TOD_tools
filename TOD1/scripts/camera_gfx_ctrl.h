class camera_gfx_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("camera_gfx_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to camera_gfx_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

camera_gfx_ctrl::Create();