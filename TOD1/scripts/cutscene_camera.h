class cutscene_camera : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_camera", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_camera.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_camera::Create();