class camera_sound_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("camera_sound_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to camera_sound_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

camera_sound_ctrl::Create();