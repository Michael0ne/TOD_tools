class camera_basic_ai : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("camera_basic_ai", "Node", false, false);
		
		//	TODO: add members and methods to camera_basic_ai.
		
		inst->CalculateSize();
		
		return inst;
	};
};

camera_basic_ai::Create();